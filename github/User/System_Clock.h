# ifndef __SYSTEM_CLOCK_H__
# define __SYSTEM_CLOCK_H__

/**
  * @attention Timer2 已被占用
  */

extern unsigned long int System_Clock;
extern unsigned long int Now_Clock;
extern unsigned long int xdata Button_Clock;

extern void SysClock_Init(void);

# endif /* __SYSTEM_CLOCK_H__ */
