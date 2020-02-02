#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h> 
#include <util/delay.h>
#include "hd44780.c"

int sequence[4] = {1, 4, 5, 7};
unsigned int totalTime = 0;
int millisecond = 2000; // 16 000 000 Hz / 8 / 1000 Hz = 2 000
char buffer[16];
int inProgress = 0;

ISR (TIMER1_COMPA_vect) {
	if (inProgress)
		totalTime++;
}

void initTimer() {
	TCCR1B |= (1<<CS11)|(1<<WGM12);
	OCR1A = millisecond;
	TIMSK |= (1<<OCIE1A);
	sei();
}

void initDevices() {
	DDRC = 0x00;  // keyboard
	PORTC = 0xff;
	DDRB |= 0xff; // led
	PORTB = 0xff;
}

void printResult() {
	LCD_Clear();
	LCD_GoTo(0,0);
	float average = totalTime/4.0;
	sprintf(buffer, "Total: %2d.%d s", (int)totalTime/1000, (int)totalTime%1000);
	LCD_WriteText(buffer);
	LCD_GoTo(0,1);
	sprintf(buffer, "Mean:   %d.%d s", (int)average/1000, (int)average%1000);
	LCD_WriteText(&buffer);
}

void testing() {
	totalTime = 0;
	
	
	for (int i = 0; i < 4; i++)
	{
		inProgress = 1;
		PORTB = ~(1 << sequence[i]);
		while(PINC != PORTB); // waiting for correct button
		inProgress = 0;
	}
}

int main(void)
{	
	initDevices();
	
	LCD_Initalize();
	initTimer();
	    
	while(1){
		LCD_Clear();
		LCD_GoTo(1,0);
		LCD_WriteText("Response meter");
		LCD_GoTo(1,1);
		LCD_WriteText("Press button...");
		while(PINC == 0xff){};
		LCD_GoTo(1,1);
		LCD_WriteText("  Testing...   ");
		testing();
		printResult();
		break;
    }
}


