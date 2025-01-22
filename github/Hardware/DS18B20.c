# include <STC89C5xRC.H>
# include "1_WIRE_BUS.H"

# define SKIP_ROM 			0xCC
# define CONVERT_T 			0x44
# define READ_SCRATCHPAD 	0xBE

unsigned char xdata Value_EA = 0; //防止中断打乱时序

void DS18B20_ConverT(void)//转换温度
{
	Value_EA = EA;
	EA = 0;
	OneWire_Start();
	OneWire_SendByte(SKIP_ROM);
	OneWire_SendByte(CONVERT_T);
	EA = Value_EA;
}

float DS18B20_ReadT(void)
{
	float Get_T;
	int Temp = 0;
	unsigned char TLSB, TMSB;
	
	Value_EA = EA;
	EA = 0;
	OneWire_Start();
	OneWire_SendByte(SKIP_ROM);
	OneWire_SendByte(READ_SCRATCHPAD);
	
	TLSB = OneWire_GetByte();
	TMSB = OneWire_GetByte();
	EA = Value_EA;
	
	Temp = (TMSB << 8) | TLSB;
	Get_T = Temp / 16.0;
	return Get_T;
}



