
#define F_CPU 8000000

#define NOP() asm volatile("nop");

//#define DRATE 460800
//#define DRATE 230400
#define DRATE 115200
//#define DRATE 57600

#define CTRL_PORT PORTD
#define CTRL_DDR DDRD
#define CTRL_PIN PIND

#define DATA1 PINA
#define DATA2 PINC
#define DATA1_PORT PORTA
#define DATA2_PORT PORTC
#define DATA1_DDR DDRA
#define DATA2_DDR DDRC



#define OE 3
#define CE 4
#define WE 2
#define RE 6
#define RD 5
#define LED 7

#define BUT1 0
#define BUTTONS PINB

#define MISO 6
#define MOSI 5
#define SS 4
#define SCK 7

char wait, delay;
#define WAIT(cycles) wait = cycles; while(wait--);
//#define WAIT(cycles)
#define WAIT2() NOP();

#define SETBYTE(var,byte) ((char*)&var)[byte]

#define LED_TOGGLE() PORTD = PORTD ^ ( 1 << LED )
#define LED_ON() PORTD = PORTD | ( 1 << LED )
#define LED_OFF() PORTD = PORTD & ~( 1 << LED )

#define WAIT_RXC() while( !(UCSRA & (1<<RXC)) );
#define WAIT_TXC() while( !(UCSRA & (1<<TXC)) );
#define WAIT_UDRE() while( !(UCSRA & (1<<UDRE)) );
#define WAIT_SPIF() while( !(SPSR & (1<<SPIF)) );
#define SS_HIGH() PORTB = PORTB | (1<<SS);
#define SS_LOW() PORTB = PORTB & ~(1<<SS);


#define reset() CTRL_PORT = 0xFF & ~( 1<<CE | 1<<RE );WAIT(10);CTRL_PORT = CTRL_PORT | 1<<RE; WAIT(10);


void sendCmd(int,char);
void sendCmdData1(char);
void sendCmdData2(char,char);
void sendSPI(char);
void sendSPIcmd(char);
void sendStr(char*);
void recvStr(char*, int);
char readData1();
char readData2();
void setAddr();
void programData();


#define recvChar(data) WAIT_RXC(); data = UDR;
#define sendChar(data) WAIT_UDRE(); UDR = data;

//#define sendSPI(byte) SS_LOW(); SPDR = byte; WAIT_SPIF(); SPDR; SS_HIGH(); WAIT(10); 
#define sendAddrWait(b1,b2,b3) sendSPIcmd('a'); sendSPI(b1); sendSPI(b2); sendSPI(b3);
#define sendAddr(b1,b2,b3) sendSPIcmd('a'); sendSPI(b1); sendSPI(b2); sendSPI(b3);

#define resetCmd() sendCmd(0,0xF0);
#define unlockCmd() sendCmd(0x555, 0xAA); sendCmd(0x2AA, 0x55);



#define OK() sendStr( "Ok\r\n" );
#define ERR(str) sendStr( "ERR!" str "\r\n" );
#define BR() sendStr( "\r\n" );


#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <ctype.h>


unsigned char hextable[] = "0123456789ABCDEF";
unsigned char h2b[128];
#define HEX(byte) hextable[ byte ]
#define BYTE(hex) h2b[hex]

static const short int bufsize = 1<<10;
char buffer[ 512 ];
volatile char head = 0, tail = 0;
unsigned char data = 0, data1, data2;

ISR(TIMER0_COMP_vect) {
	if(head == tail) return;
	if( UCSRA & (1<<UDRE) ) {
		UDR = buffer[head&0xff];
		head++;
	}
}

ISR(TIMER1_OVF_vect) {
	PORTD = PORTD ^ ( 1 << LED );
}

