// Objectives:
// 1. configure I2C
// 2. comupaction over I2C
// 3. ADXL345 usage

// Components
// ADXL345 module
// Bidirectional level shifter

// Docs
// ADXL345 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include <I2cMaster.h>
#include <Uart.h>

#define ADX345_I2C_ADDR 0x53

I2cMaster wire;
Uart uart;
char id;

void setup() {
    // initialize GDB stub

    uart.init();
    wire.init();


    delay(500);
    // TODO: 2. read ADXL DEVICE ID
    wire.sendStart();
    wire.writeAddrWrite(0x53);
    wire.writeByte(0x00);
    wire.sendStart();
    wire.writeAddrRead(0x53);
    char id;
    wire.readByte(id);
    wire.sendStop();
    uart.writeIntegerNumber(id, 16);
    
    // expected response 0xE5
    
    delay(500);
    
    // TODO: 3. enable ADXL read
    wire.sendStart();
    wire.writeAddrWrite(0x53);
    wire.writeByte(0x2D);
    wire.writeByte(1 << 3);
    wire.sendStop();
    
    delay(500);
}

void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    char d[6];
    int X, Y, Z;
    wire.sendStart();
    wire.writeAddrWrite(0x53);
    wire.writeByte(0x32);
    wire.sendStart();
    wire.writeAddrRead(0x53);
    for (int i = 0; i < 6; i = i + 2){
        wire.readByte(d[i + 1]);
        wire.readByte(d[i]);
    }

    wire.sendStop();
    
    X = *(int*)& d[0];
    Y = *(int*)& d[2];
    Z = *(int*)& d[4];

    uart.writeIntegerNumber(X, 10);
    uart.writeIntegerNumber(Y, 10);
    uart.writeIntegerNumber(Z, 10);
    
}

void loop() {
    ADXL_readData();
    delay(500);
}
