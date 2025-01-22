# ifndef __ONEWIRE_H__
# define __ONEWIRE_H__

extern unsigned char OneWire_Start(void);
extern void OneWire_SendBit(unsigned char Bit);
extern unsigned char OneWire_GetBit(void);
extern void OneWire_SendByte(unsigned char Byte);
extern unsigned char OneWire_GetByte(void);

# endif