int main() {

	delay = 0;
	unsigned char a;
	for(a='A'; a < 'F'+1; a++) h2b[a]= 10 + a - 'A'; 
	for(a='a'; a < 'f'+1; a++) h2b[a]= 10 + a - 'a'; 
	for(a='0'; a < '9'+1; a++) h2b[a]= a - '0';

	TCNT1H = 0;
	TCNT1L = 0;
	TCCR1B = 1<<CS11 | 1<<CS10; //64
	//TCCR1B = 1<<CS12; //256
	//TIMSK = 1<<TOIE1;


	TCNT0 = 0;
	OCR0 = 200;
	//TCCR0 = 1<<CS01|1<<CS00; //64
	//TCCR0 = 1<<CS01; //8
	TCCR0 = 1<<CS00; //0
	//TIMSK = TIMSK | ( 1<<OCIE0 );


	DDRA=0x0, DDRB = 0x0, DDRC = 0x0, DDRD = 0x0;
	PORTA = 0x0, PORTB = 0xFF, PORTC = 0x0, PORTD = 0x0;


	CTRL_DDR = 0xFF;
	CTRL_PORT = 0xFF & ~(1<<CE);


	UDR = 0;
	UCSRA = 0; //1<<U2X;
	UCSRB = 0;
	UCSRC = 1 << URSEL;
	static const short brate = F_CPU/(16*DRATE) - 1;
	UBRRH = brate>>8 & 127;
	UBRRL = brate & 0xFF;
	UCSRC = 1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0;
	UCSRB = 1<<RXEN | 1<<TXEN;

	SPDR = 0;
	SPCR = 1<<MSTR | 1<<SPE;
	DDRB = DDRB | 1<<MOSI | 1<<SCK | 1<<SS;
	PORTB = PORTB | 1<<MOSI | 1<<SCK | 1<<SS;


	sei();

	while( data != 2 ) {
		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		data = SPDR;
		SS_HIGH();
		WAIT(20);
	}

	sendStr("Ready\r\n");


#define CO() continue

	while(1) {

		void readData();
		void readAddr();
		//readData();
		//readAddr();

		WAIT_RXC();

		data = UDR;

		if( data == 'e' ) {	OK(); CO(); }

		if( data == 'd' ) {
			unsigned long n = (unsigned long)1 << 23;
			sendAddr(0,0,0);
			sendStr("START");
			while( n > 0 ){
/*
				CTRL_PORT = CTRL_PORT & ~( 1<<OE );

				WAIT(5);
				data1 = DATA1;
				data2 = DATA2;

				sendChar( data1 );
				WAIT(5);
				sendChar( data2 );

				CTRL_PORT = CTRL_PORT | ( 1<<OE );
*/
				data1 = readData1();
				data2 = readData2();

				sendChar( data1 );
				sendChar( data2 );

				sendSPIcmd( 'n' );
				n--;
			}
			CO(); 
		}

		if( data == 'f' ) {
			data1 = readData1();
			data2 = readData2();

			sendChar( HEX( data1>>4 & 0xf ) );
			sendChar( HEX( data1 & 0xf ) );

			sendChar( HEX( data2>>4 & 0xf ) );
			sendChar( HEX( data2 & 0xf ) );

			sendSPIcmd( 'n' );
			CO();
		}

		if( data == 'a' ) {
			setAddr();
			CO();
		}

		if( data == 'u' ) {
			recvChar(data);
			delay = data-'0';
			sendChar(delay+'0');
			OK();
			CO();
		}



		if( data == 'q' ) {
			sendCmd(0x555, 0xAA);
			OK();
			CO();
		}

		if( data == 'w' ) {
			LED_OFF();

			unlockCmd();
			sendCmd(0x555, 0xA0);

			setAddr();
			recvChar(data);

			if( data != ' ' ) {
				ERR("Bad cmd");
				CO();
			}

			recvChar(data);
			data1 = (BYTE( data ) & 0xF) << 4;
			recvChar(data);
			data1 = data1 | ( BYTE( data ) & 0xF );

			recvChar(data);
			data2 = (BYTE( data ) & 0xF) << 4;
			recvChar(data);
			data2 = data2 | ( BYTE( data ) & 0xF );

			sendChar( HEX( data1>>4&0xf ) );
			sendChar( HEX( data1&0xf ) );
			sendChar( HEX( data2>>4&0xf ) );
			sendChar( HEX( data2&0xf ) );
			BR();

			sendCmdData2( data1, data2 );

			unsigned char count1 = 1, count2 = 1;

			char data7 = data1 & (1<<7);
			data = (~data1) & (1<<7);

			while( data7 ^ data ) {
				data = readData1();
				if( count1 == 0 ) { 
					LED_TOGGLE(); 
					count2++; 
				}
				if( count2 == 0 ) { 
					LED_ON(); 
					sendStr("Timeout\r\n");
					reset();
					break; 
				}
				count1++;
				data = data & (1<<7);
			}
			OK();
			LED_ON();
			CO();
		}

		if( data == 'c' ) {
			LED_OFF();

			unlockCmd();
			sendCmd(0x555, 0x80);
			unlockCmd();
			setAddr();
			sendCmdData1( 0x30 );
			_delay_ms(50);

			char dq7 = 0, count1 = 0;
			while( dq7 == 0 ) {
				data = readData1();
				if( count1 == 0 ) 
						LED_TOGGLE(); 
				count1++;
				dq7 = data & (1<<7);
			}

			OK();
			LED_ON();
			CO();
		}

		if( data == 't' ) {
			reset();
			sendAddr(0x0,0x0,0x0);
			DATA1_PORT = 0x0;
			DATA2_PORT = 0x0;
			DATA1_DDR = 0x0;
			DATA2_DDR = 0x0;
			OK();
			CO();
		}


		if( data == 's' ) {
			unlockCmd();
			sendCmd(0x555, 0x90);

			sendAddrWait(0x00,0x00,0x00);
			data1 = readData1();
			sendChar( HEX( data1>>4&0xf ) );
			sendChar( HEX( data1&0xf ) );

			sendChar( ' ' );

			sendAddrWait(0x01,0x00,0x00);
			data1 = readData1();
			sendChar( HEX( data1>>4&0xf ) );
			sendChar( HEX( data1&0xf ) );

			sendAddrWait(0x0E,0x00,0x00);
			data1 = readData1();
			sendChar( HEX( data1>>4&0xf ) );
			sendChar( HEX( data1&0xf ) );

			sendAddrWait(0x0F,0x00,0x00);
			data1 = readData1();
			sendChar( HEX( data1>>4&0xf ) );
			sendChar( HEX( data1&0xf ) );

			resetCmd();

			sendStr("\r\n"); OK();
		}

		if( data == 'g') {
			WAIT_RXC();
			data = UDR;
			if( !isdigit( data ) ) { ERR(); }
			else {
				char pin = data - '0';
				WAIT_RXC();
				data = UDR;
				CTRL_DDR = CTRL_DDR | ( 1<<pin );
				CTRL_PORT = data == 'h' ? CTRL_PORT | 1 << pin : CTRL_PORT & ~( 1 << pin );
				OK();
			}
			CO(); 
		}

		if( data == 'v' ) {
			char pins = CTRL_PIN;
			char i = 0;

			while( i < 8 ) {
				sendChar( (pins >> i & 0x1) + '0' );
				i++;
			}
			sendStr( "\r\n" );
			CO(); 
		}

		if( data == 'l') {
			PORTD = PORTD ^ ( 1 << LED );
			OK();
			CO();
		}

		if( data == 'p') {
			unsigned char xor = 0;
			sendStr( " Ok " );

			int bytes = 0;
			while(bytes<512){
				recvChar( data );
				//sendChar(data);
				buffer[bytes] = data;
				xor = xor ^ data;
				bytes++;
			}

			sendStr(" Ok");
			sendChar(xor);

			recvChar( data );
			if( data == '.' ) 
					programData();

			_delay_ms(1);
			OK();
			CO();
		}



	} //while(1)

return 0;
}

