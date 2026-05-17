#include "AdxlTransportI2c.h"

bool AdxlTransportI2c::read(int addr, char* d, int len = 1){
    wire.sendStart();
    wire.writeAddrWrite(0x53);
    wire.writeByte(addr);
    wire.sendStart();
    wire.writeAddrRead(0x53);
    
    for (int i = 0; i < len; i++){
        wire.readByte(d[i]);
    }

    wire.sendStop();
}

bool AdxlTransportI2c::write(int addr, const char* d, int len = 1){
    wire.sendStart();
    wire.writeAddrWrite(0x53);
    wire.writeByte(addr);
    wire.sendStart();
    wire.writeAddrRead(0x53);

    for (int i = 0; i < len; i += 2){
        wire.writeByte(d[i]);
    }

    wire.sendStop();
}