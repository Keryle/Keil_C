#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20];
unsigned int xdata groundx[20];
unsigned char data trackSquare[6];
unsigned char data tetris[5] = {0x00,0x06,0x04,0x04,0};       //����λ��������
unsigned char count = 1, Down_Flag = 0, Move_flag = 0;
unsigned char square_x = 6, square_y = 0, rotate = 0;
unsigned int code tetrisData[6][4] = {
  {0x0c88,0x08e0,0x0226,0x0e20},
  {0x06c0,0x08c4,0x06c0,0x08c4},
  {0x0c60,0x04c8,0x0c60,0x04c8},
  {0x088c,0x02e0,0x0622,0x0e80},
  {0x0e40,0x0464,0x04e0,0x04c4},
  {0x0660,0x0660,0x0660,0x0660}
};
unsigned int code *pTetris = tetrisData[0];
sbit Left = P1^3;
sbit Right = P1^2;
sbit Rota = P1^1;
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
//��ʾ����˹����
void print_Tetris(unsigned char x, unsigned char y, unsigned int cube, unsigned int color){
  unsigned char row, i,bit_row,j;
  for(i = 0; i < 4; i++){               //4��
    row = cube & 0x0f;                  //ȡ����4λ������row��
    cube >>= 4;
    bit_row = 0x08;
    for(j=0; j < 4; j++){
      if(row & bit_row)                 //��λ�жϣ���ʾһ��
        fillPoint(x+j,y+3-i,color);     //����ʾ���ݵĵ�4λ��yֵ�������
      bit_row >>= 1;
    }
  }
}

