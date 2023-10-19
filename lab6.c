// Lab6		
#include <stdio.h>
#include <wiringPi.h>
int StepPins[4]={40,38,36,37};
int WaitTime=10;
void toright(int degree) {
	int stepcounter=0; //счетчик (типа текущее положение грубо говоря, и от него мы будем 
	// бежать к введенному положению)
	int steps=(degree/0.175); //делим введеный угол на угол шага, чтобы получить 
	//требуемое кол-во шагов, которое нужно пройти
	printf ("steps: %i\n", steps); // выводим шаг ВВЕДЕННЫЙ
	while (stepcounter<=steps) { // работаем пока текущий шаг не стал равен введенному шагу
		//устанавливаем состояние пинов управления
		digitalWrite(StepPins[(stepcounter-1)%4],0);
		digitalWrite(StepPins[(stepcounter)%4],1);
		digitalWrite(StepPins[(stepcounter+1)%4],0);
		
		printf ("step %d\n", stepcounter); //здесь уже выводим ТЕКУЩИЙ шаг
		stepcounter++; //повышаем счетчик на 1 после пройденного шага
		delay(WaitTime); //задержка 10мс
	}
}
void toleft(int degree) {
	int stepcounter=(degree/0.175); // делим введеный угол на угол шага, чтобы получить 
	//требуемое кол-во шагов, которое нужно пройти
	int steps=0; // счетчик 
	while (stepcounter>=steps) { //работаем, пока нужное кол-во шагов >= 0
		// в итоге мы пройдем (при degree = 512) 512 шагов влево
		//с каждым пройденным шагом мы будем уменьшать остаток еще не пройденных шагов
		digitalWrite(StepPins[(stepcounter-1)%4],0);
		digitalWrite(StepPins[(stepcounter)%4],1);
		digitalWrite(StepPins[(stepcounter+1)%4],0);
		
		printf ("step %d\n", stepcounter);
		stepcounter--;
		delay(WaitTime);
	}
}
int main() {
	if (wiringPiSetupPhys()==-1) return 0; //опрос устройств поделкдюченных
	pinMode(StepPins[0], OUTPUT); //стандарт просто делаем пинмод
	pinMode(StepPins[1], OUTPUT);
	pinMode(StepPins[2], OUTPUT);
	pinMode(StepPins[3], OUTPUT);
	int spinto, degree;
	while (1) {
		scanf("%i%i", &spinto, &degree);
		if (spinto==0) { //если идем вправо
			toright(degree);
		} else if (spinto==1) { // если идем влево
			toleft(degree);
		} else {break;}
	}

digitalWrite(StepPins[0],0);
digitalWrite(StepPins[1],0);
digitalWrite(StepPins[2],0);
digitalWrite(StepPins[3],0);
}
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <thread>

#define L2C_ADDR 0x3f
#define LCD_CHR 1
#define LCD_CMD 0
#define LINE1 0x80
#define LINE2 0XC0
#define LCD_BACKLIGHT 0x08
#define ENABLE 0b00000100

#define ch0 0x40 // Light
#define ch1 0x41 // Temp
#define ch2 0x42 // X Axis
#define ch3 0x43 // Y Axis
using namespace std;

int fd;
int btn_up=19, LightPin=22;
int Press_up_detected=0;
int StepPins[4]={40,38,36,37};
int WaitTime=4;

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
	sprintf(array1, "%.0f", i);
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

void updatet(void) {
	wiringPiSetupPhys();
	int device=wiringPiI2CSetup(0x48);
		while(1) {
			system("clear");
			wiringPiI2CReadReg8(device,ch3);
			printf("X Axis: %d\n",wiringPiI2CReadReg8(device,ch3));
			wiringPiI2CReadReg8(device,ch2);
			printf("Y Axis: %d\n",wiringPiI2CReadReg8(device,ch2));			
			if(wiringPiI2CReadReg8(device,ch3) > 0 && wiringPiI2CReadReg8(device,ch3) < 10) {
				lcdLoc(LINE1);
				typeln("X Axis: ");
				wiringPiI2CReadReg8(device,ch3);
				typeFloat(wiringPiI2CReadReg8(device,ch3));
				typeln("      ");	
			} else if (wiringPiI2CReadReg8(device,ch3) > 9 && wiringPiI2CReadReg8(device,ch3) < 100) {
						lcdLoc(LINE1);
						typeln("X Axis: ");
						wiringPiI2CReadReg8(device,ch3);
						typeFloat(wiringPiI2CReadReg8(device,ch3));	
						typeln("      ");				
					} else if (wiringPiI2CReadReg8(device,ch3) > 99 && wiringPiI2CReadReg8(device,ch3) < 256) {
							lcdLoc(LINE1);
							typeln("X Axis: ");
							wiringPiI2CReadReg8(device,ch3);
							typeFloat(wiringPiI2CReadReg8(device,ch3));	
							typeln("      ");						
						}
					
			if(wiringPiI2CReadReg8(device,ch2) > 0 && wiringPiI2CReadReg8(device,ch2) < 10) {
				lcdLoc(LINE2);
				typeln("Y Axis: ");
				wiringPiI2CReadReg8(device,ch2);
				typeFloat(wiringPiI2CReadReg8(device,ch2));
				typeln("      ");		
			} else if (wiringPiI2CReadReg8(device,ch2) > 9 && wiringPiI2CReadReg8(device,ch2) < 100) {
					lcdLoc(LINE2);
					typeln("Y Axis: ");
					wiringPiI2CReadReg8(device,ch2);
					typeFloat(wiringPiI2CReadReg8(device,ch2));			
					typeln("      ");		
				} else if (wiringPiI2CReadReg8(device,ch2) > 99 && wiringPiI2CReadReg8(device,ch2) < 256) {
						lcdLoc(LINE2);
						typeln("Y Axis: ");
						wiringPiI2CReadReg8(device,ch2);
						typeFloat(wiringPiI2CReadReg8(device,ch2));	
						typeln("      ");				
					}
		delay(500);
	}
}

	
int main() {
	int steps, stepcounter;
	int device=wiringPiI2CSetup(0x48);
	
	wiringPiSetupPhys();
	pinMode(btn_up, INPUT);
	pinMode(LightPin, INPUT);	
	fd=wiringPiI2CSetup(L2C_ADDR);
	init_lcd();
	CtrLcd();
	thread t1(updatet);
	while(1) {
		while((wiringPiI2CReadReg8(device,ch3))<256 && (wiringPiI2CReadReg8(device,ch3))>251) {
			steps+=4;
			stepcounter=0;
			while(stepcounter<=steps) {
				digitalWrite(StepPins[(stepcounter-1)%4],0);
				digitalWrite(StepPins[(stepcounter)%4],1);
				digitalWrite(StepPins[(stepcounter+1)%4],0);
				stepcounter++;
				delay(WaitTime);
				}	
			}
			
		while((wiringPiI2CReadReg8(device,ch3))>=0 && (wiringPiI2CReadReg8(device,ch3))<6) {
			steps=0;
			stepcounter+=4;
				while(stepcounter>=steps) {
					digitalWrite(StepPins[(stepcounter-1)%4],0);
					digitalWrite(StepPins[(stepcounter)%4],1);
					digitalWrite(StepPins[(stepcounter+1)%4],0);
					stepcounter--;
					delay(WaitTime);
					}		
			}
		delay(500);
		}
		t1.join();	
}
