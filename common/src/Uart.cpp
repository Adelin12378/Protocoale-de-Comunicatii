#include <Uart.h>
#include <string.h>
#include "Arduino.h"
// ======================================================
// IMPLEMENTARE UART - Uart.cpp
// ======================================================
// ======================================================
// MACRO BAUD RATE
// ======================================================
// calculează valoarea pentru baud rate
#define BAUDRATE(BAUD) (((F_CPU/(BAUD*16UL)))-1)
// ======================================================
// BIȚI UART
// ======================================================
// RXEN0 = enable recepție UART
#define RXEN0 4
// TXEN0 = enable transmisie UART
#define TXEN0 3
// ======================================================
// INITIALIZARE UART
// ======================================================
void Uart::init() {
    // ==================================================
    // UART hardware folosește:
    //
    // PB2 = RX (port 10)
    // PB1 = TX (port 9)
    // ==================================================
    // ==================================================
    // DDRB = configurare direcție pini
    // ==================================================
    DDRD |= (1 << PB1); // TX pin as output
    DDRD &= ~(1 << PB2); // RX pin as input
    // set baud rate 9600
    // enable RX and TX
    // ==================================================
    // UBRR0H și UBRR0L
    // UART Baud Rate Registers
    // configurează viteza UART
    // ==================================================
    UBRR0H = (unsigned char)BAUDRATE(9600) >> 8;
    UBRR0L = (unsigned char)BAUDRATE(9600);
    // ==================================================
    // UCSR0B = UART Control and Status Register B
    // activează:
    // - recepția UART
    // - transmisia UART
    // ==================================================
    UCSR0B = (1<<RXEN0)|(1<<TXEN0); // enable RX and TX
    // set frame format: 8 bits, 1 stop bit, no parity
    // ==================================================
    // UCSR0C = UART Control and Status Register C
    // configurează formatul cadrului:
    // - 8 biți date
    // - 1 stop bit
    // - fără paritate
    // ==================================================
    UCSR0C = (0<<USBS0)|(3<<UCSZ00);
    // DO NOT CHANGE ALL BITS

}
// ======================================================
// TRANSMITERE BYTE UART
// ======================================================
void Uart::writeByte(const char& d) {
    // ==================================================
    // UCSR0A = UART Status Register
    // UDRE0 = UART Data Register Empty
    // 1 => buffer liber pentru transmitere
    // ==================================================
    // TODO: 2. implement write + test it and capture on oscilloscope
    while (!(UCSR0A & (1<<UDRE0)));
    // ==================================================
    // UDR0 = UART Data Register
    // scrierea în UDR0 transmite caracterul
    // ==================================================
    UDR0 = d;
}
// ======================================================
// VERIFICARE DATE DISPONIBILE
// ======================================================
bool Uart::available() {
    // TODO: 3. check if are data in UART buffer
    // ==================================================
    // RXC0 = Receive Complete
    // 1 => caracter primit complet
    // ==================================================
        if (UCSR0A & (1<<RXC0))
            return true;
    return false;
}
// ======================================================
// CITIRE BYTE UART
// ======================================================
char Uart::readByte() {
    // TODO: 3. implement read

    // wait for data
        while (!(UCSR0A & (1<<RXC0)));
        // get and return received data from buffer
        return UDR0;  // citește caracterul primit
    // read data and return it
}
// ======================================================
// TRANSMITERE STRING
// ======================================================
void Uart::writeString(const char* msg) {
    // transmite fiecare caracter din string
    for (int i = 0; i < strlen(msg); i++)
        writeByte(msg[i]);
}
// ======================================================
// TRANSMITERE NUMĂR
// ======================================================
void Uart::writeIntegerNumber(const int &number, const int &base) {
    // buffer pentru conversie număr -> text
    char buffer[20];
    // convertește numărul în string
    itoa(number, buffer, base);
    // transmite stringul
    writeString(buffer);
    
}
