#include "Display_ST7789.h"
   
SPIClass LCDspi(FSPI);
#define SPI_WRITE(_dat)         LCDspi.transfer(_dat)
#define SPI_WRITE_Word(_dat)    LCDspi.transfer16(_dat)
void SPI_Init()
{
  LCDspi.begin(EXAMPLE_PIN_NUM_SCLK,EXAMPLE_PIN_NUM_MISO,EXAMPLE_PIN_NUM_MOSI); 
}

void LCD_WriteCommand(uint8_t Cmd)  
{ 
  LCDspi.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, LOW); 
  SPI_WRITE(Cmd);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  LCDspi.endTransaction();
}
void LCD_WriteData(uint8_t Data) 
{ 
  LCDspi.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH);  
  SPI_WRITE(Data);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  LCDspi.endTransaction();
}    
void LCD_WriteData_Word(uint16_t Data)
{
  LCDspi.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH); 
  SPI_WRITE_Word(Data);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  LCDspi.endTransaction();
}   
void LCD_WriteData_nbyte(uint8_t* SetData,uint8_t* ReadData,uint32_t Size) 
{ 
  LCDspi.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH);  
  LCDspi.transferBytes(SetData, ReadData, Size);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  LCDspi.endTransaction();
} 

void LCD_Reset(void)
{
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);       
  delay(50);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_RST, LOW); 
  delay(50);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_RST, HIGH); 
  delay(50);
}
void LCD_Init(void)
{
  pinMode(EXAMPLE_PIN_NUM_LCD_CS, OUTPUT);
  pinMode(EXAMPLE_PIN_NUM_LCD_DC, OUTPUT);
  pinMode(EXAMPLE_PIN_NUM_LCD_RST, OUTPUT); 
  Backlight_Init();
  SPI_Init();

  LCD_Reset();
  //************* Start Initial Sequence for ST7735 **********// 
  LCD_WriteCommand(0x01); // Software reset
  delay(150);
  
  LCD_WriteCommand(0x11); // Sleep out
  delay(500);
  
  LCD_WriteCommand(0xB1); // Frame rate control - normal mode
  LCD_WriteData(0x01);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x2D);
  
  LCD_WriteCommand(0xB2); // Frame rate control - idle mode
  LCD_WriteData(0x01);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x2D);
  
  LCD_WriteCommand(0xB3); // Frame rate control - partial mode
  LCD_WriteData(0x01);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x2D);
  LCD_WriteData(0x01);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x2D);
  
  LCD_WriteCommand(0xB4); // Display inversion control
  LCD_WriteData(0x07);
  
  LCD_WriteCommand(0xC0); // Power control 1
  LCD_WriteData(0xA2);
  LCD_WriteData(0x02);
  LCD_WriteData(0x84);
  
  LCD_WriteCommand(0xC1); // Power control 2
  LCD_WriteData(0xC5);
  
  LCD_WriteCommand(0xC2); // Power control 3
  LCD_WriteData(0x0A);
  LCD_WriteData(0x00);
  
  LCD_WriteCommand(0xC3); // Power control 4
  LCD_WriteData(0x8A);
  LCD_WriteData(0x2A);
  
  LCD_WriteCommand(0xC4); // Power control 5
  LCD_WriteData(0x8A);
  LCD_WriteData(0xEE);
  
  LCD_WriteCommand(0xC5); // VCOM control 1
  LCD_WriteData(0x0E);
  
  LCD_WriteCommand(0x36); // Memory data access control
  if (HORIZONTAL)
      LCD_WriteData(0xA0);
  else
      LCD_WriteData(0xC0);

  LCD_WriteCommand(0x3A); // Pixel format
  LCD_WriteData(0x05);
  
  LCD_WriteCommand(0x2A); // Column address set
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x7F); // 127
  
  LCD_WriteCommand(0x2B); // Row address set
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x9F); // 159
  
  LCD_WriteCommand(0xE0); // Positive gamma correction
  LCD_WriteData(0x02);
  LCD_WriteData(0x1C);
  LCD_WriteData(0x07);
  LCD_WriteData(0x12);
  LCD_WriteData(0x37);
  LCD_WriteData(0x32);
  LCD_WriteData(0x29);
  LCD_WriteData(0x2D);
  LCD_WriteData(0x29);
  LCD_WriteData(0x25);
  LCD_WriteData(0x2B);
  LCD_WriteData(0x39);
  LCD_WriteData(0x00);
  LCD_WriteData(0x01);
  LCD_WriteData(0x03);
  LCD_WriteData(0x10);

  LCD_WriteCommand(0xE1); // Negative gamma correction
  LCD_WriteData(0x03);
  LCD_WriteData(0x1D);
  LCD_WriteData(0x07);
  LCD_WriteData(0x06);
  LCD_WriteData(0x2E);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x29);
  LCD_WriteData(0x2D);
  LCD_WriteData(0x2E);
  LCD_WriteData(0x2E);
  LCD_WriteData(0x37);
  LCD_WriteData(0x3F);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x02);
  LCD_WriteData(0x10);

  LCD_WriteCommand(0x13); // Normal display mode on
  delay(10);
  
  LCD_WriteCommand(0x21); // Display inversion on

  LCD_WriteCommand(0x29); // Display on
  delay(100);
}
/******************************************************************************
function: Set the cursor position
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinatesen
******************************************************************************/
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend)
{ 
  if (HORIZONTAL) {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + Offset_X);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + Offset_X);
    
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + Offset_Y);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + Offset_Y);
  }
  else {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + Offset_Y);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + Offset_Y);
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + Offset_X);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + Offset_X);
  }
  LCD_WriteCommand(0x2C);
}
/******************************************************************************
function: Refresh the image in an area
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinates
    color :   Set the color
******************************************************************************/
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color)
{             
  uint16_t Show_Width = Xend - Xstart + 1;
  uint16_t Show_Height = Yend - Ystart + 1;
  uint32_t numBytes = Show_Width * Show_Height * sizeof(uint16_t);
  uint8_t Read_D[numBytes];
  LCD_SetCursor(Xstart, Ystart, Xend, Yend);
  LCD_WriteData_nbyte((uint8_t*)color, Read_D, numBytes);        
}
// backlight
void Backlight_Init(void)
{
#ifdef LCD_BACKLIGHT  
  ledcAttach(EXAMPLE_PIN_NUM_BK_LIGHT, Frequency, Resolution);    
  ledcWrite(EXAMPLE_PIN_NUM_BK_LIGHT, 100);                      
#endif
}

void Set_Backlight(uint8_t Light)                        //
{
#ifdef LCD_BACKLIGHT
  if(Light > 100 || Light < 0)
    printf("Set Backlight parameters in the range of 0 to 100 \r\n");
  else{
    uint32_t Backlight = Light*10;
    ledcWrite(EXAMPLE_PIN_NUM_BK_LIGHT, Backlight);
  }
#endif
}