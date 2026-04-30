#include <I2cMaster.h>
#include <Arduino.h>

void I2cMaster::_writeByte(const char& data) {
    // TODO: 2. send 1 byte on I2C
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    // write byte
    // send command to send byte
    // wait for ack or nack
    while (!(TWCR & (1<<TWINT)));
}

void I2cMaster::_readByte(char& data) {
    // TODO: 2. read 1 byte from I2C
    TWCR = (1<<TWINT) | (1<<TWEN);
    // send command to read byte
    // wait for ack or nack
    while (!(TWCR & (1<<TWINT)));
    data = TWDR;
    // read byte
}

void I2cMaster::init() {
    // TODO: 1. configure SPI for master mode

    // set frequency; read datasheet for SCLK frequency
    TWSR = 0x00;
    TWBR = 24;
    // set SCLK pin output
    // enable internal pull up resistor for SCLK and SDA, set PORTxn = 1
    PORTC |= (1 << PORTC4) | (1 << PORTC5);
    
    TWCR = (1 << TWEN);

}

void I2cMaster::sendStart() {
    // TODO: 2. send I2C start condition
    // send start condition
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    // wait for start has been transmitted
    while (!(TWCR & (1<<TWINT)));
}

bool I2cMaster::writeAddrWrite(const char& addr) {
    // TODO: 2. send slave addr for write
    // add write bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1)); 
    
    return (TWSR & 0xF8) == 0x18;
}

bool I2cMaster::writeAddrRead(const char& addr) {
    // TODO: 2. send slave addr for read
    // add read bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1) | 0x01);
    
    return (TWSR & 0xF8) == 0x40;
}

bool I2cMaster::writeByte(const char& d) {
    // TODO: 2. send byte on I2C
    // send byte
    _writeByte(d);
    
    return (TWSR & 0xF8) == 0x40;
}

bool I2cMaster::readByte(char& d) {
    // TODO: 2. read byte from I2C
    _readByte(d);
    // retrun if ack received
    return (TWSR & 0xF8) == 0x50;
}

void I2cMaster::sendStop() {
    // TODO: 2. send I2C stop condition
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}