# include <STC89C5xRC.H>
# include "Process_Ctl.h"
# include "System_Clock.h"

unsigned char PSCtl_TOP_List = 0; 	// 下一个init的进程的优先级 
unsigned char Now_Wait_Time = 0;

/**
  *  @brief  初始化进程，排序
  *  @param	 结构体 Process_Typedef
  *  @retval 无
  */
void PSCtl_Init(Process_Typedef * The_Process)
{
	The_Process -> Enable_Flag = 1;
	The_Process -> Ready_Flag = 0;
	The_Process -> Min_Occupy_Time = 1;
	The_Process -> List = PSCtl_TOP_List;
	The_Process -> DeInit_Flag = 0;
	PSCtl_TOP_List ++;
}

void PSCtl_Deinit(Process_Typedef * The_Process)
{
	The_Process -> Enable_Flag = 0;
	The_Process -> DeInit_Flag = 1;
}

void PSCtl_TurnInit(Process_Typedef * The_Process)
{
	The_Process -> Enable_Flag = !(The_Process -> Enable_Flag);
	if(The_Process -> Enable_Flag == 0){
		The_Process -> DeInit_Flag = 1;
	}
	else{
		The_Process -> Ready_Flag = 0;
		The_Process -> List = PSCtl_TOP_List;			// 重新加入序列
		The_Process -> DeInit_Flag = 0;
		PSCtl_TOP_List ++;
	}	
}

void PSCtl_ChangeMinTime(Process_Typedef * The_Process, unsigned char NewTime)
{
	if(NewTime > 0 && NewTime < 51)
	{
		The_Process -> Min_Occupy_Time = NewTime;
	}
}

unsigned char PSCtl_IsReady(Process_Typedef * The_Process)
{
	if(The_Process -> Enable_Flag == 1 && The_Process -> Ready_Flag == 1){
		return 1;
	}
	return 0;
}

/**
  *  @brief  更新进程序列
  *  @param  各个进程的结构体
  *  @retval 无
  */
void PSCtl_UpdateReady(Process_Typedef * The_Process)
{
	The_Process->List = (The_Process->List + PSCtl_TOP_List - 1) % PSCtl_TOP_List;
	The_Process->Ready_Flag = 0;
	if(The_Process->List == 0 && The_Process->Enable_Flag == 1)	// 排队结束，且ENABLE
	{
		The_Process->Ready_Flag = 1;						// 该进程已经就绪
		Now_Wait_Time = The_Process->Min_Occupy_Time;  		// 更新等待时间
		Now_Clock = System_Clock;							// 更新“此刻”
	}
}

/**
  *  @brief  根据DeInit_Flag调整序列
  *  @param  各个进程的结构体
  *  @retval 无
  *  @attention 该函数必须在各个进程都完成PSCtl_UpdateReady后才能使用
  */
void PSCtl_ChangeList(Process_Typedef * The_Process)
{
	if(The_Process->DeInit_Flag == 1 && The_Process->List == 0)
	{
		The_Process->DeInit_Flag = 0;
		PSCtl_TOP_List --;
	}
}

/**
  * @brief 以下为示例代码，Process_x （x可以是A B...）需要在主函数中定义
  * 	作用是更新进程序列，调整Ready_Flag。
  */
/*
void PSCtl_Ready_Update(void)
{
	if(System_Clock - Now_Clock >= Now_Wait_Time)
	{
		PSCtl_UpdateReady(&Process_A);
		PSCtl_UpdateReady(&Process_B);
		PSCtl_UpdateReady(&Process_C);
		...
		
		PSCtl_ChangeList(&Process_B);  	// 只处理可能要在程序中加入或退出的进程
		...
	}
}
*/


