#ifndef I_ADXL_TRANSPORT_SPI_H
#define I_ADXL_TRANSPORT_SPI_H

#include <IAdxlTransport.h>
#include <SpiMaster.h>
#include "Arduino.h"

class AdxlTransportSpi : public IAdxlTransport {

public: 
    // ==================================================
    // inițializează SPI
    // ==================================================
    void init();
    // ==================================================
    // citește date din ADXL
    // ==================================================
    bool read(int addr, char* d, int len = 1) override;
    // ==================================================
    // scrie date în ADXL
    // ==================================================
    bool write(int addr, const char* d, int len = 1) override;
    // ==================================================
    // construiește comandă SPI ADXL
    // ==================================================
    char ADXL_cmdBuilder(char addr, bool rw, bool mb);
private:
    // ==================================================
    // obiect SPI master
    // ==================================================
    SpiMaster spi;
};

#endif