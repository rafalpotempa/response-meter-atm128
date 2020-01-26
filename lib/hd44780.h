#ifndef _HD44780_H
#define _HD44780_H

//LCD connection definition
#define LCD PORTB
#define LCDin PINB
#define LCDdir DDRB
#define RS 0x08
#define WE 0x04
#define EN 0x02
#define LCD_WR_DIR 0xF0 | RS | EN | WE
#define LCD_RD_DIR 0x00 | RS | EN | WE
#define LINE_0 0x00
#define LINE_1 0x40
#define DDRAM_ADDR 0x80
#define CGRAM_ADDR 0x40


void LCDWriteData(unsigned char d);
void LCDWriteCtrl(unsigned char d);
unsigned char LCDReadData(void);
unsigned char LCDReadCtrl(void);
void WaitOnLCD();
void LCDInit(void);
void LCDClear(void);
void LCDLoadUserCharP(const char __flash* data, unsigned char addr, unsigned char size);
void LoadUserChar(const char* data, unsigned char addr);
void LCDGoTo(unsigned char pos);
void LCDPutChar(unsigned char c);
void LCDPuts(const char *c);
void LCDPutsCode(const char __flash *c);
void LCDPrintDgtBl(unsigned char dgt);
void LCDPrintDgt(unsigned char dgt);
void LCDPrintBCD(unsigned char dgt);

#endif
