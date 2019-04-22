#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20];          //追踪背景
unsigned int xdata groundx[20];       //背景
unsigned char data trackSquare[6];    //追踪区域
unsigned char data tetris[5] = {0x00,0x06,0x04,0x04,0};       //俄罗斯方块
unsigned char count = 1, Down_Flag = 0, Move_flag = 0;        //T0计数，移动标志
unsigned char square_x = 3, square_y = 0, rotate = 0;         //方块坐标，旋转标志
unsigned int code tetrisData[6][4] = {                        //方块数组
  {0x0c88,0x08e0,0x0226,0x0e20},
  {0x06c0,0x08c4,0x06c0,0x08c4},
  {0x0c60,0x04c8,0x0c60,0x04c8},
  {0x088c,0x02e0,0x0622,0x0e80},
  {0x0e40,0x0464,0x04e0,0x04c4},
  {0x0660,0x0660,0x0660,0x0660}
};
unsigned int code *pTetris = tetrisData[0];    //方块指针
unsigned int code *PTimer;    //随机生成方块指针
unsigned char tcount = 0;     //T1计数
sbit Left = P1^3;             //按键定义
sbit Right = P1^2;
sbit Rota = P1^1;
sbit fast = P1^0;
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

//  每行用一个int数据保存
/*  墙    1  2  3  4  5  6  7  8  9  10 11   墙14
    11 |  x  x  x  x  x  x  x  x  x  x  x  | 111      0xc007
*/
void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //移动到当前行
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (9-x)); //数据类型转换，读取追踪区域，14-6+1-x=9-x
    p++;
  }
}

void trackSquare_Write(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  unsigned int a, and = 0x003f;         // 低6位置1
  p += y;
  and <<= (9-x);                        // 左移到追踪区域
  for(i = 0; i < 6; i++ ){
    a = trackSquare[i] << (9-x);
    *p &= ~and;                         //取反变0，相与，清空追踪区域
    *p |= a;                            //或 新的数据
    p++;
  }

}
void tetris_Storage(unsigned char x, unsigned char y, unsigned char *p){
  unsigned int xdata *pgroundx = groundx;
  unsigned char i;
  unsigned int a;
  pgroundx += y;
  for(i = 0; i < 6; i++ ){
    a = *p & 0x3f;                   //屏蔽高两位
    a <<= (9-x);
    *pgroundx |= a;                  //或 数据
    p++;
    pgroundx++;
  }
/*  a = 0x8000;                     //debug
  for(i = 0; i < 16;i++){
    if(groundx[16] & a)
      fillPoint(i, 0, RED);
    a >>= 1;
  }
*/
}

