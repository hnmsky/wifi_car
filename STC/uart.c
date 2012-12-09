/* Description	: uart send and receive.
 *	Author		: Nate Hu
 *	Data		: 12 Nov,2012
 */
#include <DB1.H>
#include "api.h"
#define RELOAD_COUNT (0xD9)//baud 9600

#define UART_P1M0 0xC //1100 set to Open Drain mod. The uart  of WRT703n is 3V, but STC chip is 5V.
#define UART_P1M1 0xC //1100


enum machine_state{
	STA_NULL,
	STA_START0,
	STA_START1,
	STA_CMD,
	STA_DATA,
	STA_END0,
	STA_END1,
};
unsigned char state = STA_NULL;

unsigned char uart_data = 0;
void parse_cmd(unsigned char val);

void uart2_init()
 {
 	P1M0 = UART_P1M0;
	P1M1 = UART_P1M1;

 	S2CON = 0x50;//0101 0000 
	BRT = RELOAD_COUNT;

	AUXR &= ~0x3F;
	AUXR |= 0x15 & 0x3F;//0001 0101

	IE2 = 0x01;//enable UART2 INT
	EA = 1;	//enable INT
 }

void uart2_send(unsigned char val)
 {
 	unsigned char tmp = 0;
	IE2 = 0;//disable UART2 INT
	S2CON = S2CON & (~0x10);//TI = 0
	S2BUF = val;
	do{
		tmp = S2CON;
		tmp = tmp & 0x2;
	}while(tmp == 0);//while(TI == 0)

	S2CON = S2CON & (~0x10);//TI = 0
	IE2 = 0x1;//enable UART2 INT
	
 }
 void uart2_receive()interrupt 8
 {
 	unsigned char tmp = 0;
	
	tmp = S2CON;
	tmp = tmp & 0x01;//RI
	if(tmp == 1){
		S2CON = S2CON & (~0x1);//RI = 0
		uart_data = S2BUF;
		parse_cmd(uart_data);
		//uart2_send(uart_data);//send back
	}

 }

void parse_cmd(unsigned char val)//command: START0 START1 CMD DATA END0 END1. DATA is optional
{
	/* only receive characters and numbers*/
	if((val < '0') || (val > 'z'))
		return;
	switch(state){
		case STA_NULL:
			if(val == START0)
				state = STA_START0;
			else
				state = STA_NULL;
			break;
		case STA_START0:
			if(val == START1)
				state = STA_START1;
			else
				state = STA_NULL;
			break;
		case STA_START1:
			state = STA_CMD;
			if(val == FORWARD){
				forward();	
			}
			else if(val == BACK){
				back();
			}
			else if(val == LEFT){
				left();
			}
			else if(val == RIGHT){
				right();
			}
			else if(val == UP){
				up();
			}
			else if(val == DOWN){
				down();
			}
			else if(val == STOP){
				stop();
			}
			else{
				state = STA_NULL;
			}
			break;
		case STA_CMD://Do we need this?
			if(val == END0)
				state = STA_END0;
			else
				state = STA_NULL;
			break;
		case STA_END0:
			if(val == END1){
				state = STA_NULL;
			}
			else
				state = STA_NULL;
			break;
		default:
			state = STA_NULL;
			break;
	}
			
	
		
}