// Bidirectional level shifter
// ======================================================
// APLICAȚIE I2C + ADXL345
// ======================================================
// Obiective:
// 1. configurare I2C
// 2. comunicație I2C
// 3. citire accelerometru ADXL345
// ======================================================

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include <I2cMaster.h>
#include <Uart.h>
// ======================================================
// ADRESA I2C ADXL345
// ======================================================
#define ADX345_I2C_ADDR 0x53
// ======================================================
// OBIECTE
// ======================================================
I2cMaster wire;
Uart uart;
char id;
// ======================================================
// SETUP
// ======================================================
void setup() {
    // initialize GDB stub
    uart.init(); // initializează UART
    wire.init(); // initializează I2C
    delay(500);
    // ==================================================
    // CITIRE DEVICE ID DIN ADXL345
    // ==================================================
    // TODO: 2. read ADXL DEVICE ID
    wire.sendStart(); // START
    // trimite adresa slave + WRITE
    wire.writeAddrWrite(0x53);
    // ==================================================
    // registrul 0x00 = DEVICE ID
    // ==================================================
    wire.writeByte(0x00);
    wire.sendStart(); // repeated START
    wire.writeAddrRead(0x53); // trimite adresa slave + READ
    char id;
    wire.readByte(id); // citește ID-ul
    wire.sendStop(); // STOP
    // trimite ID-ul pe UART în HEX
    uart.writeIntegerNumber(id, 16);
    // expected response 0xE5
    delay(500);
    // ==================================================
    // ACTIVARE MĂSURARE ADXL345
    // ==================================================
    wire.sendStart();
    // slave + write
    wire.writeAddrWrite(0x53);
    // ==================================================
    // registrul 0x2D = POWER_CTL
    // ==================================================
    wire.writeByte(0x2D);
    // ==================================================
    // setează bitul Measure = 1
    // (1 << 3)
    // ==================================================   
    wire.writeByte(1 << 3);
    wire.sendStop();
    delay(500);
}
// ======================================================
// FUNCȚIE CITIRE X Y Z
// ======================================================
void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    // ==================================================
    // fiecare axă:
    // 16 biți
    // X0 X1
    // Y0 Y1
    // Z0 Z1
    // ==================================================
    char d[6];
    int X, Y, Z;
    wire.sendStart(); // START
    wire.writeAddrWrite(0x53); // slave + write
    // ==================================================
    // registrul 0x32 = DATAX0
    // de aici începe citirea:
    // X Y Z
    // ==================================================
    wire.writeByte(0x32);
    wire.sendStart(); // repeated START
    wire.writeAddrRead(0x53); // slave + read
    // ==================================================
    // citește 6 bytes: X0 X1 Y0 Y1 Z0 Z1
    // ==================================================
    for (int i = 0; i < 6; i = i + 2){
        wire.readByte(d[i + 1]);
        wire.readByte(d[i]);
    }
    wire.sendStop(); // STOP
    // ==================================================
    // reconstruiește valorile pe 16 biți
    // ==================================================
    X = *(int*)& d[0];
    Y = *(int*)& d[2];
    Z = *(int*)& d[4];
    // ==================================================
    // trimite valorile pe UART
    // ==================================================
    uart.writeIntegerNumber(X, 10);
    uart.writeIntegerNumber(Y, 10);
    uart.writeIntegerNumber(Z, 10);
    
}
void loop() {
    ADXL_readData(); // citește accelerațiile
    delay(500);
}
