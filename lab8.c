// Lab8
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <math.h>
#include <wiringPiI2C.h>
#include <time.h>

#define L2C_ADDR 0x3f
#define LCD_CHR 1
#define LCD_CMD 0
#define LINE1 0x80
#define LINE2 0XC0
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100

int fd;
int servo=12, btn_left=18, DOUT=23, PD_SCK=24; 
int GAIN=64, GainBits=3, OFFSET=0, offset=0;
int ReferenceUnit=1000;

void hx_powerUp(void) {
	digitalWrite(PD_SCK,0);
	delayMicroseconds(1000);
}

void hx_powerDown(void) {
	digitalWrite(PD_SCK,0);
	digitalWrite(PD_SCK,1);
	delayMicroseconds(1000);
}

void hx_reset(void) {
	hx_powerDown();
	hx_powerUp();
}

char hx_isReady(void) {
	return digitalRead(DOUT)==0;
}

int32_t hx_read(void) {
	while(!hx_isReady());
	int32_t data=0;
	int32_t value=0;
	
	for(int i=0;i<24;i++) {
		digitalWrite(PD_SCK,1);
		value=value<<1;
		digitalWrite(PD_SCK,0);
		delayMicroseconds(1);
		data=digitalRead(DOUT);
		delayMicroseconds(1);
		data=data&1;
		if(data) value++;
		}
	digitalWrite(PD_SCK,1);
	delayMicroseconds(1);
	value=value^0x800000;
	digitalWrite(PD_SCK,0);
	delayMicroseconds(1);
	return value;
}

void hx_setGain(uint8_t gain) {
	switch(gain) {
		case 128: GainBits=1; break;
		case 64: GainBits=3; break;
		case 32: GainBits=2; break;
		}
	digitalWrite(PD_SCK,0);
	hx_read();
}

int32_t hx_readAverage(uint8_t times) {
	int64_t sum=0;
	for(uint8_t i=0; i<times; i++) {
		sum+=hx_read();
		}
	return fabs(sum/times);
}

void hx_setOffset(int32_t offset) {
	OFFSET=offset;
}

void hx_tare(uint8_t times) {
	uint64_t sum=hx_readAverage(times);
	hx_setOffset(sum);
}

int32_t hx_getOffset(void) {
	return OFFSET;
}

int32_t hx_getRawValue(uint8_t times) {
	return abs(hx_readAverage(times)-OFFSET);
}

void hx_setReferenceUnit(float scale) {
	ReferenceUnit=scale;
}

float hx_getReferenceUnit(void) {
	return ReferenceUnit;
}

float hx_getUnits(uint8_t times) {
	return hx_getRawValue(times)/ReferenceUnit;
}

void move(int l, int t) {
	digitalWrite(12, 1);
	delayMicroseconds(l);
	digitalWrite(12, 0);
	delayMicroseconds(t-l);
}

void move_right(int l, int t) {
	int i=0;
	while(i!=90) {
		move(l,t);
		i++;
	}
}

void lcd_toggle_enable(int bits) {
	delayMicroseconds(500);
	wiringPiI2CReadReg8(fd, (bits | ENABLE));
	delayMicroseconds(500);
	wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
	delayMicroseconds(500);
	}
	
void lcd_byte(int bits, int mode) {
	int bits_high;
	int bits_low;
	
	bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
	bits_low = mode | ((bits<<4) & 0xF0) | LCD_BACKLIGHT;
	
	wiringPiI2CReadReg8(fd, bits_high);
	lcd_toggle_enable(bits_high);
	
	wiringPiI2CReadReg8(fd, bits_low);
	lcd_toggle_enable(bits_low);
	}
	
void CtrLcd(void) {
	lcd_byte(0x01, LCD_CMD);
	lcd_byte(0x02, LCD_CMD);
	}

void lcdLoc(int line) {
	lcd_byte(line, LCD_CMD);
	}
	
void typeln(const char *s) {
	while(*s) lcd_byte(*(s++), LCD_CHR);
	}
	
void typeInt(int i) {
	char array1[20];
	sprintf(array1, "%d", i);
	typeln(array1);
	}
	
void typeInt2(int i) {
	char array1[20];
	sprintf(array1, "%i", i);
	typeln(array1);
}
	
void init_lcd(void) {
	lcd_byte(0x33, LCD_CMD);
	lcd_byte(0x32, LCD_CMD);
	
	lcd_byte(0x06, LCD_CMD);
	
	lcd_byte(0x0C, LCD_CMD);
	
	lcd_byte(0x28, LCD_CMD);
	lcd_byte(0x01, LCD_CMD);
	delayMicroseconds(500);
}

int main(void) {
	wiringPiSetupPhys();
	pinMode(btn_left, INPUT);
	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, INPUT);
	fd=wiringPiI2CSetup(L2C_ADDR);
	init_lcd();
	
	printf("weighter wait some time calibration");
	hx_setGain(64);
	hx_read();
	delay(100);
	printf("\nCalibration");
	hx_reset();
	hx_tare(1);
	
	printf("\noffset: %d",hx_getOffset());
	printf("\nShows weight till you want press left-button");
	while(digitalRead(btn_left)==1) {
		CtrLcd();
		printf("\nWeight: %.f",hx_getUnits(1));
		lcdLoc(LINE1);
		typeln("Weigth:      ");
		lcdLoc(LINE2);
		typeInt2(hx_getUnits(1));	
		typeln("g.     ");
		
		if(hx_getUnits(1)>999) {
			move_right(300,20000);
		}
		
		
		delay(500);
		}
}