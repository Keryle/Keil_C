#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

void fillRectangle(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color){
  if((x >= TFT_Width) || (y >= TFT_Height))
    return;
  if((x + w - 1) >= TFT_Width)
    w = TFT_Width  - x;
  if((y + h - 1) >= TFT_Height)
    h = TFT_Height - y;
  Lcd_SetRegion(x, y, x+w-1, y+h-1);
  for(y = h; y > 0; y--) {
    for(x = w; x > 0; x--) {
      Lcd_WriteData_16(color);
    }
  }
}

void fillPoint(unsigned char x, unsigned char y, unsigned int color ) { // 8X8 square
  unsigned char i;
  Lcd_SetRegion(x, y, x+7, y+7);
  for(i = 64; i > 0; i-- )
    Lcd_WriteData_16(color);
}

void main(void)
{
#ifdef MCU_STC12
  P3M1 &= ~(1<<2),	P3M0 |=  (1<<2);	//P3.2 set as push-pull output mode
#endif
  lcd_initial(); //Һ������ʼ��
  bl=1;//�������IO���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
  LCD_Clear(BLACK);		//��ɫ
  fillRectangle(50,50,60,60,RED);
  fillRectangle(20,0,8,8,RED);
	fillRectangle(0,0,8,8,RED);
  fillPoint(0,40,GREEN);


  while(1)
  {
	delay(500);
   }

}
