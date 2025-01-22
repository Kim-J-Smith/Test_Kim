# include <STC89C5xRC.H>
# include "SYSTEM_CLOCK.H"  		// 加入一个计时器
# include "PROCESS_CTL.H"
# include "DS18B20.H"
# include "UART.H"
/**
  * @brief 初步探索时间片轮询的调度算法（自己发明一遍）
  */
void PSCtl_Ready_Update(void);
  
Process_Typedef Process_A, Process_B, Process_C;
float xdata Get_T = 0;

int main(void)
{
	SysClock_Init();
	UART_Init();
	PSCtl_Init(&Process_A);
	PSCtl_Init(&Process_B);
	PSCtl_Init(&Process_C);
	PSCtl_Deinit(&Process_C);
	PSCtl_ChangeMinTime(&Process_A, 10);
	PSCtl_ChangeMinTime(&Process_B, 5);
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
		Process_A.List = (Process_A.List + PSCtl_TOP_List - 1) % PSCtl_TOP_List;
		Process_B.List = (Process_B.List + PSCtl_TOP_List - 1) % PSCtl_TOP_List;
		Process_C.List = (Process_C.List + PSCtl_TOP_List - 1) % PSCtl_TOP_List;
		Process_A.Ready_Flag = 0;							// 就绪标志清空
		Process_B.Ready_Flag = 0;
		Process_C.Ready_Flag = 0;
		if(Process_A.List == 0 && Process_A.Enable_Flag == 1)	// 排队结束，且ENABLE
		{
			Process_A.Ready_Flag = 1;						// 该进程已经就绪
			Now_Wait_Time = Process_A.Min_Occupy_Time;  	// 更新等待时间
			Now_Clock = System_Clock;						// 更新“此刻”
		}
		else if(Process_B.List == 0 && Process_B.Enable_Flag == 1)
		{
			Process_B.Ready_Flag = 1;
			Now_Wait_Time = Process_B.Min_Occupy_Time;
			Now_Clock = System_Clock;
		}
		else if(Process_C.List == 0 && Process_C.Enable_Flag == 1)
		{
			Process_C.Ready_Flag = 1;
			Now_Wait_Time = Process_C.Min_Occupy_Time;
			Now_Clock = System_Clock;
		}
	}
}



