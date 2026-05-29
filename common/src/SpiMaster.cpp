#include <SpiMaster.h>
#include "Arduino.h"
// ======================================================
// INITIALIZARE SPI
// ======================================================
void SpiMaster::init() {
    // TODO: 1. configure SPI
    /* ---!VERY IMPORTANT!--- */
    // write 1 on CS after set CS as OUTPUT
    // ==================================================
    // SPI hardware pe ATmega328P folosește:
    //
    // PB2 = SS   (Slave Select) PORT 10    
    // PB3 = MOSI (Master Out Slave In) PORT 11
    // PB4 = MISO (Master In Slave Out) POER 12
    // PB5 = SCK  (Serial Clock) PORT 13
    // ==================================================
    // ==================================================
    // DDRB = Data Direction Register B
    // stabilește direcția pinilor:
    // 0 = INPUT
    // 1 = OUTPUT
    // ==================================================
    // ==================================================
    // configurează PB2 (SS) ca OUTPUT
    // IMPORTANT:
    // dacă SS devine INPUT și LOW,
    // hardware-ul dezactivează modul MASTER
    // ==================================================
    DDRB |= (1 << PB2);
    // ==================================================
    // PORTB = registru ieșire pentru PORTB
    // pune SS pe HIGH
    // niciun slave selectat
    // ==================================================
    PORTB |= (1 << 2);
    // ==================================================
    // configurează MOSI ca OUTPUT
    // PB3 = MOSI
    // ==================================================
    DDRB |= (1 << PB3);
    // ==================================================
    // configurează SCK ca OUTPUT
    // PB5 = SPI Clock
    // ==================================================
    DDRB |= (1 << PB5);
    // ==================================================
    // configurează MISO ca INPUT
    // PB4 = Master In Slave Out
    // ==================================================
    DDRB &= ~(1 << PB4);
    // enable SPI as master
    // ==================================================
    // SPCR = SPI Control Register
    // configurează:
    // SPE  = enable SPI
    // MSTR = Master mode
    // SPR1/SPR0 = prescaler clock
    // CPOL = clock polarity
    // CPHA = clock phase
    // ==================================================
    // (1 << 6) => SPE
    // activează SPI hardware
    // (1 << 4) => MSTR
    // configurează SPI în modul MASTER
    // (1 << 1) și (1 << 0)
    // configurează viteza SPI
    // (1 << 3) => CPOL
    // configurează polaritatea clockului
    // (1 << 2) => CPHA
    // configurează faza clockului
    // =================================================
    SPCR |= (1 << 4) | (1 << 6) | (1 << 2) | (1 << 1) | (1 << 3);
    // check ADXL datasheet for clock frequency
    // check ADXL datasheet for clock polarity
    // check ADXL datasheet for clock phase
    // ==================================================
    // FRECVENȚA SPI
    // Formula:
    // fSPI = F_CPU / Prescaler
    // unde:
    // F_CPU = 16MHz
    // SPR1 = 1
    // SPR0 = 1
    // Prescaler = 128
    // rezultă:
    // fSPI = 16000000 / 128
    // fSPI = 125kHz
    // ==================================================
    // ==================================================
    // CPOL și CPHA
    // stabilesc:
    // - nivelul idle al clockului
    // - pe ce front se citesc datele
    // ==================================================
}
// ======================================================
// TRANSMITERE BYTE SPI
// ======================================================
char SpiMaster::transmit(const char& d) {
    // TODO: 2. transmit 1 byte on SPI
    // start transmission
    // ==================================================
    // SPDR = SPI Data Register
    // scrierea în SPDR:
    // - începe transmisia SPI
    // - transmite byte-ul către slave
    // ==================================================
    SPDR = d;
     // ==================================================
    // SPDR = SPI Data Register
    // scrierea în SPDR:
    // - începe transmisia SPI
    // - transmite byte-ul către slave
    // ==================================================
    // wait for transmission complete
    while (!(SPSR & (1<<SPIF)));
    // return response from slave
    // ==================================================
    // SPI transmite și recepționează simultan
    // SPDR conține:
    // răspunsul primit de la slave
    // ==================================================
    return SPDR;
}