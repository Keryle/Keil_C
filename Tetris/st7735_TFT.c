#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20];
unsigned int xdata *data pArea=Area;
unsigned char data trackSquare[6];
unsigned char xdata groundx[20];
unsigned char data tetris[5] = {0x00,0x0c,0x08,0x08,0};       //低四位存行数据
unsigned char data ground[5];
unsigned char square_x = 5, square_y = 0;

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

void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //数据开始行
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (9-x)); //取出6位数据，右移 11-6-x = 5-x位，再屏蔽高两位
    p++;
  }

  for(i = 0; i < 6; i++){
    ground[i] = (groundx[y+i] >> (9-x)); //取出6位数据，右移 11-6-x = 5-x位，再屏蔽高两位
    p++;
  }
}

void trackSquare_Write(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  unsigned int a;
  p += y;
  for(i = 0; i < 6; i++ ){
    a = trackSquare[i];
    a <<= (9-x);
    *p |= a;
    p++;
  }

}

//Show trackSquare
unsigned char showTrackSquare_Down(unsigned char x, unsigned char y, unsigned char direction){
  signed char i,j;
  unsigned char data temptrack[6],aa[6];
  unsigned char data *pTrack =trackSquare;
  unsigned char data *pTemp = temptrack;
  unsigned char row;
  for(i = 0; i < 6; i++)
    temptrack[i] = *pTrack++;

  for(i = 0; i < 6; i++ )
    trackSquare[i]=0;                              //清零

  if(direction == 0)                               //下降
    for(i = 0; i < 4; i++){
      row = tetris[i] << 1;
      if((ground[5-i] & row) > 0)
        return 1;
      trackSquare[5-i] = ground[5-i] | row;             //track区域与俄罗斯方块相或,保存这次移动操作的数据
      }
  else
    if(direction == 1)                             //左移
      for(i = 0; i < 4; i++){
        row = tetris[i] << 2;
        if((ground[4-i] & row) > 0)
          return 1;
        trackSquare[4-i] = ground[4-i] | row;
      }
    else
      if(direction == 2)                          //右移
        for(i = 0; i < 4; i++){
          row = tetris[i];
          if(ground[4-i] & row)
            return 1;
          trackSquare[4-i] = ground[4-i] | row;
        }
      else
        return 1;
  if(ground[2] == 0xc0)
    fillPoint(1,1,RED);
  pTemp = &temptrack[0];
  pTrack = trackSquare;
  for(i = 0; i < 6; i++){
    aa[i] = *pTrack ^ *pTemp;
    pTrack++;
    pTemp++;                            //异或得到改变位存入aa
  }
  if(aa[1] == 0x20)
  fillPoint(2,2,RED);
  for(i = 0; i < 6; i++){                 //逐行扫描
    if(aa[i]){                           //异或值为真，改变颜色
      row = 0x20;
      for(j = 0; j < 6; j++){             //行内扫描
        if(aa[i] & row ){                 //按位判断
          if(temptrack[i] & row)        //原来的值为真，改成背景色（黑色），否则填充颜色
            fillPoint(x+j+1,y+i,BLACK);
          else
            fillPoint(x+j+1,y+i,RED);
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
  for(i = 0; i < 6; i++){
    ground[i]=0;
  }
  for(i = 0; i < 20; i++){
    groundx[i] = 0xf007;
    Area[i]=0;
  }
  groundx[18] = 0xffff;
  lcd_initial(); //液晶屏初始化
  bl=1;//背光采用IO控制，也可以直接接到高电平常亮
  //LCD_Clear(WHITE);		//黑色

  print_Tetris(square_x+1,5,0x88c0,YELLOW);
  delay(500);
  trackSquare_Read(square_x,4);
  //showTrackSquare_Down(square_x, 5+i, 1 );
  trackSquare_Write(square_x,4);
  i = 0;
  while(1){
    trackSquare_Read(square_x,5);
    if(showTrackSquare_Down(square_x, 5, 0 ) == 1)
      break;
    trackSquare_Write(square_x,5);

    delay(1000);
    square_x--;
  }
  while(1)
  {
	delay(500);
   }

}
