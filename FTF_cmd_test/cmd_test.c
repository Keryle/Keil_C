#include <8052.h>
unsigned short colstart = 0, rowstart = 0, wrap = 1;
#define TFT_Width       128
#define TFT_Height      160
#define ST7735_NOP      0X00
#define ST7735_SWRESET  0x01
// Color definitions
#define   ST7735_BLACK   0x0000
#define   ST7735_BLUE    0x001F
#define   ST7735_RED     0xF800
#define   ST7735_GREEN   0x07E0
#define   ST7735_CYAN    0x07FF
#define   ST7735_MAGENTA 0xF81F
#define   ST7735_YELLOW  0xFFE0
#define   ST7735_WHITE   0xFFFF

void SPI_Write(unsigned char SpiData){
    unsigned char ss;
    for(ss = 0x80; ss; ss >>= 1){
        if(SpiData & ss)    TFT_DATA = 1;
        else                TFT_DATA = 0;
        TFT_CLK = 1;
        TFT_CLK = 0;
    }
}

void write_command(unsigned char cmd_) {
    TFT_DC = 0;
    TFT_CS = 0;
    SPI_Write(cmd_);
    TFT_CS = 1;
}

void write_data(unsigned char data_) {
    TFT_DC = 0;
    TFT_CS = 1;
    SPI_Write(data_);
    TFT_CS = 1;
}

void main() {
    write_command(ST7735_SWRESET);
    while(1);
}
