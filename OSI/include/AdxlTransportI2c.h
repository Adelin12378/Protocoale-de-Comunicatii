#ifndef I_ADXL_TRANSPORT_I2C_H
#define I_ADXL_TRANSPORT_I2C_H

#include <IAdxlTransport.h>
#include <I2cMaster.h>
#include <Uart.h>

class AdxlTransportI2c : public IAdxlTransport {

public:
    bool read(int addr, char* d, int len = 1) override;

    bool write(int addr, const char* d, int len = 1) override;

private:
    Uart uart;
    I2cMaster wire;
};

#endif