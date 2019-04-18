#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20],groundx[20];
unsigned int xdata *data pArea=Area;
unsigned char data trackSquare[6],ground[6];
unsigned char data tetris[5] = {0x00,0x0c,0x08,0x08,0};       //����λ��������
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
//��ʾ����˹����
void print_Tetris(unsigned char x, unsigned char y, unsigned int cube, unsigned int color){
  unsigned char row, i,bit_row,j;
  x += 1;                               //����ƽ������
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
    11 |  x  x  x  x  x  x  x  x  x  x  x  | 111
*/
void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //���ݿ�ʼ��
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (14-6+1-x)) & 0; //ȡ��6λ����
    p++;
  }
  p = groundx;
  p += y;           //���ݿ�ʼ��
  for(i = 0; i < 6; i++){
    ground[i] = (*p >> (14-6+1-x)); //ȡ��6λ����
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
  x += 1;                                          //��Ϸ��������ƽ��2��
  for(i = 0; i < 6; i++)
    temptrack[i] = *pTrack++;
  pTrack = &temptrack[5];
  for(i = 0; i <= 3; i++ ){
    pTrack--;
    row = tetris[i] << 1;
    *pTrack = (*pTrack) | row;             //temptrack���������˹�������
  }
  pTrack = &ground[5];                  //ָ���5��
  j = 5;
  for(i = 0; i <= 3; i++){
    row = tetris[i] << 1;
  //  if((*pTrack & row) > 0)
  //  return  1;
    trackSquare[j] = (*pTrack) | row;             //track���������˹�������,��������ƶ�����������
    pTrack--;
    j--;
  }
  pTrack = &temptrack[0];
  for(i = 0; i < 6; i++){
    aa[i] = *pTrack ^ trackSquare[i];
    pTrack++;                             //���õ��ı�λ����aa
  }
  for(i = 0; i < 6; i++){                 //����ɨ��
    if(aa[i]){                           //���ֵΪ�棬�ı���ɫ
      row = 0x10;
      for(j = 0; j < 6; j++){             //����ɨ��
        if(aa[i] & row ){                 //��λ�ж�
          if(temptrack[i] & row)        //ԭ����ֵΪ�棬�ĳɱ���ɫ����ɫ�������������ɫ
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


//��Ϸ����12X20����һ��16λ���ݱ���һ�У���4λ��Ч����20�У�20������
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
  Area[18] = 0xffff;//��Ϸ�±߽�
  Area[17] = 0xffff;
  lcd_initial(); //Һ������ʼ��
  bl=1;//�������IO���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
  LCD_Clear(BLACK);		//��ɫ

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
