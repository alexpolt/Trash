
#define F_CPU 8000000
#define NOP() asm volatile("nop");

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED 0

#define MISO 6
#define MOSI 5
#define SS 4
#define SCK 7

#define WAIT_SPIF() while( !(SPSR & (1<<SPIF)) );
#define SKIP_SPIF() while( !(SPSR & (1<<SPIF)) ); SPDR;

#define BYTE(var,byte) ((char*)&var)[byte]

ISR(TIMER1_OVF_vect) {
		PORTB = PORTB ^ ( 1 << LED );
}


int main() {

unsigned char data, addr1, addr2, addr3;
unsigned long addr = (unsigned long)0;

TCCR1B = 1<<CS11|1<<CS10; //64
TCCR1B = 1<<CS12; //128
//TIMSK = 1<<TOIE1;


DDRA = 0, DDRB = 0, DDRC = 0, DDRD = 0;
PORTA = 0x0, PORTB = 0x0, PORTC = 0x0, PORTD = 0x0;
DDRB = DDRB | ( 1 << LED );
PORTB = PORTB ^ ( 1 << LED );


SPDR = 0;
SPCR = 1 << SPE;
DDRB = DDRB | ( 1<<MISO );


sei();

while( SPDR != 1 ) WAIT_SPIF();

SPDR = 2;
SKIP_SPIF();

DDRA = 0xff, DDRC = 0xff, DDRD = 0xff;
PORTA = 0x0, PORTC = 0x0, PORTD = 0x0;

while(1) {

	WAIT_SPIF();
	data = SPDR;

	if( data == 'n' ) {
		addr++;
//		PORTA = (addr >> 0 ) & 0xff;
//		PORTC = (addr >> 8 ) & 0xff;
//		PORTD = (addr >> 16) & 0xff;		
		PORTA = ( BYTE(addr,0) ) & 0xff;
		PORTC = ( BYTE(addr,1) ) & 0xff;
		PORTD = ( BYTE(addr,2) ) & 0xff;		
		continue;
	}

	if( data == 'a' ) {
		WAIT_SPIF();
		addr1 = SPDR;
		WAIT_SPIF();
		addr2 = SPDR;
		WAIT_SPIF();
		addr3 = SPDR;

		PORTA = addr1;
		PORTC = addr2;
		PORTD = addr3;

		BYTE(addr,0) = addr1;
		BYTE(addr,1) = addr2;
		BYTE(addr,2) = addr3;
		//addr = (unsigned long) addr3<<16 | addr2<<8 | addr1;
		continue;
	}


	//if( data == 'l' )
	//	PORTB = PORTB ^ ( 1 << LED );
	//_delay_ms(500);
}


return 0;
}




