// Objectives:
// 1. configure SPI
// 2. comupaction over SPI
// 3. ADXL345 usage

// Components
// ADXL345 module
// Bidirectional level shifter

// Docs
// ADXL345 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf


// HINTS
// use UART lib from lab2 to send messages to PC
// use itoa to convert int to string

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include <SpiMaster.h>
#include <Uart.h>

SpiMaster spi;

Uart uart;

void ADXL_enable() {
    // TODO: 2. select ADXL
    PORTB &= ~(1 << PB0);
}

void ADXL_disable() {
    // TODO: 2. deselect ADXL
    PORTB |= (1 << PB0);
}

char ADXL_cmdBuilder(char addr, bool rw, bool mb) {
    return (addr & 0b00111111) | (mb << 6) | (rw << 7);
}

void setup() {
    // initialize GDB stub
    
    DDRB |= 1 << 0;

    // TODO: 1. set an CS pin and make it as output
    ADXL_disable();

    spi.init();
    uart.init();

    // TODO: 2. read ADXL DEVICE ID
    // analyze SPI protocol on oscilloscope
    // expected response 0xE5

    ADXL_enable();
    spi.transmit(0x80);
    char id = spi.transmit(0x00);
    ADXL_disable();
    
    uart.writeIntegerNumber(id, 16);

    ADXL_enable();

    spi.transmit(0x2D);
    spi.transmit(1 << 3);

    ADXL_disable();
    
    
}

void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    int data[6];
    int X, Y, Z;
    ADXL_enable();
    spi.transmit(0xF2);
    data[1] = spi.transmit(0x00);
    data[0] = spi.transmit(0x00);
    data[3] = spi.transmit(0x00);
    data[2] = spi.transmit(0x00);
    data[5] = spi.transmit(0x00);
    data[4] = spi.transmit(0x00);
    ADXL_disable();
    uart.writeIntegerNumber(data[1], 16); 
}

void loop() {
    ADXL_readData();
    delay(500);
}