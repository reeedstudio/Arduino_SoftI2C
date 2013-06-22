/*
  SoftwareI2C.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include "SoftwareI2C.h"

void SoftwareI2C::begin(int Sda, int Scl)
{
    pinSda = Sda;
    pinScl = Scl;

    pinMode(pinScl, OUTPUT);
    pinMode(pinSda, OUTPUT);
    digitalWrite(pinScl, HIGH);
    digitalWrite(pinSda, HIGH);
    delay(200);
    mpr121_setup();
}

void SoftwareI2C::sda_set(unsigned char ucData)
{
    pinMode(pinSda, OUTPUT);
    digitalWrite(pinSda, ucData);
}

void SoftwareI2C::scl_set(unsigned char ucData)
{
    digitalWrite(pinScl, ucData);
}

unsigned char SoftwareI2C::get_sda(void)
{
    pinMode(pinSda, INPUT);
    return digitalRead(pinSda);
}

void SoftwareI2C::i2c_delay(void)
{
    delayMicroseconds(1);
}


/*********************************************************************************************************
** Function name:           getAck
** Descriptions:            receive ack
*********************************************************************************************************/
unsigned char SoftwareI2C::getAck(void)
{
    unsigned int ntime = 50000;

    pinMode(pinSda, INPUT);
    digitalWrite(pinSda, HIGH);
    DELAY();
    scl_set(HIGH);
    // DELAY();
    while(ntime--)
    {
        if(!digitalRead(pinSda))                        // get ack
        {
            scl_set(LOW);                               // scl low
            DELAY();
            return GETACK;
        }
    }

    if(ntime<1)
    {
#if __Debug
        Serial.println("get nak");
#endif
    }

    scl_set(LOW);
    DELAY();
    return GETNAK;
}

/*********************************************************************************************************
** Function name:           sendAck
** Descriptions:            send ack
*********************************************************************************************************/
void SoftwareI2C::sendAck(void)
{
    sda_set(LOW);                                                       /* sda_set(LOW)                 */
    DELAY();
    scl_set(HIGH);                                                      /* scl_set(HIGH)                */
    DELAY();
    scl_set(LOW);                                                       /* scl_set(LOW)                 */
    DELAY();
}

/*********************************************************************************************************
** Function name:           sendNak
** Descriptions:            send nak
*********************************************************************************************************/
void SoftwareI2C::sendNak(void)
{
    sda_set(HIGH);                                                      /* sda_set(LOW)                 */
    DELAY();
    scl_set(HIGH);                                                      /* scl_set(HIGH)                */
    DELAY();
    scl_set(LOW);                                                       /* scl_set(LOW)                 */
    DELAY();
}

/*********************************************************************************************************
** Function name:           sendStart
** Descriptions:            send start
*********************************************************************************************************/
void SoftwareI2C::sendStart(void)
{
    scl_set(HIGH);DELAY();DELAY();DELAY();DELAY();                      /* scl =  1                     */
    sda_set(HIGH);DELAY();DELAY();DELAY();DELAY();                      /* sda  = 1                     */
    sda_set(LOW);DELAY();DELAY();DELAY();DELAY();                       /* sda_set(LOW);                */
    DELAY();DELAY();DELAY();DELAY();
    scl_set(LOW);DELAY();DELAY();DELAY();DELAY();                                             /* scl = 0;                     */
                                                                        /* delay_us_cfg(15);            */

}

/*********************************************************************************************************
** Function name:           sendStop
** Descriptions:            send stop
*********************************************************************************************************/
void SoftwareI2C::sendStop(void)
{
    scl_set(HIGH);DELAY();
    sda_set(LOW); DELAY();
    sda_set(HIGH);DELAY();
    scl_set(LOW);
    for(int i = 0; i<10; i++)
    DELAY();
}

/*********************************************************************************************************
** Function name:           sendStop
** Descriptions:            send stop
*********************************************************************************************************/
void SoftwareI2C::sendBit(unsigned char bit)
{
    if (bit & 0x80) {
        sda_set(HIGH);
    } else {
        sda_set(LOW);
    }
    DELAY();
    scl_set(HIGH);
    DELAY();
    scl_set(LOW);

}

