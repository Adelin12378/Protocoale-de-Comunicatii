#ifndef I_ADXL_TRANSPORT_SPI_H
#define I_ADXL_TRANSPORT_SPI_H

#include <IAdxlTransport.h>
#include <SpiMaster.h>
#include <Uart.h>
#include "Arduino.h"

class AdxlTransportSpi : public IAdxlTransport {

public: 
    bool read(int addr, char* d, int len = 1) override;

    bool write(int addr, const char* d, int len = 1) override;

    char ADXL_cmdBuilder(char addr, bool rw, bool mb);

private:
    Uart uart;
    SpiMaster spi;
};

#endif