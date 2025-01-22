# include <STC89C5xRC.H>
# include "Process_Ctl.h"

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
	PSCtl_TOP_List ++;
}

void PSCtl_Deinit(Process_Typedef * The_Process)
{
	The_Process -> Enable_Flag = 0;
	PSCtl_TOP_List --;
}


void PSCtl_ChangeMinTime(Process_Typedef * The_Process, unsigned char NewTime)
{
	if(NewTime > 0 && NewTime < 11)
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








