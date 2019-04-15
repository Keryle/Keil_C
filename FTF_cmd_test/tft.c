// Interfacing ST7735R SPI TFT with PIC16F887 mikroC code
// External HS oscillator used @ 20 MHz
// Configuration words: CONFIG1 = 0x2CD2
//                      CONFIG2 = 0x0700


// SPI TFT module connections
#define TFT_SPI_HARDWARE                  // Hardware SPI module is used
sbit TFT_CS at RD0_bit;                   // TFT CS pin is connected to RD0 pin
sbit TFT_DC at RD1_bit;                   // TFT DC pin is connected to RD1 pin
sbit TFT_CS_Direction at TRISD0_bit;
sbit TFT_DC_Direction at TRISD1_bit;
// End SPI TFT module connections

#include <ST7735_TFT.c>                   // Include ST7735 TFT driver source file

unsigned short k = 0;
char *txt = "Hello World!";
char *txt1 = "  ";

void testlines(unsigned int color) {
  unsigned short x, y;
  fillScreen(ST7735_BLACK);
  for (x=0; x < TFT_Width; x+=6) {
    drawLine(0, 0, x, TFT_Height-1, color);
  }
  for (y=0; y < TFT_Height; y+=6) {
    drawLine(0, 0, TFT_Width-1, y, color);
  }

  fillScreen(ST7735_BLACK);
  for (x=0; x < TFT_Width; x+=6) {
    drawLine(TFT_Width-1, 0, x, TFT_Height-1, color);
  }
  for (y=0; y < TFT_Height; y+=6) {
    drawLine(TFT_Width-1, 0, 0, y, color);
  }

  fillScreen(ST7735_BLACK);
  for (x=0; x < TFT_Width; x+=6) {
    drawLine(0, TFT_Height-1, x, 0, color);
  }
  for (y=0; y < TFT_Height; y+=6) {
    drawLine(0, TFT_Height-1, TFT_Width-1, y, color);
  }

  fillScreen(ST7735_BLACK);
  for (x=0; x < TFT_Width; x+=6) {
    drawLine(TFT_Width-1, TFT_Height-1, x, 0, color);
  }
  for (y=0; y < TFT_Height; y+=6) {
    drawLine(TFT_Width-1, TFT_Height-1, 0, y, color);
  }
}
void testfastlines(unsigned int color1, unsigned int color2) {
  unsigned int x, y;
  fillScreen(ST7735_BLACK);
  for (y = 0; y < TFT_Height; y += 5) {
    drawFastHLine(0, y, TFT_Width, color1);
  }
  for (x = 0; x < TFT_Width; x += 5) {
    drawFastVLine(x, 0, TFT_Height, color2);
  }
}
void testdrawrects(unsigned int color) {
  unsigned int x;
  fillScreen(ST7735_BLACK);
  for (x = 0; x < TFT_Width; x+=6) {
    drawRect(TFT_Width/2 -x/2, TFT_Height/2 -x/2 , x, x, color);
  }
}
void testfillrects(unsigned int color1, unsigned int color2) {
  unsigned int x;
  fillScreen(ST7735_BLACK);
  for (x = TFT_Width - 1; x > 6; x -= 6) {
    fillRect(TFT_Width/2 -x/2, TFT_Height/2 -x/2 , x, x, color1);
    drawRect(TFT_Width/2 -x/2, TFT_Height/2 -x/2 , x, x, color2);
  }
}
void testfillcircles(unsigned short radius, unsigned int color) {
  unsigned int x, y;
  for (x = radius; x < TFT_Width; x += radius * 2) {
    for (y = radius; y < TFT_Height; y += radius * 2) {
      fillCircle(x, y, radius, color);
    }
  }
}
void testdrawcircles(unsigned short radius, unsigned int color) {
  unsigned int x, y;
  for (x = 0; x < TFT_Width + radius; x += radius * 2) {
    for (y = 0; y < TFT_Height + radius; y += radius * 2) {
      drawCircle(x, y, radius, color);
    }
  }
}
void testroundrects() {
  unsigned short i, t;
  unsigned int color = 100;
  fillScreen(ST7735_BLACK);
  for(t = 0 ; t <= 4; t += 1) {
    unsigned short x = 0, y = 0, w = TFT_Width - 2, h = TFT_Height - 2;
    for(i = 0 ; i <= 16; i++) {
      drawRoundRect(x, y, w, h, 5, color);
      x += 2;
      y += 3;
      w -= 4;
      h -= 6;
      color += 1100;
    }
    color += 100;
  }
}
void testtriangles() {
  unsigned short t, w, x, y, z;
  unsigned int color = 0xF800;
  fillScreen(ST7735_BLACK);
  w = TFT_Width/2, x = TFT_Height - 1, y = 0, z = TFT_Width;
  for(t = 0 ; t <= 15; t++) {
    drawTriangle(w, y, y, x, z, x, color);
    x -= 4;
    y += 4;
    z -= 4;
    color += 100;
  }
}
void mediabuttons() {
  // play
  fillScreen(ST7735_BLACK);
  fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
  fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
  delay_ms(500);
  // pause
  fillRoundRect(25, 90, 78, 60, 8, ST7735_WHITE);
  fillRoundRect(39, 98, 20, 45, 5, ST7735_GREEN);
  fillRoundRect(69, 98, 20, 45, 5, ST7735_GREEN);
  delay_ms(500);
  // play color
  fillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
  delay_ms(50);
  // pause color
  fillRoundRect(39, 98, 20, 45, 5, ST7735_RED);
  fillRoundRect(69, 98, 20, 45, 5, ST7735_RED);
  // play color
  fillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
}
void tst(){
  drawtext(0, 5, "1.8 Inch ST7735 TFT  display test example", ST7735_WHITE, ST7735_BLACK, 1);
}
void main() {
  ST7735_TFT_Init();                                // Initialize the TFT module
  fillScreen(ST7735_BLACK);
  tst();

  setTextWrap(0);                                   // Turn off text wrap

  // Draw text "Hello World!"
  drawtext(0, 30, txt, ST7735_RED, ST7735_BLACK, 1);
  drawtext(0, 47, txt, ST7735_YELLOW, ST7735_BLACK, 2);
  drawtext(0, 80, txt, ST7735_MAGENTA, ST7735_BLACK, 3);
  drawtext(0, 120, txt, ST7735_CYAN, ST7735_BLACK, 4);

  delay_ms(5000);                                   // Wait 5 seconds
  fillScreen(ST7735_BLACK);
  drawFastHLine(0, 53, TFT_Width,  ST7735_WHITE);
  drawFastHLine(0, 106, TFT_Width, ST7735_WHITE);

  // Display number
  while(k++ < 20){
    txt1[0] = (k / 10)%10 + 48;
    txt1[1] = (k % 10) + 48;
    drawtext(59, 25, txt1,  ST7735_GREEN, ST7735_BLACK, 1);
    drawtext(54, 75, txt1,  ST7735_BLUE,  ST7735_BLACK, 2);
    drawtext(49, 125, txt1, ST7735_RED,  ST7735_BLACK, 3);
    delay_ms(500);
  }

  testlines(ST7735_YELLOW);
  delay_ms(1000);

  testfastlines(ST7735_RED, ST7735_BLUE);
  delay_ms(1000);

  testdrawrects(ST7735_GREEN);
  delay_ms(1000);

  testfillrects(ST7735_YELLOW, ST7735_MAGENTA);
  delay_ms(1000);

  fillScreen(ST7735_BLACK);
  testfillcircles(10, ST7735_BLUE);
  testdrawcircles(10, ST7735_WHITE);
  delay_ms(1000);

  testroundrects();
  delay_ms(1000);

  testtriangles();
  delay_ms(1000);

  mediabuttons();
  delay_ms(1000);

  while(1) {
    invertDisplay(1);
    delay_ms(500);
    invertDisplay(0);
    delay_ms(500);
  }
}
// End of code
