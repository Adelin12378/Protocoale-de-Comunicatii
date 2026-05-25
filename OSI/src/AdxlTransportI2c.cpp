#include "AdxlTransportI2c.h"
// ======================================================
// INITIALIZARE I2C
// ======================================================
void AdxlTransportI2c::init()
{
    wire.init();
}
// ======================================================
// CITIRE DATE PRIN I2C
// ======================================================
bool AdxlTransportI2c::read(int addr, char *d, int len = 1)
{
    // ==================================================
    // I2C pe ATmega328P folosește:
    // PC4 = SDA
    // PC5 = SCL
    // ==================================================
    wire.sendStart(); // START
    // ==================================================
    // adresa slave ADXL345 = 0x53
    // ==================================================
    wire.writeAddrWrite(0x53);
    wire.writeByte(addr); // trimite adresa registrului
    wire.sendStart(); // repeated START
    wire.writeAddrRead(0x53); // schimbă direcția pe READ
    // ==================================================
    // citește len bytes
    // ==================================================
    for (int i = 0; i < len; i++){
        wire.readByte(d[i]);
    }
    wire.sendStop(); // STOP
}
// ======================================================
// SCRIERE DATE PRIN I2C
// ======================================================
bool AdxlTransportI2c::write(int addr, const char* d, int len = 1){
    wire.sendStart(); // START
    wire.writeAddrWrite(0x53); // slave + write
    wire.writeByte(addr); // registru destinație
    wire.sendStart(); // repeated START
    wire.writeAddrRead(0x53); // READ mode
    // ==================================================
    // trimite date
    // ==================================================
    for (int i = 0; i < len; i += 2){
        wire.writeByte(d[i]);
    }
    wire.sendStop(); // STOP
}