//Show trackSquare
unsigned char showTrackSquare_Down(unsigned char x, unsigned char y, unsigned char direction){
  signed char i,j;
  unsigned char data temptrack[6],aa[6];
  unsigned char data *pTrack =trackSquare;
  unsigned char data *pTemp = temptrack;
  unsigned char row;
  unsigned int  row_int;
  for(i = 0; i < 6; i++)
    temptrack[i] = *pTrack++; //暂存上次的追踪区域

  for(i = 0; i < 6; i++)
    trackSquare[i] = groundx[y+i] >> (9-x) & 0x3f; //读取背景数据到tracksquare

  if(direction == 0)                               //下降
    for(i = 0; i < 4; i++){
      row = tetris[i] << 1;                       //将俄罗斯方块区域移到追踪区域的中心
      if(tetris[i+1] & trackSquare[4-i] >> 1){    //tracksquare左移对位，与俄罗斯方块区域与操作，为真则触底
        tetris_Storage(x,y,temptrack);            //俄罗斯方块第2行与追踪区域第4行比较
        square_y = 0;     //初始化x坐标
        square_x = 3;     //初始化y坐标
        pTetris = PTimer; //pTetris指向随机方块生成指针的数据，且pTetris将用于旋转操作
        row_int = *pTetris;
        for(i = 0; i < 4; i++){
          tetris[3-i] = row_int & 0x0f;            //写入方块区域
          row_int >>= 4;
        }
        return 1;
      }
      trackSquare[5-i] |= row;                     //下移生成新的track区域
    }
  else
    if(direction == 1)                             //左移
      for(i = 0; i < 4; i++){
        row = tetris[i] << 2;
        if(tetris[i] & trackSquare[4-i] >> 2)
          return 1;
        trackSquare[4-i] |= row;
      }
    else
      if(direction == 2)                           //右移
        for(i = 0; i < 4; i++){
          row = tetris[i];
          if(tetris[i] & trackSquare[4-i])
            return 1;
          trackSquare[4-i] |= row;
        }
      else
        if(direction == 3){                        //旋转
          row_int = *(pTetris + rotate);
          rotate++;
          if(rotate > 3)
            rotate = 0;
          for(i = 0; i < 4; i++){
            tetris[3-i] = row_int & 0x0f;          //写入数据到方块区域
            row_int >>= 4;
          }
          for(i = 0; i < 4; i++){
            row = tetris[i] << 1;
            if(tetris[i] & trackSquare[4-i] >> 1){ //判断是否能进行旋转
              rotate--;
              row_int = *(pTetris + rotate);
              for(i = 0; i < 4; i++){
                tetris[3-i] = row_int & 0x0f;      //不能旋转，方块数据返回前一次的值
                row_int >>= 4;
              }
              return 1;
            }
            trackSquare[4-i] |= row;
          }
        }
        else
          return 1;
  pTemp = &temptrack[0];
  pTrack = trackSquare;
  for(i = 0; i < 6; i++){
    aa[i] = *pTrack ^ *pTemp;
    pTrack++;
    pTemp++;                               //异或得到改变值，存入aa
  }

  for(i = 0; i < 6; i++){                 //逐行扫描
    if(aa[i]){                            //非空判断
      row = 0x20;
      for(j = 0; j < 6; j++){             //行内扫描
        if(aa[i] & row ){                 //判断改变位
          if(temptrack[i] & row)          //和暂存区域比较与操作，为真则变暗，为假则点亮
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

//消除行函数
void tetris_Clear(void)
{
  unsigned int xdata *pRead = groundx;
  unsigned int xdata *pWrite;
  unsigned int xdata groundx_bak[20],aa[20];
  unsigned char i,co = 0;
  unsigned int row_int;
  for(i = 0; i < 20; i++)
    groundx_bak[i] = *pRead++;
  pRead = &groundx_bak[18];     //从倒数第二行读数据
  pWrite = &groundx[18];        //从倒数第二行写数据
  for(i = 0; i < 18; i++){
    if(*pRead == 0xffff)
      pRead--;                  //一行全为1 则跳过这一行
    *pWrite = *pRead;           //写入
    if(*pRead == 0)             //遇到空行 写入0
      *pWrite = 0;
    pRead--;
    pWrite--;
  }
  if(pRead == pWrite)
    return;                           //若指针指向地址相同，没有满行，返回主函数
  pRead = groundx;
  pWrite = groundx_bak;
  for(i = 0; i < 20; i++)
    aa[i] = *pRead++ ^ *pWrite++;     //异或得到改变位存入aa,pwrite
  pRead = groundx_bak;
  pWrite = aa;
  for(i = 0; i < 20; i++){            //扫描行
    if(*pWrite){                      //判断空行
      row_int = 0x8000;
      for(co = 0; co < 20; co++){     //行内扫描
        if(*pWrite & row_int){        //按位判断
          if(*pRead & row_int)        //与备份数据比较
          fillPoint(co, i, BLACK);
          else
          fillPoint(co, i, RED);
        }
      row_int >>= 1;
      }
    }
    pWrite++;
    pRead++;
  }
}

void main(void)
{
  unsigned char i;
  PT0 = 1;
  TMOD = 0x11;
  TH0 = 0x3C;
  TL0 = 0xB0;
  TH1 = 0xa6;
  TL1 = 0x28;
  EA = 1;
  ET0 = 1;
  ET1 = 1;
  TR0 = 1;                  //定时器T0定时50ms
  TR1 = 1;                  //定时器T1定时23ms
  P1 = 0x0f;                //矩阵键盘设置初值
  for(i = 0; i < 20; i++){  //游戏区域左右写入1，左右边界
    groundx[i] = 0xc007;
    Area[i]=0xc007;
  }
  groundx[19] = 0xffff;     //设置下边界
  Area[19] = 0xffff;
  lcd_initial();            //显示屏初始化，具体参见basic.c
  bl=1;                     //背光灯
  //游戏界面显示
  LCD_Clear(BLACK);		               //背景黑色
  fillRectangle(0,0,16,160,BLUE);    //边框蓝色
  fillRectangle(0,152,128,8,BLUE);
  fillRectangle(104,0,24,160,BLUE);

  while(1)
  {
    if(Down_Flag){                                      //downflag由T0置位，定时1.5s
      trackSquare_Read(square_x,square_y);              //从Area读取数据到Tracksquare
      if(!showTrackSquare_Down(square_x, square_y, 0)){ //下降操作，返回1表示触底
        trackSquare_Write(square_x,square_y);           //将tracksquare中的数据写入Area
        square_y++;                  //纵坐标自增
      }
      else{
        tetris_Clear();             //消除满行
        for(i = 0; i < 20; i++)
        Area[i]=groundx[i];         //ͬ同步数据
      }
      Down_Flag = 0;                //清零
    }
    if(!Left && Move_flag){         //左移按键判断，Move_flag由T0置位，定时1.1s
      delay(20);
      if(!Left){                    //延时再次判断
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 1)){
          trackSquare_Write(square_x,square_y);
          square_x--;
        }
      }
    }
    if(!Right && Move_flag){        //右移按键判断
      delay(20);
      if(!Right){
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 2)){
          trackSquare_Write(square_x,square_y);
          square_x++;
        }
      }
    }
    if(!Rota && Move_flag){        //旋转判断
      delay(20);
      if(!Rota){
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 3))
          trackSquare_Write(square_x,square_y);
      }
    }
    if(!fast){                    //加速下降
      delay(20);
      if(!fast)
        Down_Flag = 1;
    }
  }

}

void Timer0() interrupt 1
{
  TH0 = 0x3C;
  TL0 = 0xB0;                //定时50ms
  count++;
  if(count <= 22)           //定时22*50=1.1s
    Move_flag = 1;
  else
    Move_flag = 0;

  if(count >= 30){          //定时1.5s
    count = 0;
    Down_Flag = 1;
  }
}

void Timer1() interrupt 3   //随机指向方块数组
{
  TH1 = 0xa6;
  TL1 = 0x28;
  if(tcount > 5)
    tcount = 0;
  PTimer = tetrisData[tcount];
  tcount++;
}
