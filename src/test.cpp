#include "FONTK52.h"
#include "font_5x7.h"


#define DI PF1
#define CS_1 0xE0 
#define CS_2 0xD0
#define CS_3 0xC0
#define E PF3
#define RW PF2

#define DI_0 PORTF &=~ 0x02
#define DI_1 PORTF |= 0x02
#define CS1_1 PORTF &=~ 0x10
#define CS1_0 PORTF |= 0x10
#define CS2_1 PORTF &=~ 0x20
#define CS2_0 PORTF |= 0x20
#define RW_0 PORTF &=~ 0x04
#define RW_1 PORTF |= 0x04
#define E_0 PORTF &=~ 0X08
#define E_1 PORTF |= 0X08
#define CS3_0 PORTF |= 0x30
#define CS3_1 PORTF &=~ 0x30

#define DISPOFF 0xAE 
#define DISPON 0xAF 
#define LCD_SET_LINE 0xC0
#define SET_X 0xB8
#define SET_Y 0x00
#define STATIC_OFF 0xA4 
#define STATIC_ON 0xA5
#define DUTY_16 0xA8
#define DUTY_32 0xA9
#define SET_MODIFY 0xE0
#define CLR_MODIFY 0xEE 
#define RESET 0xE2

#define _2Khz 3999

unsigned char char_x, char_y; 
unsigned char cursor_flag, cur_X, cur_Y; 
unsigned char NT[32];

unsigned char picture[]= {
	0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0x78, 0x78, 0x78, 0x78, 0x78, 0x70, 0x70, 0x70, 0x70, 0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x70, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0xF0, 0x10, 
	0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x30, 0x60, 0xC0, 0x80, 0x80, 0x00, 0x80, 0x80, 0xC0, 0x60, 0x70, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x9C, 0x9C, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8C, 0x0E, 0xCE, 0xE6, 0xE2, 0xC2, 0x02, 0x82, 0x03, 0xE1, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x70, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x04, 0x04, 0x04, 0x0C, 0x0C, 0x1C, 0x1C, 0x3C, 0x64, 0x66, 0x67, 0x43, 0x41, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x00, 0x00, 0x40, 0x40, 0x60, 0x30, 0x18, 0x0D, 0x0F, 0x07, 0x0F, 0x0D, 0x18, 0x38, 0x30, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x10, 0x18, 0x1C, 0x1E, 0x1F, 0x1F, 0x17, 0x13, 0x11, 0x30, 0x48, 0x48, 0x30, 0x00, 0x01, 0x82, 0x85, 0x85, 0x82, 0x41, 0x40, 0x3E, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x20, 0x30, 0xF0, 0xE0, 0x00, 
	0x20, 0xF8, 0xF8, 0x20, 0x00, 0x00, 0xF0, 0xF0, 0x30, 0x30, 0x00, 0xE0, 0xF0, 0x90, 0x90, 0xF0, 0xE0, 0x00, 0x10, 0x70, 0xF0, 0xE0, 0xF0, 0x30, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x10, 0x10, 0x70, 0x60, 0x00, 0xE0, 0xF0, 0x10, 0x10, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xC0, 0xF0, 0x30, 0x10, 0x00, 0xF0, 0xF0, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x1F, 0x1F, 0x3F, 0x3F, 0x3C, 0x38, 0x18, 0x18, 0x18, 0x0C, 0x0C, 0x0C, 0x04, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x00, 
	0x00, 0x0F, 0x1F, 0x10, 0x10, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1C, 0x1C, 0x00, 0x10, 0x1C, 0x1F, 0x07, 0x1E, 0x18, 0x00, 0x18, 0x18, 0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1C, 0x0C, 0x00, 0x0F, 0x1F, 0x10, 0x10, 0x1F, 0x0F, 0x00, 0x00, 0x18, 0x18, 0x00, 0x1F, 0x1F, 0x03, 0x0F, 0x1E, 0x10, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
unsigned char devicemart[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0xE0, 0xF0, 0xF8, 0x78, 0x38, 0x3C, 0x3C, 0x3C, 0xFD, 0xFD, 0xFD, 0xFD, 0x3C, 0x3C, 0x78, 0xF8, 0xF0, 0xF0, 0xE0, 0x80, 0x80, 0xC0, 0xF0, 0xF0, 0xF8, 0x78, 0x3C, 0x3C, 0xFC, 0xFC, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1F, 0x7F, 0xFF, 0xF9, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x06, 0x06, 0x86, 0xFC, 0x78, 0x00, 0xFC, 0xFC, 0xB6, 0x36, 0x36, 0x36, 0x00, 0x06, 0x1E, 0x7C, 0xE0, 0xC0, 0xF0, 0x3C, 
	0x0E, 0x00, 0x00, 0xFE, 0xFC, 0x00, 0xF8, 0xFC, 0x86, 0x06, 0x06, 0x06, 0x00, 0xF8, 0xFC, 0xB6, 0x36, 0x36, 0x36, 0x22, 0x00, 0x00, 0xFE, 0xFE, 0x1E, 0xF8, 0xC0, 0xE0, 0x78, 0x1E, 0xFE, 0xFC, 0x00, 0xC0, 0xF8, 0x7E, 0x4E, 0x7C, 0xF0, 0x80, 0x00, 0xFE, 0xFE, 0x26, 0x26, 0x66, 0xFE, 0xBC, 0x08, 0x06, 0x06, 0x06, 0xFE, 0xFE, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};
void SetPage(unsigned char , unsigned char);
void SetAddr(unsigned char , unsigned char);
void SetXY(unsigned char , unsigned char , unsigned char);


void delay_us(unsigned char time_us)

{
	register unsigned char k;
	for(k=0;k<time_us;k++)
	{
		asm volatile("PUSH R0");
		asm volatile("POP R0");
		asm volatile("PUSH R0");
		asm volatile("POP R0");
		asm volatile("PUSH R0");
		asm volatile("POP R0");
	}
}

void delay_ms(unsigned char time_ms)
{
	register unsigned char k;
	for(k=0;k<time_ms;k++) {
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);
	}
}

