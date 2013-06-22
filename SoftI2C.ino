#include <Arduino.h>
#include "SoftwareI2C.h"
#include "MPR121LVC.h"
#define ADDRESS 0x5a

SoftwareI2C SoftI2C1;

//SoftwareI2C SoftI2C2;

void setup()
{
    Serial.begin(38400);
    Serial.println("init over");

    SoftI2C1.begin(3, 2);           // device 1
    
    delay(1000);
    
    delay(100);
}

unsigned char dada = 0;
void loop()
{
    unsigned char tmp1 = 0;
    unsigned char tmp2 = 0;
    //Serial.print("I2C1:\t");

    SoftI2C1.getDtaMpr121(&tmp1, &tmp2);
    if(tmp1 > 0 || tmp2 > 0)
    {
        Serial.print("device 1:\t");
        Serial.print(tmp1);
        Serial.print("\t");
        Serial.print(tmp2);
        Serial.println();
    }
    delay(50);

}
