#ifndef I_ADXL_H
#define I_ADXL_H

#include <IAdxlTransport.h>


class Adxl {
private:
    // ==================================================
    // pointer către interfața de transport
    // ==================================================
    IAdxlTransport* adxlTrans;
public:
    // ==================================================
    // constructor
    // primește implementarea transportului
    // ==================================================
    Adxl(IAdxlTransport* adxlTrans);
    // ==================================================
    // inițializare senzor
    // =================================================
    void init();
    // ==================================================
    // pornește măsurarea
    // ==================================================
    void start();
    // ==================================================
    // verifică dacă senzorul răspunde
    // ==================================================
    bool isConnected();
    // ==================================================
    // citește X Y Z
    // ==================================================
    void dataReader(int &X, int &Y, int &Z);
};

#endif