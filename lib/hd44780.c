//HD44780 display service routines
#include "hd44780.h"
#include <ioavr.h>
#include <inavr.h>

void static PulseEN()
{
    LCD |= EN;
    __delay_cycles(4);
    LCD &= ~EN;
}

void LCDWriteCtrl(unsigned char d)
{
    LCD = d & 0xF0;
    PulseEN();
    LCD = (d << 4) & 0xF0;
    PulseEN();
}

void LCDWriteData(unsigned char d)
{
    LCD = d & 0xF0 | RS;
    PulseEN();
    LCD = (d << 4) & 0xF0 | RS;
    PulseEN();
}

unsigned char static ReadNibble()
{
    unsigned char retVal;
    LCD |= EN;
    __delay_cycles(3);
    retVal = LCDin;
    LCD &= ~EN;
    return retVal;
}

unsigned char LCDReadData()
{
    unsigned char retVal;
    LCDdir &= LCD_RD_DIR;
    LCD |= (0xF0 | WE | RS);
    retVal = (ReadNibble() & 0xF0) | (ReadNibble() >> 4);
    LCDdir |= LCD_WR_DIR;
    return retVal;
}

unsigned char LCDReadCtrl()
{
    unsigned char retVal;
    LCDdir &= LCD_RD_DIR;
    LCD = 0xF0 | WE;
    retVal = (ReadNibble() & 0xF0) | (ReadNibble() >> 4);
    LCDdir |= LCD_WR_DIR;
    return retVal;
}

void static WaitOnLCD()
{
    while (LCDReadCtrl() & 0x80)
        ;
}

void LCDInit(void)
{
    LCDdir = LCD_WR_DIR;
    //Software clear
    LCD = 0x30;
    //Implied settings
    // Pulse + 250us * 20 = 5ms
    PulseEN();
    __delay_cycles(36900UL);
    PulseEN(); // Pulse + 250us
    __delay_cycles(18000UL);
    PulseEN(); // Pulse + 250us
    __delay_cycles(18000UL);
    LCD = 0x20;
    PulseEN();
    __delay_cycles(10000);
    //Initial settings
    LCDWriteCtrl(0x28); //4-bit interface, 2 lines display, 7x5 dot matrix
    WaitOnLCD();
    LCDWriteCtrl(0x0C); //Display On, Cursor Off, Blinking Off
    WaitOnLCD();
    LCDWriteCtrl(0x01); //Clear display
    WaitOnLCD();
    LCDWriteCtrl(0x06); //Increment without shift
}

void LCDClear(void)
{
    WaitOnLCD();
    LCDWriteCtrl(0x01);
}

void LCDGoTo(unsigned char pos)
{
    WaitOnLCD();
    LCDWriteCtrl(DDRAM_ADDR | pos);
}

void LCDLoadUserCharP(const char __flash *data, unsigned char addr, unsigned char size)
{
    const char __flash *pd;
    pd = data;
    WaitOnLCD();
    LCDWriteCtrl(CGRAM_ADDR | addr);
    do
    {
        WaitOnLCD();
        LCDWriteData(*(pd++));
    } while (--size);
}

void LoadUserChar(char const *data, unsigned char addr)
{
    unsigned char cnt;
    WaitOnLCD();
    LCDWriteCtrl(CGRAM_ADDR | (addr << 3));
    cnt = 8;
    do
    {
        WaitOnLCD();
        LCDWriteData(*(data++));
    } while (--cnt);
}

void LCDPutChar(unsigned char c)
{
    WaitOnLCD();
    LCDWriteData(c);
}

void LCDPuts(const char *pc)
{
    unsigned char c;
    while (1)
    {
        if (!(c = *pc++))
            return;
        WaitOnLCD();
        LCDWriteData(c);
    }
}

void LCDPutsCode(char const __flash *pc)
{
    unsigned char c;
    while (1)
    {
        if (!(c = *pc++))
            return;
        WaitOnLCD();
        LCDWriteData(c);
    }
}

void LCDPrintDgtBl(unsigned char dgt)
{
    unsigned char c;
    c = dgt / 10;
    c = c ? c | 0x30 : ' ';
    LCDPutChar(c);
    c = dgt % 10;
    LCDPutChar(0x30 | c);
}

void LCDPrintDgt(unsigned char dgt)
{
    unsigned char c;
    c = dgt / 10;
    LCDPutChar(0x30 | c);
    c = dgt % 10;
    LCDPutChar(0x30 | c);
}

void LCDPrintBCD(unsigned char dgt)
{
    LCDPutChar(0x30 | (dgt >> 4));
    LCDPutChar(0x30 | (dgt & 0x0F));
}
