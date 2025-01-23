# ifndef __PROCESS_CTL_H__
# define __PROCESS_CTL_H__

/**
  * @brief 进程状态参数设置
  * @param Enable_Flag: 当程序需要启动该进程时，置 1
  * @param Ready_Flag: 进展就绪时，置 1
  * @param Min_Occupy_Time: 最小占用毫秒数，某些通信协议
  */
typedef struct{
	unsigned char Enable_Flag;			// 需要该进程 置1
	unsigned char Ready_Flag;
	unsigned char Min_Occupy_Time;		// 按毫秒计算，默认为 1
	unsigned char List;
	unsigned char DeInit_Flag;			// 进程退出标志，用来处理PSCtl_TOP_List的正确改变
}Process_Typedef;

extern unsigned char PSCtl_TOP_List;
extern unsigned char Now_Wait_Time;

extern void PSCtl_Init(Process_Typedef * The_Process);
extern void PSCtl_Deinit(Process_Typedef * The_Process);
extern void PSCtl_TurnInit(Process_Typedef * The_Process);
extern void PSCtl_ChangeMinTime(Process_Typedef * The_Process, unsigned char NewTime);
extern unsigned char PSCtl_IsReady(Process_Typedef * The_Process);
extern void PSCtl_UpdateReady(Process_Typedef * The_Process);
extern void PSCtl_ChangeList(Process_Typedef * The_Process);

# endif /* __PROCESS_CTL_H__ */