//���ݴ���ṹ��һ��16λint���ʹ���һ��
//��λΪ1������ǽ�ڣ�����λ��1������ǽ��
/*  ǽ    1  2  3  4  5  6  7  8  9  10 11   ǽ14
    11 |  x  x  x  x  x  x  x  x  x  x  x  | 111      0xc007
*/
void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //���ݿ�ʼ��
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (9-x)); //ȡ��6λ���ݣ����� 14-6+1-x=9-x
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
    *p = a;
    p++;
  }

}
void tetris_Storage(unsigned char x, unsigned char y, unsigned char *p){
  unsigned int xdata *pgroundx = groundx;
  unsigned char i;
  unsigned int a;
  pgroundx += (y);
  for(i = 0; i < 6; i++ ){
    a = *p & 0x3f;                   //���θ���λ
    a <<= (9-x);
    *pgroundx |= a;                  //д����һ��tracksquare�е�����
    p++;
    pgroundx++;
  }
/*  a = 0x8000;
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
    temptrack[i] = *pTrack++;

  for(i = 0; i < 6; i++)
    trackSquare[i] = groundx[y+i] >> (9-x) & 0x3f; //��������д��tracksquare

  if(direction == 0)                               //�½�
    for(i = 0; i < 4; i++){
      row = tetris[i] << 1;
      if(tetris[i+1] & trackSquare[4-i] >> 1){    //tracksquare ��6λ�м�4λ�������ݣ���teteris����λ�������ݣ� ����һλ
        tetris_Storage(x,y,temptrack);
        square_y = 0;
        square_x = 3;
        return 1;
      }
      trackSquare[5-i] |= row;             //track���������˹�������,��������ƶ�����������
    }
  else
    if(direction == 1)                             //����
      for(i = 0; i < 4; i++){
        row = tetris[i] << 2;
        if(tetris[i] & trackSquare[4-i] >> 2)
          return 1;
        trackSquare[4-i] |= row;
      }
    else
      if(direction == 2)                          //����
        for(i = 0; i < 4; i++){
          row = tetris[i];
          if(tetris[i] & trackSquare[4-i])
            return 1;
          trackSquare[4-i] |= row;
        }
      else
        if(direction == 3){                     //��ת
          row_int = *(pTetris + rotate);
          rotate++;
          if(rotate > 3)
            rotate = 0;
          for(i = 0; i < 4; i++){
            tetris[3-i] = row_int & 0x0f;
            row_int >>= 4;
          }                                     //д��tetris
          for(i = 0; i < 4; i++){
            row = tetris[i] << 1;
            if(tetris[i] & trackSquare[4-i] >> 1){   //�ж��Ƿ������ת
              rotate--;
              row_int = *(pTetris + rotate);
              for(i = 0; i < 4; i++){
                tetris[3-i] = row_int & 0x0f;
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
    pTemp++;                            //���õ��ı�λ����aa
  }

  for(i = 0; i < 6; i++){                 //����ɨ��
    if(aa[i]){                           //���ֵΪ�棬�ı���ɫ
      row = 0x20;
      for(j = 0; j < 6; j++){             //����ɨ��
        if(aa[i] & row ){                 //��λ�ж�
          if(temptrack[i] & row)        //ԭ����ֵΪ�棬�ĳɱ���ɫ����ɫ�������������ɫ
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


//��Ϸ����12X20����һ��16λ���ݱ���һ�У���4λ��Ч����20�У�20������


void main(void)
{
  unsigned char i;
  PT0 = 1;
  TMOD = 0x01;
  TH0 = 0x3C;
  TL0 = 0xB0;
  EA = 1;
  ET0 = 1;
  TR0 = 1;                  //�򿪶�ʱ��0����ʱ50ms
  P1 = 0x0f;                //������̽�P1�ڣ�����ֵ
  for(i = 0; i < 20; i++){
    groundx[i] = 0xc007;
    Area[i]=0xc007;
  }

  groundx[19] = 0xffff;
  Area[19] = 0xffff;
  //Area[19] = 0xffff;
  lcd_initial(); //Һ������ʼ��
  bl=1;//�������IO���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
  //LCD_Clear(BLACK);		//��ɫ

  //print_Tetris(square_x+1,6,0x88c0,YELLOW);
  delay(500);
  trackSquare_Read(square_x,square_y);

  while(1)
  {
    if(Down_Flag){                                      //downflag��ʱ��T0��ʱ1.5s������λ�������½�
      trackSquare_Read(square_x,square_y);              //��ȡ׷���������ݵ�Tracksquare
      if(!showTrackSquare_Down(square_x, square_y, 0)){ //�ж��Ƿ񴥵ף�������д��׷�����ݣ��ɸú���������д�뱳������
        trackSquare_Write(square_x,square_y);           //û�д��ף�Tracksquare����д��׷������
        square_y++;                   //����һ��
      }
      else{
        for(i = 0; i < 20; i++)
        Area[i]=groundx[i];         //ͬ������
      }
      Down_Flag = 0;                //����½���־
    }
    if(!Left && Move_flag){         //�����жϣ�ͬʱ�ж��Ƿ������ƶ�������moveflag��T0����
      delay(20);
      if(!Left){                    //��ʱ�������ٴ��ж�
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 1)){
          trackSquare_Write(square_x,square_y);
          square_x--;
        }
      }
    }
    if(!Right && Move_flag){        //�����ж�
      delay(20);
      if(!Right){
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 2)){
          trackSquare_Write(square_x,square_y);
          square_x++;
        }
      }
    }
    if(!Rota && Move_flag){        //��ת�ж�
      delay(20);
      if(!Rota){
        trackSquare_Read(square_x,square_y);
        if(!showTrackSquare_Down(square_x, square_y, 3))
          trackSquare_Write(square_x,square_y);
      }
    }
  }

}

void Timer0() interrupt 1
{
  TH0 = 0x3C;
  TL0 = 0xB0;                //��ʱ50ms
  count++;
  if(count <= 22)           //ǰ22*50=1.1s���Խ��������ƶ�����
    Move_flag = 1;
  else
    Move_flag = 0;

  if(count >= 30){          //��ʱ1.5s
    count = 0;
    Down_Flag = 1;
  }
}
