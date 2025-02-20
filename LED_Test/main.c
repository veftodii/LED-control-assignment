#include <avr/io.h>

#define sysOscilatorClockFrequency 14745600 // Hz
#define USARTBaudRate 9600  // bps

void USARTInit(unsigned int UBRRValue) {
	// Use default frame format (8N1)
	// Set baud rate
	UBRR0H = ((unsigned char) (UBRRValue >> 8)) & 0x0F;  // bits 12..15 are system reserved and must be written to zero
	UBRR0L = (unsigned char) UBRRValue;
	// Enable receiver
	UCSR0B = (1 << RXEN0);
}

unsigned char USARTReceive() {
	unsigned char rxData = 0x00;
	
	// Check for data
	if (UCSR0A & (1 << RXC0)) {
		rxData = UDR0;
	}

	// Return received data
	return rxData;
}

int main() {
	unsigned char rxData = 0x00;
	
	// Set PortD Pin7 as output
	DDRD |= (1 << DDD7);

	// Initialize USART communication
	USARTInit(sysOscilatorClockFrequency/16/USARTBaudRate - 1);
	
    while (1) {
		rxData = USARTReceive();
		
		// LED commands: '0' = Turn off, '1' = Turn on
		// Preserve LED state if the command is unknown
		if (rxData & 0x31) {
			PORTD |= (1 << PORTD7);  // Turn LED on
		} 
		else if (rxData & 0x30) {
			PORTD &= ~(1 << PORTD7);  // Turn LED off
		}
    }
	
	return 0;
}