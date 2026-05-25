#include <I2cMaster.h>
#include <Arduino.h>
// ======================================================
// IMPLEMENTARE I2C MASTER - ATmega328P
// ======================================================
// Protocol folosit:
// I2C (Inter-Integrated Circuit)
// Dispozitiv folosit:
// ADXL345 Accelerometer
// Microcontroller:
// ATmega328P
// ======================================================
// ======================================================
// FUNCȚIE TRIMITERE BYTE PE I2C
// ======================================================
void I2cMaster::_writeByte(const char& data) {
    // TODO: 2. send 1 byte on I2C
    // ==================================================
    // TWDR = Two Wire Data Register
    // registru folosit pentru:
    // - transmiterea datelor I2C
    // - recepția datelor I2C
    // caracterul este încărcat în bufferul hardware I2C
    // ==================================================
    TWDR = data;
    // ==================================================
    // TWCR = Two Wire Control Register
    // TWINT = flag operație completă
    // TWEN  = enable I2C hardware
    // scrierea acestor biți pornește transmisia
    // ==================================================
    TWCR = (1<<TWINT) | (1<<TWEN);
    // write byte
    // send command to send byte
    // wait for ack or nack
    // ==================================================
    // așteaptă până transmisia s-a terminat
    // hardware-ul setează TWINT = 1
    // când operația este completă
    // ==================================================
    while (!(TWCR & (1<<TWINT)));
}
// ======================================================
// FUNCȚIE CITIRE BYTE I2C
// ======================================================
void I2cMaster::_readByte(char& data) {
    // TODO: 2. read 1 byte from I2C
    // ==================================================
    // pornește recepția unui byte
    // ==================================================
    TWCR = (1<<TWINT) | (1<<TWEN);
    // send command to read byte
    // wait for ack or nack
    // ==================================================
    // așteaptă finalizarea recepției
    // ==================================================
    while (!(TWCR & (1<<TWINT)));
    // ==================================================
    // citește byte-ul primit
    // TWDR conține datele recepționate
    // ==================================================
    data = TWDR;
    // read byte
}
// ======================================================
// INITIALIZARE I2C MASTER
// ======================================================
void I2cMaster::init() {
    // TODO: 1. configure SPI for master mode
    // ==================================================
    // I2C hardware pe ATmega328P folosește:
    // PC4 = SDA (date)
    // PC5 = SCL (clock)
    // ==================================================
    // ==================================================
    // TWSR = Two Wire Status Register
    // conține:
    // - status I2C
    // - prescaler I2C
    // prescaler = 1
    // ==================================================
    // set frequency; read datasheet for SCLK frequency
    TWSR = 0x00;
    // ==================================================
    // TWBR = Two Wire Bit Rate Register
    // stabilește frecvența SCL
    // ==================================================
    TWBR = 24;
     // ==================================================
    // FORMULA FRECVENȚĂ I2C
    // SCL frequency = F_CPU / (16 + 2 * TWBR * Prescaler)
    // unde:
    // F_CPU = 16MHz
    // TWBR = 24
    // Prescaler = 1
    // Calcul:
    // SCL = 16000000 / (16 + 2 * 24 * 1)
    // SCL = 16000000 / 64
    // SCL = 250kHz
    // ==================================================
    // ==================================================
    // PORTC = registru ieșire pentru PORTC
    // PORTC4 = SDA
    // PORTC5 = SCL
    // pune liniile pe HIGH
    // activează rezistențele interne pull-up
    // ==================================================
    // set SCLK pin output
    // enable internal pull up resistor for SCLK and SDA, set PORTxn = 1
    PORTC |= (1 << PORTC4) | (1 << PORTC5);
    // ==================================================
    // TWCR = Two Wire Control Register
    // TWEN = enable hardware I2C
    // ==================================================
    TWCR = (1 << TWEN);

}
// ======================================================
// START CONDITION
// ======================================================
void I2cMaster::sendStart() {
    // TODO: 2. send I2C start condition
    // send start condition
    // ==================================================
    // TWSTA = transmite condiție START
    // START:
    // SDA trece LOW înainte de SCL
    // ==================================================
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    // așteaptă transmiterea START
    while (!(TWCR & (1<<TWINT)));
}
// ======================================================
// TRIMITERE ADRESĂ + WRITE
// ======================================================
bool I2cMaster::writeAddrWrite(const char& addr) {
    // TODO: 2. send slave addr for write
    // add write bit to addr {addr[6:0],r/w}
    // ==================================================
    // FORMAT CADRU I2C
    // addr[6:0] + R/W bit
    // R/W:
    // 0 = WRITE
    // 1 = READ
    // ==================================================
    // ==================================================
    // deplasează adresa cu 1 bit la stânga
    // ultimul bit devine:
    // 0 => WRITE
    // ==================================================
    _writeByte((addr << 1)); 
    // ==================================================
    // TWSR conține codul status I2C
    // masca 0xF8 elimină biții prescaler
    // 0x18 = SLA+W transmis și ACK primit
    // ==================================================
    return (TWSR & 0xF8) == 0x18;
}
// ======================================================
// TRIMITERE ADRESĂ + READ
// ======================================================
bool I2cMaster::writeAddrRead(const char& addr) {
    // TODO: 2. send slave addr for read
    // add read bit to addr {addr[6:0],r/w}
    // ==================================================
    // adaugă bitul READ = 1
    // exemplu:
    // 0x53 << 1 = 0xA6
    // 0xA6 | 1  = 0xA7
    // ==================================================
    _writeByte((addr << 1) | 0x01);
    // ==================================================
    // 0x40 =
    // SLA+R transmis și ACK primit
    // ==================================================
    return (TWSR & 0xF8) == 0x40;
}
// ======================================================
// TRIMITERE BYTE
// ======================================================
bool I2cMaster::writeByte(const char& d) {
    // TODO: 2. send byte on I2C
    // transmite byte
    _writeByte(d);
    // ==================================================
    // verifică ACK primit
    // ==================================================
    return (TWSR & 0xF8) == 0x40;
}
// ======================================================
// CITIRE BYTE
// ======================================================
bool I2cMaster::readByte(char& d) {
    // TODO: 2. read byte from I2C
    _readByte(d); // citește byte
    // retrun if ack received
    // ==================================================
    // 0x50 =
    // date primite și ACK trimis
    // ==================================================
    return (TWSR & 0xF8) == 0x50;
}
// ======================================================
// STOP CONDITION
// ======================================================
void I2cMaster::sendStop() {
    // TODO: 2. send I2C stop condition
    // ==================================================
    // TWSTO = transmite condiție STOP
    // STOP:
    // SDA trece HIGH după SCL
    // ==================================================
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}