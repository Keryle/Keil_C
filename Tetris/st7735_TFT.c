#include <BasicSet.c>
#define TFT_Width       128
#define TFT_Height      160

unsigned int xdata Area[20];
unsigned int xdata *data pArea=Area;
unsigned char data trackSquare[6];
unsigned char xdata groundx[20];
unsigned char data tetris[5] = {0x00,0x0c,0x08,0x08,0};       //����λ��������
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

void trackSquare_Read(unsigned char x, unsigned char y){
  unsigned int xdata *p = Area;
  unsigned char i;
  p += y;           //���ݿ�ʼ��
  for(i = 0; i < 6; i++){
    trackSquare[i] = (*p >> (9-x)); //ȡ��6λ���ݣ����� 11-6-x = 5-xλ�������θ���λ
    p++;
  }

  for(i = 0; i < 6; i++){
    ground[i] = (groundx[y+i] >> (9-x)); //ȡ��6λ���ݣ����� 11-6-x = 5-xλ�������θ���λ
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
    trackSquare[i]=0;                              //����

  if(direction == 0)                               //�½�
    for(i = 0; i < 4; i++){
      row = tetris[i] << 1;
      if((ground[5-i] & row) > 0)
        return 1;
      trackSquare[5-i] = ground[5-i] | row;             //track���������˹�������,��������ƶ�����������
      }
  else
    if(direction == 1)                             //����
      for(i = 0; i < 4; i++){
        row = tetris[i] << 2;
        if((ground[4-i] & row) > 0)
          return 1;
        trackSquare[4-i] = ground[4-i] | row;
      }
    else
      if(direction == 2)                          //����
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
    pTemp++;                            //���õ��ı�λ����aa
  }
  if(aa[1] == 0x20)
  fillPoint(2,2,RED);
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
  lcd_initial(); //Һ������ʼ��
  bl=1;//�������IO���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
  //LCD_Clear(WHITE);		//��ɫ

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
