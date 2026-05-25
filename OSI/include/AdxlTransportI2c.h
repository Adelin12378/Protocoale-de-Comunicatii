#ifndef I_ADXL_TRANSPORT_I2C_H
#define I_ADXL_TRANSPORT_I2C_H

#include <IAdxlTransport.h>
#include <I2cMaster.h>

class AdxlTransportI2c : public IAdxlTransport {

public:
    // ==================================================
    // inițializează I2C
    // ==================================================
    void init();
    // ==================================================
    // citește date din registru ADXL
    // ==================================================
    bool read(int addr, char* d, int len = 1) override;
    // ==================================================
    // scrie date în registru ADXL
    // ==================================================
    bool write(int addr, const char* d, int len = 1) override;
private:
    // ==================================================
    // obiect I2C master
    // ==================================================
    I2cMaster wire;
};

#endif