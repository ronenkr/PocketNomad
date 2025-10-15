#pragma once
#include <Arduino.h>
#include <SPI.h>
#define LCD_WIDTH   160 //LCD width
#define LCD_HEIGHT  80 //LCD height

#define SPIFreq                        40000000
#define EXAMPLE_PIN_NUM_MISO           -1
#define EXAMPLE_PIN_NUM_MOSI           11
#define EXAMPLE_PIN_NUM_SCLK           10
#define EXAMPLE_PIN_NUM_LCD_CS         -1
#define EXAMPLE_PIN_NUM_LCD_DC         13
#define EXAMPLE_PIN_NUM_LCD_RST        14
#define EXAMPLE_PIN_NUM_BK_LIGHT       -1

#ifdef LCD_BACKLIGHT

#define Frequency       1000                    // PWM frequencyconst 
#define Resolution      10                      

#endif

#define VERTICAL   0
#define HORIZONTAL 1

#define Offset_X 0
#define Offset_Y 26


void LCD_SetCursor(uint16_t x1, uint16_t y1, uint16_t x2,uint16_t y2);

void LCD_Init(void);
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend);
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color);

void Backlight_Init(void);
void Set_Backlight(uint8_t Light);
