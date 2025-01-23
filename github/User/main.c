# include <STC89C5xRC.H>
# include "SYSTEM_CLOCK.H"  		// 加入一个计时器
# include "PROCESS_CTL.H"
# include "DS18B20.H"
# include "UART.H"
# include "LCD1602.H"
# include "Key_button.h"
# include "EEPROM.H"
/**
  * @brief 初步探索时间片轮询的调度算法（自己发明一遍）
  */
void PSCtl_Ready_Update(void);
  
Process_Typedef Process_A, Process_B, Process_C, Process_D;
float xdata Get_T = 0;
Button_State Button_Now = BUTTON_FREE;


int main(void)
{
	SysClock_Init();
	UART_Init();
	LCD_Init();
	EEPROM_Init();
	PSCtl_Init(&Process_A);
	PSCtl_Init(&Process_B);
	PSCtl_Init(&Process_C);
	PSCtl_Init(&Process_D);
	PSCtl_Deinit(&Process_D);
	PSCtl_ChangeMinTime(&Process_A, 10);
	PSCtl_ChangeMinTime(&Process_B, 5);
	PSCtl_ChangeMinTime(&Process_C, 25);
	PSCtl_ChangeMinTime(&Process_D, 15);
	LCD_ShowStr(1, 1, "Temperature");
	while(1)
	{
		PSCtl_Ready_Update();
		if(PSCtl_IsReady(&Process_A))
		{
			DS18B20_ConverT();
			Get_T = DS18B20_ReadT();
			System_Clock += 10;
		}
		else if(PSCtl_IsReady(&Process_B))
		{
			unsigned char temp;
			temp = (unsigned char) Get_T;
			UART_SendByte(temp);
			System_Clock += 5;
		}
		else if(PSCtl_IsReady(&Process_C))
		{
			if(Get_T >= 0){
				LCD_ShowChar(2, 1, '+');
				LCD_ShowSignedNum(2, 2, (unsigned int)Get_T, 3);
				LCD_ShowChar(2, 5, '.');
				LCD_ShowSignedNum(2, 6, ((unsigned int)(Get_T*10000)) % 10000, 4);
			}
			else{
				LCD_ShowChar(2, 1, '-');
				LCD_ShowSignedNum(2, 2, (unsigned int)(-Get_T), 3);
				LCD_ShowChar(2, 5, '.');
				LCD_ShowSignedNum(2, 6, ((unsigned int)(-Get_T*10000)) % 10000, 4);
			}
			System_Clock += 25;
		}
		else if(PSCtl_IsReady(&Process_D))
		{
			if(EEPROM_CleanSector(EEPROM_Sector_1)){
				EEPROM_WriteByte((unsigned char) Get_T, EEPROM_Sector_1);
			}
			PSCtl_Deinit(&Process_D);
			System_Clock += 15;
		}
		
		/*
		下方是按钮的检测
		*/
		switch(Button_Now)
		{
			case BUTTON_FREE:
				if(Button_Pin == 0){
					Button_Clock = System_Clock;
					Button_Now = BUTTON_DELAY;
					Button_flag = 0;// 第几次进入delay的标记
				}
				break;
			case BUTTON_WAIT:
				if(System_Clock - Button_Clock > 1000){
					ACT_LONG_FLAG = 0;
					Button_Now = BUTTON_ACT_LONG;//进入长按功能				
				}
				else if(Button_Pin == 1){
					Button_Now = BUTTON_ACT_SHORT;//进入短按功能
				}	
				break;
			case BUTTON_DELAY:
				if(System_Clock - Button_Clock > 20){
					
					if(Button_flag == 0){
						Button_Now = BUTTON_WAIT;
						Button_Clock = System_Clock;
						Button_flag = 1;
					}
					else{
						Button_Now = BUTTON_FREE;
					}	
				}
				break;
			case BUTTON_ACT_SHORT:
				PSCtl_TurnInit(&Process_B);		// 翻转进程B
				//短按功能
				Button_Clock = System_Clock;
				Button_Now = BUTTON_DELAY; 
				break;
			case BUTTON_ACT_LONG:
				if(ACT_LONG_FLAG == 0){
					ACT_LONG_FLAG = 1; // 只执行一遍
					PSCtl_Init(&Process_D); // 开启进程D
					//长按功能
				}
				if(Button_Pin == 1){
					Button_Clock = System_Clock;
					Button_Now = BUTTON_DELAY; 
				}
				break;
		}
	}
}


/**
  *  @brief  更新各个进程的就绪情况，更新队列情况
  *  @param  None 
  *  @retval None
  */
void PSCtl_Ready_Update(void)
{
	if(System_Clock - Now_Clock >= Now_Wait_Time)
	{
		PSCtl_UpdateReady(&Process_A);
		PSCtl_UpdateReady(&Process_B);
		PSCtl_UpdateReady(&Process_C);
		PSCtl_UpdateReady(&Process_D);
		
		PSCtl_ChangeList(&Process_B);
		PSCtl_ChangeList(&Process_D);
	}
}