/*
void readData() {
	volatile char ce;
	char byte1, byte2;//, wait;

	while(1) {

		ce = 1;
		while(ce)
			ce = CTRL_PIN & (1 << CE);

		//cli();
		//wait = 100;
		//while(wait--);
		//_delay_us(80);
		byte1 = DATA1;
		byte2 = DATA2;
		//sei();

		if( tail - head == bufsize ) { continue; }

		buffer[tail&0xff] = byte1;
		tail++;

		if( tail - head == bufsize ) { tail--; continue; }

		buffer[tail&0xff] = byte2;
		tail++;

		while(!ce)
			ce = CTRL_PIN & (1 << CE);
	}
}
*/
void sendStr(char *str) {
	short int i = 0;
	while( str[i] ) {
		WAIT_UDRE();
		UDR = str[i];
		i++;
	}
}

void recvStr(char *buf, int n) {
	short int i = 0;
	while( i < n ) {
		WAIT_RXC();
		buf[i] = UDR;
		i++;
	}
}
void recvStr6(char *buf) {
		WAIT_RXC(); buf[0] = UDR;
		WAIT_RXC(); buf[1] = UDR;
		WAIT_RXC(); buf[2] = UDR;
		WAIT_RXC(); buf[3] = UDR;
		WAIT_RXC(); buf[4] = UDR;
		WAIT_RXC(); buf[5] = UDR;
}


