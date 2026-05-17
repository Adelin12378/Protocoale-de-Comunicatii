#include "AdxlTransportSpi.h"

bool AdxlTransportSpi::read(int addr, char* d, int len = 1){
    PORTB &= ~(1 << PB0); //enable
    spi.transmit(ADXL_cmdBuilder(addr, 0x03, 1));
    
    for (int i = 0; i < len; i++)
        d[i] = spi.transmit(0x00);

    PORTB |= (1 << PB0); //disable
}

bool AdxlTransportSpi::write(int addr, const char* d, int len = 1){
    PORTB &= ~(1 << PB0); //enable

    spi.transmit(addr);

    for (int i = 0; i < len; i++) {
        spi.transmit(d[i]);
    }
    
    PORTB |= (1 << PB0); //disable
}

char AdxlTransportSpi::ADXL_cmdBuilder(char addr, bool rw, bool mb) {
    return (addr & 0b00111111) | (mb << 6) | (rw << 7);
}