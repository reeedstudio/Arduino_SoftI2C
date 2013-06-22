/*
  SoftwareI2C.h
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
#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_

#include "MPR121LVC.h"

#define DELAY i2c_delay

#define __Debug        1                      // debug mode
#define  GETACK        1                      // get ack                        
#define  GETNAK        0                      // get nak      
#ifndef  HIGH        
#define  HIGH          1
#endif
#ifndef  LOW
#define  LOW           0
#endif

class SoftwareI2C
{
    private:
    
    int pinSda;
    int pinScl;
    
    private:
    
    void sda_set(unsigned char ucData); 
    void scl_set(unsigned char ucData);                             
    unsigned char get_sda(void);                                        
    void i2c_delay(void);          

    void mpr121_setup(void);

    public:
    
    void begin(int Sda, int Scl); 
    void sendStart(void);
    void sendStop(void);
    unsigned char getAck(void);
    void sendAck(void);
    void sendNak(void);
    
    void sendBit(unsigned char bit);
    unsigned char revBit();
    
    void sendByte(unsigned char ucData);
    unsigned char revByte();
    
   // unsigned char I2CWrite(unsigned char ucAddr, unsigned char uiRegAddr, unsigned char ucRegAddrLen, unsigned char *pucData, unsigned char ucDataLen)
    
    void setRegister(unsigned char ucAddr, unsigned char ucReg, unsigned char dta);
    void getDtaMpr121(unsigned char *d1, unsigned char *d2);

};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