/*
void sendData() {
	cli();
	for(char A = 0; A != 255; A++) {
		PORTD = PORTD & ( 1 << OE | 1 << CE );
		PORTB = A;
		//DDRB = 0xFF;
		PORTD = PORTD & ~( 1 << OE | 1 << CE );
		//NOP();
		//NOP();
		//DDRB = 0x00;
		while( !(UCSRA & (1<<UDRE)) );
		UDR = PINC;
		while( !(UCSRA & (1<<UDRE)) );
		UDR = PINA;
		PORTD = PORTD ^ ( 1 << LED );
	}
	sei();
}

void readAddr() {
	char data, addr1, addr2, addr3, addr4;

	data = 0;

	while( data != 2 ) {
		sendStr("Wait for 2... Got: ");
		sendChar(data + '0');
		sendStr("\r\n");
		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		data = SPDR;
		SS_HIGH();
	}

	volatile char ce = 1;
	while(ce)
		ce = CTRL_PIN & (1 << CE);

	while(1) {

		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		SS_HIGH();
		addr1 = SPDR;

		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		SS_HIGH();
		addr2 = SPDR;

		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		SS_HIGH();
		addr3 = SPDR;

		SS_LOW();
		SPDR = 1;
		WAIT_SPIF();
		SS_HIGH();
		addr4 = SPDR;

		if( SPDR != 0 ) { sendStr("Error!\r\n"); continue; };
//#define continue return

		if( tail - head == bufsize ) { continue; }
		buffer[tail&0xff] = addr4;
		tail++;

		if( tail - head == bufsize ) { tail--; continue; }
		buffer[tail&0xff] = addr3;
		tail++;

		if( tail - head == bufsize ) { tail=tail-2; continue; }
		buffer[tail&0xff] = addr2;
		tail++;

		if( tail - head == bufsize ) { tail=tail-3; continue; }
		buffer[tail&0xff] = addr1;
		tail++;

	} //while(1)

}
*/
char readData2() {
	char mydata;
	CTRL_PORT = CTRL_PORT & ~( 1<<OE );
	WAIT2();
	mydata = DATA2;	
	CTRL_PORT = CTRL_PORT | ( 1<<OE );
	WAIT2();
	return mydata;
}

char readData1() {
	char mydata;
	CTRL_PORT = CTRL_PORT & ~( 1<<OE );
	WAIT2();
	mydata = DATA1;	
	CTRL_PORT = CTRL_PORT | ( 1<<OE );
	WAIT2();
	return mydata;
}

