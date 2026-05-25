#include "AdxlTransportSpi.h"
// ======================================================
// INITIALIZARE SPI
// ======================================================
void AdxlTransportSpi::init()
{
    spi.init();
}
// ======================================================
// CITIRE DATE SPI
// ======================================================
bool AdxlTransportSpi::read(int addr, char* d, int len = 1){
    // ==================================================
    // SPI hardware pe ATmega328P:
    // PB0 = CS
    // PB3 = MOSI
    // PB4 = MISO
    // PB5 = SCK
    // ==================================================
    // ==================================================
    // PORTB controlează:
    // PB0 = Chip Select
    // LOW => enable slave
    // ==================================================
    PORTB &= ~(1 << PB0); //enable
    // ==================================================
    // construiește comandă READ
    // ==================================================
    spi.transmit(ADXL_cmdBuilder(addr, 0x03, 1));
    // ==================================================
    // citește len bytes
    // ==================================================
    for (int i = 0; i < len; i++)
        d[i] = spi.transmit(0x00);
    // ==================================================
    // HIGH => disable slave
    // ==================================================
    PORTB |= (1 << PB0); //disable
}
// ======================================================
// SCRIERE DATE SPI
// ======================================================
bool AdxlTransportSpi::write(int addr, const char* d, int len = 1){
    PORTB &= ~(1 << PB0); // enable slave
    // transmite adresa registrului
    spi.transmit(addr);
    // transmite datele
    for (int i = 0; i < len; i++) {
        spi.transmit(d[i]);
    }
    PORTB |= (1 << PB0);  // disable slave
}
// ======================================================
// CONSTRUIRE COMANDĂ SPI
// ======================================================
char AdxlTransportSpi::ADXL_cmdBuilder(char addr, bool rw, bool mb) {
    // ==================================================
    // FORMAT COMANDĂ ADXL345
    // bit7 = R/W
    // 0 = write
    // 1 = read
    // bit6 = MB
    // 0 = single byte
    // 1 = multi byte
    // bit5..0 = adresă registru
    // ==================================================
    // ==================================================
    // Formula:
    // (addr & 0b00111111)
    // păstrează doar biții adresei
    // (mb << 6)
    // mută bitul multi-byte pe bit6
    // (rw << 7)
    // mută bitul read/write pe bit7
    // ==================================================
    return (addr & 0b00111111) | (mb << 6) | (rw << 7);
}