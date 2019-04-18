#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20],groundx[20];
unsigned int xdata *data pArea=Area;
unsigned char data trackSquare[6],ground[6];
unsigned char data tetris[5] = {0x00,0x0c,0x08,0x08,0};       //低四位存行数据
unsigned char square_x = 4, square_y = 0;

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
  x += 1;                               //向右平移两列
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
//数据储存结构：一个16位int类型代表一行
//高位为1代表左墙壁，低三位置1代表右墙壁
/*  墙    1  2  3  4  5  6  7  8  9  10 11   墙14
    11 |  x  x  x  x  x  x  x  x  x  x  x  | 111
*/
void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //数据开始行
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (14-6+1-x)) & 0; //取出6位数据
    p++;
  }
  p = groundx;
  p += y;           //数据开始行
  for(i = 0; i < 6; i++){
    ground[i] = (*p >> (14-6+1-x)); //取出6位数据
    p++;
  }

}

void trackSquare_Write(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i,j;
  unsigned int a;
  p += y;
  for(i = 0; i < 6; i++ ){
    a = trackSquare[i];
    a <<= (14-6+1-x);
    *p = a;
    p++;
  }

}

//Show trackSquare
unsigned char showTrackSquare_Down(unsigned char x, unsigned char y, unsigned int color){
  signed char i,j;
  unsigned char data temptrack[6],aa[6];
  unsigned char data *pTrack = trackSquare;
  unsigned char row;
  x += 1;                                          //游戏界面向右平移2格
  for(i = 0; i < 6; i++)
    temptrack[i] = *pTrack++;
  pTrack = &temptrack[5];
  for(i = 0; i <= 3; i++ ){
    pTrack--;
    row = tetris[i] << 1;
    *pTrack = (*pTrack) | row;             //temptrack区域与俄罗斯方块相或
  }
  pTrack = &ground[5];                  //指向第5行
  j = 5;
  for(i = 0; i <= 3; i++){
    row = tetris[i] << 1;
  //  if((*pTrack & row) > 0)
  //  return  1;
    trackSquare[j] = (*pTrack) | row;             //track区域与俄罗斯方块相或,保存这次移动操作的数据
    pTrack--;
    j--;
  }
  pTrack = &temptrack[0];
  for(i = 0; i < 6; i++){
    aa[i] = *pTrack ^ trackSquare[i];
    pTrack++;                             //异或得到改变位存入aa
  }
  for(i = 0; i < 6; i++){                 //逐行扫描
    if(aa[i]){                           //异或值为真，改变颜色
      row = 0x10;
      for(j = 0; j < 6; j++){             //行内扫描
        if(aa[i] & row ){                 //按位判断
          if(temptrack[i] & row)        //原来的值为真，改成背景色（黑色），否则填充颜色
            fillPoint(x+j+1,y+i,BLACK);
          else
            fillPoint(x+j+1,y+i,color);
        }
        row >>= 1;
      }
    }
  }
  return 0;
}


//游戏界面12X20，用一个16位数据保存一行，高4位无效，共20行，20个数据
void tetris_Storage(unsigned int cube){
  unsigned char i;
  for(i = 0; i < 4; i++){
    tetris[i] = cube & 0x0f;
    cube >>= 4;
  }
}
void main(void)
{
  unsigned char i;
  Area[18] = 0xffff;//游戏下边界
  Area[17] = 0xffff;
  lcd_initial(); //液晶屏初始化
  bl=1;//背光采用IO控制，也可以直接接到高电平常亮
  LCD_Clear(BLACK);		//黑色

  print_Tetris(square_x+1,5,0x88c0,YELLOW);
  delay(500);
  trackSquare_Read(square_x,4);
  showTrackSquare_Down(square_x,4,RED);
  trackSquare_Write(square_x,4);
  i = 0;
  Area[0] = 0x75a2;
  trackSquare_Read(11,0);
  trackSquare_Write(11,0);
  if(Area[0] == 0x00a2)
  fillPoint(0,3,GREEN);
  while(0){
    trackSquare_Read(square_x,5+i);
    showTrackSquare_Down(square_x, 5+i, RED );
  /*  if(showTrackSquare_Down(square_x, 5+i, RED ))
    break;
*/    trackSquare_Write(square_x,5+i);

    delay(1000);
    i++;
  }
  while(1)
  {
	delay(500);
   }

}
