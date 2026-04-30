#include <Uart.h>
#include <string.h>
#include "Arduino.h"

#define BAUDRATE(BAUD) (((F_CPU/(BAUD*16UL)))-1)
#define RXEN0 4
#define TXEN0 3

void Uart::init() {
    // TODO: 1. configure UART
    // set UART pin directions
    DDRD |= (1 << PB1); // TX pin as output
    DDRD &= ~(1 << PB2); // RX pin as input
    // set baud rate 9600
    // enable RX and TX
    UBRR0H = (unsigned char)BAUDRATE(9600) >> 8;
    UBRR0L = (unsigned char)BAUDRATE(9600);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0); // enable RX and TX
    // set frame format: 8 bits, 1 stop bit, no parity
    UCSR0C = (0<<USBS0)|(3<<UCSZ00);
    // DO NOT CHANGE ALL BITS

}

void Uart::writeByte(const char& d) {
    // TODO: 2. implement write + test it and capture on oscilloscope

    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = d;

}

bool Uart::available() {
    // TODO: 3. check if are data in UART buffer
        if (UCSR0A & (1<<RXC0))
            return true;
    return false;
}

char Uart::readByte() {
    // TODO: 3. implement read

    // wait for data
        while (!(UCSR0A & (1<<RXC0)));
        // get and return received data from buffer
        return UDR0;
    // read data and return it
}

void Uart::writeString(const char* msg) {
    for (int i = 0; i < strlen(msg); i++)
        writeByte(msg[i]);
}

void Uart::writeIntegerNumber(const int &number, const int &base) {

    char buffer[20];
    itoa(number, buffer, base);
    writeString(buffer);
    
}
