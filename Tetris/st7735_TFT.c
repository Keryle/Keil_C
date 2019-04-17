#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20];
unsigned int xdata *data pArea=Area;
pArea += 4;

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
// 8X8 square
void fillPoint(unsigned char x, unsigned char y, unsigned int color ) {
  unsigned char i;
  x *= 8;
  y *= 8;
  Lcd_SetRegion(x, y, x+7, y+7);
  for(i = 64; i > 0; i-- )
    Lcd_WriteData_16(color);
}
//显示俄罗斯方块
void print_Tetris(unsigned char x, unsigned char y, unsigned int cube, unsigned int color){
  unsigned char row, i,bit_row,j;
  for(i = 0; i < 4; i++){               //4行
    row = cube & 0x0f;                  //取出低4位保存在row中
    cube >>= 4;
    bit_row = 0x08;
    for(j=0; j < 4; j++){
      if(row & bit_row)                 //按位判断，显示一行
        fillPoint(x+j,y+3-i,color);     //先显示数据的低4位，y值反而最大
      bit_row >>= 1;
    }
  }
}

//游戏界面12X20，用一个16位数据保存一行，高4位无效，共20行，20个数据
void dataStorageDwon(unsigned int cube){
  unsigned int row_1,row_2,row_3,row_4;
  row_4 = cube & 0x0f;
  cube >>= 4;
  row_3 = cube & 0x0f;
  cube >>= 4;
  row_2 = cube & 0x0f;
  cube >>= 4;
  row_1 = cube & 0x0f;
  row_1 <<= 4;
  row_2 <<= 4;
  row_3 <<= 4;
  row_4 <<= 4;             //居中

  if((*pArea & row_4) || (pArea > 19)){
    *pArea |= row_4;
    pArea--;
    *pArea |= row_3;
    pArea--;
    *pArea |= row_2;
    pArea--;
    *pArea |= row_1;
    pArea += 3;
  }
  dataStorge();
  pArea++;
  dataStorageWrite();

}
void main(void)
{
  lcd_initial(); //液晶屏初始化
  bl=1;//背光采用IO控制，也可以直接接到高电平常亮
  LCD_Clear(BLACK);		//黑色
  fillPoint(0,3,GREEN);
  print_Tetris(8,8,0x88c0,YELLOW);


  while(1)
  {
	delay(500);
   }

}
