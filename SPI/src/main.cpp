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
// ======================================================
// SELECTARE ADXL345
// ======================================================
void ADXL_enable() {
    // TODO: 2. select ADXL
    // ==================================================
    // PB0 folosit ca CS (Chip Select)
    // activ LOW:
    // LOW  => slave selectat
    // HIGH => slave deselectat
    // ==================================================
    PORTB &= ~(1 << PB0);
}
// ======================================================
// DESELECTARE ADXL345
// ======================================================
void ADXL_disable() {
    // TODO: 2. deselect ADXL
    PORTB |= (1 << PB0); // pune CS pe HIGH
}
// ======================================================
// CONSTRUIRE COMANDĂ SPI
// ======================================================
char ADXL_cmdBuilder(char addr, bool rw, bool mb) {
    // ==================================================
    // FORMAT COMANDĂ ADXL345
    // bit7 = R/W
    // 0 = WRITE
    // 1 = READ
    // bit6 = MB
    // 0 = single byte
    // 1 = multiple byte
    // bit5..0 = adresă registru
    // ==================================================
    return (addr & 0b00111111) | (mb << 6) | (rw << 7);
}
// ======================================================
// SETUP
// ======================================================
void setup() {
    // initialize GDB stub
    // ==================================================
    // configurează PB0 ca OUTPUT
    // PB0 = Chip Select pentru ADXL
    // ==================================================
    DDRB |= 1 << 0;
    // TODO: 1. set an CS pin and make it as output
    ADXL_disable(); // deselectează ADXL
    spi.init(); // initializează SPI
    uart.init(); // initializează UART
    // TODO: 2. read ADXL DEVICE ID
    // analyze SPI protocol on oscilloscope
    // expected response 0xE5
    // ==================================================
    // CITIRE DEVICE ID
    // ==================================================
    ADXL_enable(); // selectează ADXL
    // ==================================================
    // 0x80 = READ bit = 1
    // adresă = 0x00
    // registrul 0x00 = DEVICE ID
    // ==================================================
    spi.transmit(0x80); 
    // transmite byte dummy
    // SPI primește date simultan cu transmisia
    char id = spi.transmit(0x00);
    ADXL_disable(); // deselectează ADXL
    uart.writeIntegerNumber(id, 16); // trimite ID-ul pe UART
    // ==================================================
    // valoare așteptată:
    // 0xE5
    // ==================================================
    // ==================================================
    // ACTIVARE MĂSURARE
    // ==================================================
    ADXL_enable();
    // ==================================================
    // registrul 0x2D = POWER_CTL
    // ==================================================
    spi.transmit(0x2D);
    // ==================================================
    // (1 << 3)
    // setează bitul Measure = 1
    // ==================================================
    spi.transmit(1 << 3);
    ADXL_disable();
}
// ======================================================
// CITIRE X Y Z
// ======================================================
void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    int data[6];
    int X, Y, Z;
    ADXL_enable(); // selectează ADXL
    // ==================================================
    // 0xF2 =
    // 11110010
    // bit7 = 1 => READ
    // bit6 = 1 => MULTI BYTE
    // adresă = 0x32
    // 0x32 = DATAX0
    // ==================================================
    spi.transmit(0xF2);
    // ==================================================
    // citește:
    // X0 X1
    // Y0 Y1
    // Z0 Z1
    // ==================================================
    for (int i = 0; i < 6; i = i + 2){
        data[i + 1] = spi.transmit(0x00);
        data[i] = spi.transmit(0x00);
    }
    // ==================================================
    // reconstruiește valorile pe 16 biți
    // ==================================================
    X = *(int*)& data[0];
    Y = *(int*)& data[2];
    Z = *(int*)& data[4];
    ADXL_disable(); // deselectează ADXL
    // ==================================================
    // trimite valorile către PC
    // ==================================================
    uart.writeIntegerNumber(X, 16); 
    uart.writeIntegerNumber(Y, 16); 
    uart.writeIntegerNumber(Z, 16); 
}

void loop() {
    ADXL_readData(); // citește accelerațiile
    delay(500);
}