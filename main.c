#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DFU_OTA.h"

#define  LED_DDR               DDRD
#define  LED_PORT              PORTD
#define  LED_PIN               2

#define  LED_on()              {LED_DDR|=(1<<LED_PIN);LED_PORT|=(1<<LED_PIN);}
#define  LED_off()             {LED_PORT&=~(1<<LED_PIN);LED_DDR&=~(1<<LED_PIN);}

#define  VCC_CHANNEL           6
#define  SUPPLY_CHANNEL        7

volatile uint8_t RX_buffer[24],cnt=0,actual_addr[14];
uint32_t adc_constant=0,adc_val=0,adc_sum=0,adc_avg=0;



void EEPROM_write(uint16_t EE_address, unsigned char EE_data){
while(EECR & (1<<EEPE));
EEAR=EE_address;
EEDR=EE_data;
EECR=(1<<EEMPE);
EECR|=(1<<EEPE);
}

unsigned char EEPROM_read(uint16_t EE_address){
while(EECR & (1<<EEPE));
EEAR=EE_address;
EECR|=(1<<EERE);
return EEDR;
}

uint32_t ADC_read(uint8_t channel){
ADMUX  = 0xC0;
ADMUX |= channel;
ADCSRA=(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2)|(1<<ADEN);
_delay_ms(100);
ADCSRA|=(1<<ADSC);
while((ADCSRA & (1<<ADIF))==0);
uint16_t adc_val=ADCW;
ADCSRA|=(1<<ADIF);
return adc_val;
}


void adc_cal(uint32_t voltage_mv){
OTA_print_text("\nMake sure supply is greater than 4.8v");
OTA_print_text("\nIf supply is less than 4.8v, calibration result is invalid!\n");
OTA_print_text("Task in progress!\n\n");
adc_val=ADC_read(VCC_CHANNEL);
adc_val=ADC_read(VCC_CHANNEL);
adc_val=ADC_read(VCC_CHANNEL);
adc_val=ADC_read(VCC_CHANNEL);
adc_val=ADC_read(VCC_CHANNEL);
for(uint8_t i=0;i<30;i++)
        {
          adc_val=ADC_read(VCC_CHANNEL);
		  adc_sum+=adc_val;
		  _delay_ms(100);
        }
adc_avg=adc_sum/30;
adc_constant=(adc_avg*3000)/voltage_mv;
EEPROM_write(1023,adc_constant);
OTA_print_text("ADC Calibration Value : ");
OTA_print_number(adc_constant,1);
OTA_print_text("\nCalibration value can be accessed from EEPROM address 1023\n");
for(uint8_t i=0;i<10;i++)
        {
          adc_val=ADC_read(SUPPLY_CHANNEL)*300;
        }
OTA_print_text("\nVoltage parameters...\n");
OTA_print_text("Supply voltage  : ");
OTA_print_number((adc_val/adc_constant)/100.0,1);
OTA_print_text(" V\n");
for(uint8_t i=0;i<10;i++)
        {
          adc_val=ADC_read(VCC_CHANNEL)*300;
        }
OTA_print_text("CPU voltage     : ");
OTA_print_number((adc_val/adc_constant)/100.0,1);
OTA_print_text(" V\n");
OTA_print_text("\nADC successfully calibrated!\n");
OTA_print_text("Ready for new application program to upload!\n");
}

int main(void){
OTA_init();
adc_cal(3270);


while(1){
           
		   
		   
		   }
}