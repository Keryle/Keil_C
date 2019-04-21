//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ����STC51
//1.44��TFT_ST7735Һ������
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����
//All rights reserved
//********************************************************************************

#include<reg51.h>
#include<absacc.h>
#include<intrins.h>
#include<string.h>
#define uchar unsigned char
#define uint unsigned int

#define ST7735_NOP     0x00     //��ָ��
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_VSCRSADD 0x37
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_PWCTR6  0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

//����Ӳ��ƽ̨��STC12LE5A60S2(3.3V��Ƭ��)
//��Ƶ��12MHZ
//---------------------------Һ��������˵��-------------------------------------//
//����ǰ��ο�Һ����˵�����10ҳ���Ŷ���
sbit bl        =P2^5;//��ģ��BL���ţ�������Բ���IO���ƻ���PWM���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
sbit scl       =P2^0;//��ģ��CLK����,������Pin9_SCL
sbit sda       =P2^1;//��ģ��DIN/MOSI���ţ�������Pin8_SDA
sbit rs        =P2^3;//��ģ��D/C���ţ�������Pin7_A0
sbit cs        =P2^4;//��ģ��CE���ţ�������Pin12_CS
sbit reset     =P2^2;//��ģ��RST���ţ�������Pin6_RES
//---------------------------End ofҺ��������---------------------------------//


//���峣����ɫ
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208

void delay(uint time)
{
 uint i,j;
  for(i=0;i<time;i++)
   for(j=0;j<250;j++);
}

//��SPI���ߴ���һ��8λ����
void  SPI_WriteData(uchar Data)
{
    unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)
		sda=1; //�������
		else sda=0;
		scl=0;
		scl=1;
		Data<<=1;
	}
}
//��Һ����дһ��8λָ��
void  Lcd_WriteIndex(uchar Data)
{

		cs=0;
		rs=0;
		SPI_WriteData(Data);
		cs=1;
}
//��Һ����дһ��8λ����
void  Lcd_WriteData(uchar Data)
{
		unsigned char i=0;
		cs=0;
		rs=1;
		SPI_WriteData(Data);
		cs=1;
}
//��Һ����дһ��16λ����
void  Lcd_WriteData_16(unsigned int Data)
{
	unsigned char i=0;
	cs=0;
	rs=1;
	SPI_WriteData(Data>>8); 	//д���8λ����
	SPI_WriteData(Data); 			//д���8λ����
	cs=1;

}
//LCD��λʱ��
void Reset(void)
{
    reset=0;
    delay(100);
    reset=1;
    delay(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//Һ������ʼ�� for S6D02A1
void lcd_initial(void)
{
	Reset();//Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit
	delay (120);

	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB2);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB3);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB4); //Column inversion
	Lcd_WriteData(0x07);

	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0);
	Lcd_WriteData(0xA2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x84);
	Lcd_WriteIndex(0xC1);
	Lcd_WriteData(0xC5);

	Lcd_WriteIndex(0xC2);
	Lcd_WriteData(0x0A);
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0xC3);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0x2A);
	Lcd_WriteIndex(0xC4);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0xEE);

	Lcd_WriteIndex(0xC5); //VCOM
	Lcd_WriteData(0x0E);

	Lcd_WriteIndex(0x36); //MX, MY, RGB mode
	Lcd_WriteData(0xC0);

	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1a);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x2f);
	Lcd_WriteData(0x28);
	Lcd_WriteData(0x20);
	Lcd_WriteData(0x22);
	Lcd_WriteData(0x1f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x23);
	Lcd_WriteData(0x37);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0xe1);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x17);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x2c);
	Lcd_WriteData(0x29);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x39);
	Lcd_WriteData(0x3f);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+3);

	Lcd_WriteIndex(0xF0); //Enable test command
	Lcd_WriteData(0x01);
	Lcd_WriteIndex(0xF6); //Disable ram power save mode
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0x3A); //65k mode
	Lcd_WriteData(0x05);


	Lcd_WriteIndex(0x29);//Display on

}
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x01);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x01);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x02);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x02);
	Lcd_WriteData(y_end+1);

	Lcd_WriteIndex(0x2c);

}

//ȫ����亯��
void LCD_Clear(unsigned int Color)
{
	uchar i,j;
	Lcd_SetRegion(0,0,127,159);
	for (i=0;i<160;i++)
	{
    	for (j=0;j<128;j++)
		{
        	Lcd_WriteData_16(Color);
		}
	}
}
