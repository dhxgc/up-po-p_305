// Lab4
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define L2C_ADDR 0x3f
#define LCD_CHR 1
#define LCD_CMD 0
#define LINE1 0x80
#define LINE2 0XC0
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100
int fd;
int btn_up=19, LightPin=22;
int Press_up_detected=0;

char* get_temp_raw(void){
		int str_len=100;
		system("find /sys/bus/w1/devices/ -name 28* >$PWD/temp_way.txt");
		static char temp_raw_result[200]={0};
		char buff[100];
		FILE *way_file = fopen("temp_way.txt", "r");
		fgets(buff, str_len,way_file);
		for (int count = 0; count < (int)strlen(buff); count++)
			if(buff[count]=='\n') buff[count]=0;
		fclose(way_file);
		strcat(buff,"/w1_slave");
		FILE *sensor_file=fopen(buff, "r");
		while (fgets(buff,str_len, sensor_file) != NULL)
		{
			strcpy(temp_raw_result, buff);
		}
		//printf(" TESTS BUFF:   %s\n", temp_raw_result);
		return temp_raw_result;
}

float get_temp(void) {
		char* mas = get_temp_raw();
		//printf(get_temp_raw());
		char str_temp[10]={0};
		int str_pos=0;
		int char_pos=0;
		while(mas[char_pos]!='t') char_pos++;
		char_pos+=2;
		for (int count = char_pos; count < (int)strlen(mas); count++)
		{
			str_temp[str_pos]=mas[count];
			str_pos++;
		}
		float temp=(atof(str_temp)/1000);
		//printf(" Temp:   %f\n", temp);
		return temp;
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
	
void typeFloat(float i) {
	char array1[20];
	sprintf(array1, "%.3f", i);
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

void Press_up_Interrupt(void) {
	Press_up_detected=1;
	delay(100);
}


int main(void) {
wiringPiSetupPhys();
pinMode(btn_up, INPUT);
pinMode(LightPin, INPUT);
wiringPiISR(btn_up,INT_EDGE_FALLING, &Press_up_Interrupt);	
fd=wiringPiI2CSetup(L2C_ADDR);
init_lcd();

while(1) {
	
	if ((!digitalRead(LightPin))==1){
		lcdLoc(LINE1);
		typeln("Light ON         ");
		lcdLoc(LINE2);
		typeln("Temp: ");
		typeFloat(get_temp());		
	} else {
		lcdLoc(LINE1);
		typeln("Light OFF         ");
		lcdLoc(LINE2);
		typeln("Temp: ");
		typeFloat(get_temp());	
		}
	
	if(Press_up_detected==1) {
		Press_up_detected=0;
		CtrLcd();
		lcdLoc(LINE1);
		typeln("Hoox1");
		
		lcdLoc(LINE2);
		typeln("solo");
		delay(2000);
			}

		delay(1000);
	}	
}