/*********************************************************************************************************
** Function name:           sendByte
** Descriptions:            send a byte
*********************************************************************************************************/
unsigned char SoftwareI2C::revBit()
{
    unsigned  char  ucRt;
    DELAY();
    scl_set(HIGH);
    DELAY();
    ucRt = get_sda();
    scl_set(LOW);
    return  ucRt;

}

/*********************************************************************************************************
** Function name:           sendByte
** Descriptions:            send a byte
*********************************************************************************************************/
void SoftwareI2C::sendByte(unsigned char ucData)
{
    unsigned  char  i;

    i = 8;
    do {
        sendBit(ucData);
        ucData=ucData << 1;
    } while (--i != 0);

}

/*********************************************************************************************************
** Function name:           revByte
** Descriptions:            receive a byte
*********************************************************************************************************/
unsigned char SoftwareI2C::revByte()
{
    unsigned  char  ucRt;
    unsigned  char  i;

    //  receive data
    i = 8;
    do {
        ucRt  =  (ucRt << 1) + revBit();
    } while (--i != 0);


}

void SoftwareI2C::setRegister(unsigned char ucAddr, unsigned char ucReg, unsigned char dta)
{
    sendStart();
    sendByte(ucAddr<<1);
    getAck();
    delayMicroseconds(1);
    sendByte(ucReg);
    getAck();
    delayMicroseconds(1);
    sendByte(dta);
    getAck();
    sendStop();

}

void SoftwareI2C::mpr121_setup()
{
    // Section A - Controls filtering when data is > baseline.
    setRegister(0x5A, MHD_R, 0x01);
    setRegister(0x5A, NHD_R, 0x01);
    setRegister(0x5A, NCL_R, 0x00);
    setRegister(0x5A, FDL_R, 0x00);

    // Section B - Controls filtering when data is < baseline.
    setRegister(0x5A, MHD_F, 0x01);
    setRegister(0x5A, NHD_F, 0x01);
    setRegister(0x5A, NCL_F, 0xFF);
    setRegister(0x5A, FDL_F, 0x02);

    // Section C - Sets touch and release thresholds for each electrode
    setRegister(0x5A, ELE0_T, TOU_THRESH);
    setRegister(0x5A, ELE0_R, REL_THRESH);
    setRegister(0x5A, ELE1_T, TOU_THRESH);
    setRegister(0x5A, ELE1_R, REL_THRESH);
    setRegister(0x5A, ELE2_T, TOU_THRESH);
    setRegister(0x5A, ELE2_R, REL_THRESH);
    setRegister(0x5A, ELE3_T, TOU_THRESH);
    setRegister(0x5A, ELE3_R, REL_THRESH);
    setRegister(0x5A, ELE4_T, TOU_THRESH);
    setRegister(0x5A, ELE4_R, REL_THRESH);
    setRegister(0x5A, ELE5_T, TOU_THRESH);
    setRegister(0x5A, ELE5_R, REL_THRESH);
    setRegister(0x5A, ELE6_T, TOU_THRESH);
    setRegister(0x5A, ELE6_R, REL_THRESH);
    setRegister(0x5A, ELE7_T, TOU_THRESH);
    setRegister(0x5A, ELE7_R, REL_THRESH);
    setRegister(0x5A, ELE8_T, TOU_THRESH);
    setRegister(0x5A, ELE8_R, REL_THRESH);
    setRegister(0x5A, ELE9_T, TOU_THRESH);
    setRegister(0x5A, ELE9_R, REL_THRESH);
    setRegister(0x5A, ELE10_T, TOU_THRESH);
    setRegister(0x5A, ELE10_R, REL_THRESH);
    setRegister(0x5A, ELE11_T, TOU_THRESH);
    setRegister(0x5A, ELE11_R, REL_THRESH);
    setRegister(0x5A, FIL_CFG, 0x04);
    setRegister(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes

}

void SoftwareI2C::getDtaMpr121(unsigned char *d1, unsigned char *d2)
{
    unsigned char dta;
    sendStart();
    sendByte(0xb5);
    getAck();
    *d1 = revByte();
    sendAck();
    *d2 = revByte();
    sendNak();
    sendStop();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
