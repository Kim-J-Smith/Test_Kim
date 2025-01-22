# include <STC89C5xRC.H>

unsigned long int System_Clock = 0; // 记录系统运行时刻
unsigned long int Now_Clock = 0;  	// 记录时间用于比较

void SysClock_Init(void)		//1ms @11.0592MHz
{
	T2MOD = 0;				//初始化模式寄存器
	T2CON = 0;				//初始化控制寄存器
	TL2 = 0x66;				//设置定时初始值
	TH2 = 0xFC;				//设置定时初始值
	RCAP2L = 0x66;			//设置定时重载值
	RCAP2H = 0xFC;			//设置定时重载值
	TR2 = 1;				//定时器2开始计时
	EA = 1;
	ET2 = 1;				//使能定时器2中断
	PT2 = 1;				//次低优先级
}

void Timer2_Isr(void) interrupt 5
{
	System_Clock ++;
	TF2 = 0;
}