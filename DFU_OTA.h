#include <avr/interrupt.h> 
#include <stdio.h>            
#include <string.h>
#define BOOT_ENTER_KEY    0x1A

volatile uint8_t r_data=0;
void OTA_init(void){
cli();
MCUSR =0x00;WDTCSR=0x18;
WDTCSR=0x00;UBRR0L=0x19;
UCSR0B=0x98;UCSR0C=0x06;
sei();
}

ISR(USART_RX_vect){
r_data=UDR0;
if(r_data==BOOT_ENTER_KEY){WDTCSR=0x18;WDTCSR=0x08;for(;;);}
}

void OTA_print_text(char *c){
for(uint8_t i=0;i<strlen(c);i++){
   while((UCSR0A & (1<<UDRE0))==0);
   UDR0=(unsigned char)c[i];
   }
}

void OTA_print_number(double x,int always_float){
char line[10];int d1=x,d2=0;
if(x<0){d2=d1*100.0-x*100;}else{d2=x*100.0-d1*100;}
if(d2==0){if(always_float==1){sprintf(line ,"%d.00",d1);}
		  else{sprintf(line,"%d",d1);}}
else if(d2<10){sprintf(line,"%d.0%d",d1,d2);}
else          {sprintf(line,"%d.%d",d1,d2);}
OTA_print_text(line);
}