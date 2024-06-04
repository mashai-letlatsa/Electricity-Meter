/*
 * NewElecCode.c
 *
 * Created: 2024/04/11 13:34:22
 * Author : 219024684
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <avr/eeprom.h>

#define F_CPU 8000000UL
#define BAUDRATE 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUDRATE)) - 1)
//*****
#define LCDDDDR DDRB
#define LCDCDDR DDRD
#define LCDDATA PORTB
#define LCDCONTR PORTD
//***

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define RS 4
#define RW 5
#define EN 6
#define RL 3
#define WV 7
#define SAMPLES 1000 // Reduced number of samples for optimization

float peak_voltage = 0.0;
float peak_current = 0.0;
float accumulated_energy_kWh = 0.0;
//volatile uint32_t time_elapsed_seconds = 0;
float Ebalance = 1.84;
volatile uint16_t time_elapsed_seconds = 0;
volatile uint16_t previous_time_difference = 0;
volatile uint16_t time_difference = 0;

void ADC_Init() {
	ADMUX |= (1 << REFS0); // Set voltage reference to AREF
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler 128
}

unsigned int ADC_Read(unsigned char INPUT) {
	ADMUX = (ADMUX & 0xF0) | (INPUT & 0x0F); // Select ADC channel
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
	return ADC;
}
// LCD functions
void lcdCommand(char cmnd){
	LCDDATA = cmnd;
	LCDCONTR &= ~(1<<RS); // RS = 0 to indicate that we send command to lcd;
	LCDCONTR |= (1<<EN);  // pulse enable;
	_delay_ms(5);
	LCDCONTR &= ~(1<<EN);
	_delay_ms(5);
}

void lcdData(char data){
	LCDDATA = data;
	LCDCONTR |= (1<<RS); // RS = 1 to indicate that we sending a data to the lcd;
	LCDCONTR  |= (1<<EN);  // pulse enable;
	_delay_ms(5);
	LCDCONTR  &= ~(1<<EN);
	_delay_ms(5);
}

void lcdInit(){
	LCDDDDR=0xFF;//PORTB as digital output
	LCDCDDR|=(1<<EN)|(1<<RS)|(1<<RW);
	_delay_ms(5);
	lcdCommand(0x38); // 8 bits mode initialization
	lcdCommand(0x0c);// display cursor ON and OFF
	lcdCommand(0x06); // shift cursor to the right
	lcdCommand(0x01); // clear display
	_delay_ms(5);
}

void lcdClear(){
	lcdCommand(0x01); // clear display
}

void lcdString(char* str){
	int i;
	for(i=0; str[i]!=0;i++){
		lcdData(str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)  /* Send string to LCD with x_y position */
{
	char address = 0;
	if (row == 0) {
		address = 0x00 + pos;
		} else if (row == 1) {
		address = 0x40 + pos;
		} else if (row == 2) {
		address = 0x10 + pos;
		} else if (row == 3) {
		address = 0x50 + pos;
	}
	lcdCommand(address | 0x80);
	lcdString(str);
}


void Voltage(){
	float sum_voltage = 0.0;
	float sum_current = 0.0;

	for (int i = 0; i < SAMPLES; i++) {
		float V = ADC_Read(A2);
		float C = ADC_Read(A3);
		float current_voltage = ((V * 5.0) / 1024.0);
		float actual_voltage=(((current_voltage + 0.09)*(8.5))-5.7)*10;
		float current_current = fabs(((C * 5.0) / 1024.0) - 2.5) / 0.06;

		sum_voltage += actual_voltage;
		sum_current += current_current;
	}

	peak_voltage = sum_voltage / SAMPLES;
	peak_current = sum_current / SAMPLES;
		char volt[20];
		char shelembe[20];
		strcpy(volt,"V=");
		dtostrf(peak_voltage, 5 , 2,shelembe);
		strcat(volt,shelembe);
		//strcat(volt,"V");
		LCD_String_xy(0,0, volt);
		
		char curr[20];
		char buffer[20];
		strcpy(curr,"I=");
		dtostrf(peak_current, 4 , 2,buffer);
		strcat(curr,buffer);
		//strcat(curr,"A");
		LCD_String_xy(0,9, curr);
}

void CalculatePower() {
	Voltage();
	float power = peak_voltage * peak_current;
	float power_kW = power / 1000.0;

	char power_str[20];
	char buffer[20];
	strcpy(power_str,"P=");
	dtostrf(power_kW, 4, 2, buffer);
	strcat(power_str, buffer);
	strcat(power_str,"kW");
	LCD_String_xy(1, 0, power_str);
// Calculate time difference in seconds
uint16_t current_time_difference = time_difference;
uint16_t difference_seconds = (current_time_difference - previous_time_difference);

	float energy_kWh = (power * difference_seconds) / 3600000.0;
	accumulated_energy_kWh += energy_kWh;

    char energy_str[20];
    char energy_buff[20];
    strcpy(energy_str,"E=");
    dtostrf(accumulated_energy_kWh, 4, 2, energy_buff);
    strcat(energy_str, energy_buff);
    strcat(energy_str,"kWh");
    LCD_String_xy(2, 0, energy_str);

	if (Ebalance >= (energy_kWh)/1.84) {
		Ebalance -= (energy_kWh)/1.84;
		} else {
		Ebalance = 0.0;
		peak_current = 0.0;
	}

		char k[20];
		char h[20];
		strcpy(h,"BAL=");
		LCD_String_xy(3,0,h);
		strcpy(k,"R");
		dtostrf(Ebalance,5,2,&k[1]);
		LCD_String_xy(3,4,k);
}

void Elec_Control(){
	CalculatePower();
	if (Ebalance <= 0.0) {
		PORTA &= ~(1 << PA4);
		LCD_String_xy(1, 9, "Pwr:OFF");
		_delay_us(10);
		} else {
		PORTA |= (1 << PA4);
		LCD_String_xy(1, 9, "Pwr:ON");
		_delay_us(10);
	}
}

void initTimer() {
	TCCR1B |= (1 << WGM12) | (1 << CS12); // CTC mode, prescaler 256
	OCR1A = 31250; // Compare value for 1 second interrupt interval
	TIMSK |= (1 << OCIE1A); // Enable Timer1 Compare Match A interrupt
	sei(); // Enable global interrupts
}

ISR(TIMER1_COMPA_vect) {
	time_elapsed_seconds++; // Increment time counter every second
	previous_time_difference = time_difference; // Store the previous time difference
	time_difference = time_elapsed_seconds; // Store the current time difference
}
int main() {
	DDRA &= ~(1 << PA2);
	DDRA &= ~(1 << PA3);
	DDRA |= (1 << PA4); // RELAY CONTROL PIN INTO OUTPUT
	ADC_Init();
	lcdInit();
	initTimer();

	while (1) {
		Elec_Control();
	}
	return 0;
}