void Clk(unsigned int n)
{
	DDRB |= 0x20;
	TCCR1A=(1< TCCR1B=0x09);

	OCR1AH=n>>8; 
	OCR1AL=n&0xff;

}

void MCU_init(void)

{ 
	Clk(_2Khz);

	DDRC = 0xFF;
	DDRF = 0xFF;
	PORTC = 0x00;
	PORTF = 0xF0; 
}

void Command(unsigned char signal, unsigned char command)
{ 
	PORTF = signal | CS_3;
	// PORTF = signal;
	DI_0;
	Delay_us(2);
	RW_0;
	Delay_us(2);
	PORTC = command;
	Delay_us(2);
	E_1;
	Delay_us(2);
	E_0;
	PORTF = 0x00;

}

void Data(unsigned char signal, unsigned char character)
{ 
	PORTF = signal | CS_3;
	// PORTF = signal;
	DI_1;
	Delay_us(2);
	RW_0;
	Delay_us(2);
	PORTC = character;
	Delay_us(2);
	E_1;
	Delay_us(2);
	E_0;
	PORTF = 0x00;

}

void Clr()
{
	int page, colm;

	for(page=0;page<4;page++)
	{
		for(colm=0;colm<122;colm++)
		{
			SetPage(colm/61+1,page);
			SetAddr(colm/61+1,colm);
			Data(colm/61+1,0x00);
		}
	}

}
/*
void wClr(void)
{

unsigned char i, j, x; 
Command(CS_3, DISPON); 
Command(CS_3, 0xC0);

x=SET_X;
for(i = 0; i <= 3; i++)
{
Command(CS_3, x); 
Command(CS_3, SET_Y); 
for(j = 0; j <= 60; j++)
{
Data(CS_3, 0x00); 
}
x++;
}
}

*/
void LCD_init()
{

	Command( CS_3, RESET );
	Command( CS_3, LCD_SET_LINE );
	Command( CS_3, SET_X ); 
	Command( CS_3, SET_Y ); 
	Command( CS_3, DISPON );

	Clr();
}
void xy( unsigned char x, unsigned char y ) 
{
	char_x = x; 
	char_y = y; 
}


void xy_row(unsigned char row,unsigned char x,unsigned char y)
{ 
	xy(x,y);

	if(row == 0) 
		Command(CS_3,0xB8 + x*2);
	else 
		Command(CS_3,0xB8 + x*2 +1);

	if(char_y <= 7) 
		Command(CS_1,0x00 + y*8-3);
	else 
		Command(CS_2,0x00 + (y-8)*8);
}