void sendSPI(char byte) {
	SS_LOW(); SPDR = byte; WAIT_SPIF(); SPDR; SS_HIGH(); WAIT(10);
}
void sendSPIcmd(char byte) {
	SS_LOW(); SPDR = byte; WAIT_SPIF(); SPDR; SS_HIGH(); WAIT(20);
}
void sendCmd( int addr, char d ) {
	sendAddrWait( SETBYTE(addr,0), SETBYTE(addr,1), 0x0 );
	DATA1_DDR = 0xff;
	DATA1_PORT = d;
	WAIT2();
	CTRL_PORT = CTRL_PORT & ~(1<<WE);
	WAIT2();
	CTRL_PORT = CTRL_PORT | (1<<WE);
	DATA1_PORT = 0x00;
	DATA1_DDR = 0x00;
	WAIT2();
}
void sendCmdData1( char d ) {
	DATA1_DDR = 0xff;
	DATA1_PORT = d;
	WAIT2();
	CTRL_PORT = CTRL_PORT & ~(1<<WE);
	WAIT2();
	CTRL_PORT = CTRL_PORT | (1<<WE);
	DATA1_PORT = 0x00;
	DATA1_DDR = 0x00;
	WAIT2();
}
void sendCmdData2( char d1, char d2 ) {
	DATA1_DDR = 0xff;
	DATA1_PORT = d1;
	DATA2_DDR = 0xff;
	DATA2_PORT = d2;
	WAIT2();
	CTRL_PORT = CTRL_PORT & ~(1<<WE);
	WAIT2();
	CTRL_PORT = CTRL_PORT | (1<<WE);
	DATA1_PORT = 0x00;
	DATA1_DDR = 0x00;
	DATA2_PORT = 0x00;
	DATA2_DDR = 0x00;
	WAIT2();
}

void setAddr() {
	unsigned char addr[7];
	recvStr6( (char*)addr );
	addr[6] = '\0';
	sendStr("0x");
	sendStr( (char*)addr );
	BR();

	sendSPIcmd( 'a' );

	int i = 0;
	while( i < 5 ) {
		char byte = BYTE( addr[4-i] ) << 4 | BYTE( addr[5-i] );
		sendSPI( byte );
		sendChar( HEX( byte>>4&0xf ) );
		sendChar( HEX( byte&0xf ) );
		i=i+2;
	}
	BR();

}


void programData() {
	unsigned char data, count, dq7, addr1, addr2, addr3;//, fail, try;
	int bytes = 0;

	recvChar(addr2);
	recvChar(addr1);

	sendChar(addr2);
	sendChar(addr1);

	recvChar(data);
	if( data != '.' ) {
		sendStr("ERR");
		sendChar(0);
		sendChar(0);
		sendChar(addr1);
		sendChar(addr2);
		sendChar(0);
		return;
	}

	while(bytes<256){
		LED_OFF();

		unlockCmd();
		sendCmd(0x555, 0xA0);

		data1 = buffer[bytes*2];
		data2 = buffer[bytes*2+1];

		addr3 = bytes & 0xFF;

		sendAddrWait( addr3, addr2, addr1 );
		sendCmdData2( data1, data2 );

		dq7 = data1 & (1<<7);
		data = (~data1) & (1<<7);

		count = 0;

		while( dq7 ^ data ) {
			data = readData1();
			LED_TOGGLE();
/*
			sendStr("TUS");
			sendChar(data1);
			sendChar(data);
			sendChar(addr1);
			sendChar(addr2);
			sendChar(addr3);

			recvChar(data2);
*/
			count++;
			if( count == 0 ) {
					sendStr("ERR");
					sendChar(data1);
					sendChar(data);
					sendChar(addr1);
					sendChar(addr2);
					sendChar(addr3);
					return;
			}
			data = data & (1<<7);
		}

//		sendChar('.');	
		bytes = bytes+1;
	} 

	LED_ON();
}



