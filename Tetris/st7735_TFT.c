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

//��Ϸ����12X20����һ��16λ���ݱ���һ�У���4λ��Ч����20�У�20������
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
  row_4 <<= 4;             //����

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
  lcd_initial(); //Һ������ʼ��
  bl=1;//�������IO���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
  LCD_Clear(BLACK);		//��ɫ
  fillPoint(0,3,GREEN);
  print_Tetris(8,8,0x88c0,YELLOW);


  while(1)
  {
	delay(500);
   }

}