void E_font(unsigned char EN) 
{ 
	unsigned char x, y, i, signal;

	x = char_x;
	y = char_y;

	xy_row(0,x,y);

	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;

	for(i = 0; i <= 7; i++)
		Data(signal,pgm_read_byte(&English[EN][i]));

	xy_row(1,x,y);

	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;

	for(i = 8; i <= 15; i++)
	{ 
		if((cursor_flag == 1) && (char_x == cur_X) && (char_y == cur_Y))
			Data(signal,pgm_read_byte(&English[EN][i]) | 0x80);
		else
			Data(signal,pgm_read_byte(&English[EN][i]));
	}

	char_y++;
	if(char_y==16)
	{
		char_y = 0;
		char_x++;
	}
}

void K_font(unsigned int KO) 
{ 
	unsigned char x, y, i, signal;
	unsigned char cho_5bit, joong_5bit, jong_5bit;
	unsigned char cho_bul, joong_bul, jong_bul, jong_flag;
	unsigned int character;

	if(char_y==15)
		E_font(0x20);

	cho_5bit = table_initial[(KO >> 10) & 0x001F]; 
	joong_5bit = table_medial[(KO >> 5) & 0x001F];
	jong_5bit = table_final[KO & 0x001F];

	if(jong_5bit == 0) 
	{
		jong_flag = 0;
		cho_bul = bul_initial1[joong_5bit];

		if((cho_5bit == 1) || (cho_5bit == 16)) 
			joong_bul = 0;
		else 
			joong_bul = 1;
	}
	else 
	{ 
		jong_flag = 1;
		cho_bul = bul_initial2[joong_5bit];
		if((cho_5bit == 1) || (cho_5bit == 16))
			joong_bul = 2;
		else 
			joong_bul = 3;
		jong_bul = bul_final[joong_5bit];
	}

	character = cho_bul*20 + cho_5bit; 

	for(i = 0; i <= 31; i++)
		NT[i] = pgm_read_byte(&Korean[character][i]);
	character = 8*20 + joong_bul*22 + joong_5bit;

	for(i = 0; i <= 31; i++)
		NT[i] |= pgm_read_byte(&Korean[character][i]);

	if(jong_flag == 1) 
	{ 
		character = 8*20 + 4*22 +jong_bul*28 + jong_5bit;
		for(i = 0; i <= 31; i++)
			NT[i] |= pgm_read_byte(&Korean[character][i]);
	}

	x = char_x;
	y = char_y;
	xy_row(0,x,y); 
	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;
	for(i = 0; i <= 7; i++)
		Data(signal,NT[i]);
	xy_row(1,x,y); 
	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;
	for(i = 16; i <= 23; i++)
	{ 
		if((cursor_flag == 1) && (char_x == cur_X) && (char_y == cur_Y))
			Data(signal,NT[i] | 0x80);
		else
			Data(signal,NT[i]);
	}

	char_y++;

	y = char_y;
	xy_row(0,x,y); 
	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;
	for(i = 8; i <= 15; i++)
		Data(signal,NT[i]);

	xy_row(1,x,y); 
	if(char_y <= 7) 
		signal = CS_1;
	else 
		signal = CS_2;
	for(i = 24; i <= 31; i++)
	{ 
		if((cursor_flag == 1) && (char_x == cur_X) && (char_y == (cur_Y+1)))
			Data(signal,NT[i] | 0x80);
		else
			Data(signal,NT[i]);
	}

	char_y++;
	if(char_y==16)
	{
		char_y = 0;
		char_x++;
	}
}
unsigned int Change(unsigned int c)
{
	unsigned char high,low;
	unsigned int index;
	unsigned int R_data;

	high = (c>>8)&0xff;
	low = c &0xff;

	if ((c >= 0xb0a1)&&(c <= 0xc8fe))
	{
		index = (high - 0xb0) * 94 + low - 0xa1;
		R_data = pgm_read_byte(&KSTable[index][0])*256;
		R_data |= pgm_read_byte(&KSTable[index][1]);
		return R_data;
	}
	return 0;
}

void String(unsigned char x, unsigned char y, char *string) 
{ 
	unsigned char char1;
	unsigned int char2;

	xy(x,y);

	while(*string != '\0')
	{ 
		char1 = *string;
		string++;
		if(((char1 >> 7) & 0x01) == 0)
			E_font(char1); 
		else
		{
			char2 = *string;
			char2 = 256*char1 + (char2 & 0xFF);
			string++;
			char2 = Change(char2);
			K_font(char2); 
		}
	}
}

