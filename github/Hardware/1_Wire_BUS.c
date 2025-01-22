# include <STC89C5xRC.H>
# include <INTRINS.H>

sbit OneWire_DQ = P3^7;

unsigned char OneWire_Start(void)
{
	unsigned char i;
	unsigned char Ack;
	OneWire_DQ = 1;
	OneWire_DQ = 0;
	i = 237;while (--i); // 500us	
	OneWire_DQ = 1;
	i = 32;while (--i); // 70us
	Ack = OneWire_DQ;
	i = 237;while (--i); // 500us 走完时序
	return Ack;
}

void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i;
	OneWire_DQ = 0;
	i = 4;while (--i); // 10us
	OneWire_DQ = Bit;
	i = 27;while (--i); // 60us
	OneWire_DQ = 1;
}

unsigned char OneWire_GetBit(void)
{
	unsigned char i;
	unsigned char Bit;
	OneWire_DQ = 0;
	i = 2;while (--i); // 5us
	OneWire_DQ = 1;
	i = 2;while (--i); // 5us
	Bit = OneWire_DQ;
	_nop_();i = 22;while (--i); // 50us
	return Bit;
}

void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		OneWire_SendBit(Byte & (0x01 << i));
	}
}

unsigned char OneWire_GetByte(void)
{
	unsigned char i;
	unsigned char Byte = 0x00;
	for(i = 0; i < 8; i++)
	{
		if(OneWire_GetBit()){
			Byte |= (0x01 << i);
		}
	}
	return Byte;
}



