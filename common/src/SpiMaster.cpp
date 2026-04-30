#include <SpiMaster.h>
#include "Arduino.h"

void SpiMaster::init() {
    // TODO: 1. configure SPI
    /* ---!VERY IMPORTANT!--- */
    // write 1 on CS after set CS as OUTPUT
    
    DDRB |= (1 << PB2);
    PORTB |= (1 << 2);

    // in the end configure SPI
    // set MOSI, SCK - output
    DDRB |= (1 << PB3);
    DDRB |= (1 << PB5);
    // set MISO - input
    DDRB &= ~(1 << PB4);
    // enable SPI as master
    SPCR |= (1 << 4) | (1 << 6) | (1 << 2) | (1 << 1) | (1 << 3);
    // check ADXL datasheet for clock frequency
    // check ADXL datasheet for clock polarity
    // check ADXL datasheet for clock phase
}

char SpiMaster::transmit(const char& d) {
    // TODO: 2. transmit 1 byte on SPI
    // start transmission
    SPDR = d;
    // wait for transmission complete
    while (!(SPSR & (1<<SPIF)));
    // return response from slave
    return SPDR;
}