void SetPage(unsigned char module, unsigned char page)
{
	//wCommand(module,SET_X|page);
	Command(CS_3 | (0x04 >> module) ,SET_X|page);
}
void SetAddr(unsigned char module, unsigned char addr)
{
	//wCommand(module,SET_Y|addr);
	Command(CS_3 | (0x04 >> module) ,SET_Y|addr);
}
void SetXY(unsigned char module, unsigned char x, unsigned char y)
{
	Command(CS_3 | (0x04 >> module) ,(SET_X | x));
	Command(CS_3 | (0x04 >> module) ,(SET_Y | y));
}
#define MAX_COL (128+64)
void Graphic(unsigned char *img,unsigned char mode)
{
	int page, colm;
	for(page=0;page<4;page++)
	{
		for(colm=0;colm<61;colm++)
		{
			//SetPage(CS_3,page);
			//SetAddr(CS_3,colm);
			SetXY(CS_3, page, colm);
			if (mode==0)
			{
				Data(CS_1,img[page*MAX_COL+colm]);
				Data(CS_2,img[page*MAX_COL+colm+61]);
			}
			else
			{
				Data(CS_1,~img[page*MAX_COL+colm]);
				Data(CS_2,~img[page*MAX_COL+colm+61]);
			}

		}
	}
}
void Graphic2(unsigned char *img)
{
	int page, colm, num;
	for(num=60;num<(122+61);num++)
	{
		for(page=0;page<4;page++)
		{
			for(colm=0;colm<61;colm++)
			{
				SetPage(CS_3,page);
				SetAddr(CS_3,colm);
				int seq = num+colm;
				if (seq >=60 )
					seq -= 60;

				if (seq >=121)
				{
					Data(CS_1,0x00);
					Data(CS_2,0x00);
				}
				else
				{
					Data(CS_1,img[page*MAX_COL+seq]);
					Data(CS_2,img[page*MAX_COL+seq+61]);
				}
			}
		}
		_delay_ms(50);
	}
}

void E_xy(unsigned char x, unsigned char y) 
{ 
	char_x = x;
	char_y = y;

	Command(CS_3, 0xB8+char_x);

	if(char_y <= 9) 
		Command(CS_1, 0x00+char_y*6+1);
	else 
		Command(CS_2, 0x00+(char_y-10)*6);
}

void E_fill(unsigned char character) 
{ 
	unsigned char i, signal;

	E_xy(char_x,char_y);

	if(char_y <= 9) 
		signal = CS_1;
	else 
		signal = CS_2;

	for(i = 0; i <= 4; i++)
	{ 
		if((cursor_flag == 1) && (char_x == char_x) && (char_y == char_y))
			Data(signal,(font[character - 0x20][i]) | 0x80);

		else
			Data(signal,font[character - 0x20][i]);
	}

	Data(signal,0x00);

	char_y++; 
	if(char_y == 20)
	{ 
		char_y = 0;
		char_x++;
	}
}

void E_String(unsigned char x, unsigned char y, char *str) 
{ 
	char_x = x;
	char_y = y;

	while(*str != '\0')
	{
		E_fill(*str); 
		str++;
	}
}

int main(void)
{

	MCU_init(); 
	LCD_init();



	while(1)
	{

		Graphic(devicemart,0);
		delay_ms(2000);
		Clr();


		/* Clr(); 
		Graphic(picture,0);
		_delay_ms(200);
		Graphic2(picture);
		Clr(); 
		*/
		Graphic(picture,0);
		delay_ms(1500);
		for(int i=0;i<5;i++)
		{
			// Clr(); 
			Graphic(picture,0);
			delay_ms(500);
			Clr(); 
			Graphic(picture,1);
			delay_ms(500);
			Clr();
		}

		String(0,0," ���� �����Ʈ! ");
		String(1,0," �׷��� LCD ");
		delay_ms(2000);
		Clr();



		String(0,0," No.1 ���θ� ");
		String(1,0," ����̽� ��Ʈ ");
		delay_ms(2000);
		Clr();

		String(0,0," (��) ��Ƽ���� "); 
		String(1,0," NTREX LAB ");
		delay_ms(2000);
		Clr();

		E_String(0,0,"==== Contact Us ==== ");
		E_String(1,0," Tel:070-7019-8887");
		E_String(2,0,"www.DeviceMart.co.kr");
		E_String(3,0," www.Ntrexlab.co.kr ");

		delay_ms(3000);



		Clr(); 
	} 
}