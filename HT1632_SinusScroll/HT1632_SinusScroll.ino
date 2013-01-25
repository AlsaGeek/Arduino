/*
 * matrix_08x32 - Arduino demo program for Holtek HT1632 LED driver chip,
 *            As implemented on the Sure Electronics DE-DP106 display board
 *            (08*32 dot matrix LED module.)
 * Andrew Hedges
 * Dec, 2008
 * Jul, 2009 by Bill Ho  
 *              for multi-devices
 * Adapted from code by Bill Westfield ("WestfW")
 *   Copyrighted and distributed under the terms of the Berkely license
 *   (copy freely, but include this notice of original author.)
 */


#include "ht1632.h"
#include "sinus.h"

#define NUM_DEVICES 3     // Supoort up to 4 devices

#define X_MAX 31*NUM_DEVICES - 1
#define Y_MAX 8
#define X_SIZE 32*NUM_DEVICES
#define Y_SIZE 8

#define ASSERT(condition) //nothing

/*
 * Set these constants to the values of the pins connected to the SureElectronics Module
 * NOTE - these are different from the original demo code by westfw
 */
static const byte ht1632_data = 6;  // Data pin (pin 7 bleu)
static const byte ht1632_wrclk = 7; // Write clock pin (pin 5 vert)
static const byte ht1632_cs[] = {2,3,4,5};    // Chip Select (1, 2, 3, or 4)

int sensorPin = 0;
int sensorValue = 0;

// The should also be a common GND.
// The module with all LEDs like draws about 200mA,
//  which makes it PROBABLY powerable via Arduino +5V

#define DEMOTIME 20000  // 20 seconds max on each demo is enough.
#define DISPDELAY 40    // Each "display" lasts this long
#define LONGDELAY 1000  // This delay BETWEEN demos

/*
 * ht1632_writebits
 * Write bits (up to 8) to h1632 on pins ht1632_data, ht1632_wrclk
 * Chip is assumed to already be chip-selected
 * Bits are shifted out from MSB to LSB, with the first bit sent
 * being (bits & firstbit), shifted till firsbit is zero.
 */
void ht1632_chipselect(byte chipno)
{
  DEBUGPRINT("\nHT1632(%d) ", chipno);
  digitalWrite(ht1632_cs[chipno], 0);
}

void ht1632_chipfree(byte chipno)
{
  DEBUGPRINT(" [done %d]", chipno);
  digitalWrite(ht1632_cs[chipno], 1);
}
/*
 * ht1632_writebits
 * Write bits (up to 8) to h1632 on pins ht1632_data, ht1632_wrclk
 * Chip is assumed to already be chip-selected
 * Bits are shifted out from MSB to LSB, with the first bit sent
 * being (bits & firstbit), shifted till firsbit is zero.
 */
void ht1632_writebits (byte bits, byte firstbit)
{
  DEBUGPRINT(" ");
  while (firstbit) {
    DEBUGPRINT((bits&firstbit ? "1" : "0"));
    digitalWrite(ht1632_wrclk, LOW);
    if (bits & firstbit) {
    digitalWrite(ht1632_data, HIGH);
    }
    else {
    digitalWrite(ht1632_data, LOW);
    }
    digitalWrite(ht1632_wrclk, HIGH);
    firstbit >>= 1;
  }
}
/*
 * ht1632_sendcmd
 * Send a command to the ht1632 chip.
 * A command consists of a 3-bit "CMD" ID, an 8bit command, and
 * one "don't care bit".
 *   Select 1 0 0 c7 c6 c5 c4 c3 c2 c1 c0 xx Free
 */
static void ht1632_sendcmd (byte chipno, byte command)
{
  ht1632_chipselect(chipno);  // Select chip
  ht1632_writebits(HT1632_ID_CMD, 1<<2);  // send 3 bits of id: COMMMAND
  ht1632_writebits(command, 1<<7);  // send the actual command
  ht1632_writebits(0, 1);     /* one extra dont-care bit in commands. */
  ht1632_chipfree(chipno); //done
}
/*
 * ht1632_senddata
 * send a nibble (4 bits) of data to a particular memory location of the
 * ht1632.  The command has 3 bit ID, 7 bits of address, and 4 bits of data.
 *    Select 1 0 1 A6 A5 A4 A3 A2 A1 A0 D0 D1 D2 D3 Free
 * Note that the address is sent MSB first, while the data is sent LSB first!
 * This means that somewhere a bit reversal will have to be done to get
 * zero-based addressing of words and dots within words.
 */
static void ht1632_senddata (byte chipno, byte address, byte data)
{
  ht1632_chipselect(chipno);  // Select chip
  ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
  ht1632_writebits(address, 1<<6); // Send address
  ht1632_writebits(data, 1<<3); // send 4 bits of data
  ht1632_chipfree(chipno); // done
}

void ht1632_setup()
{
  for (byte chipno=0; chipno<NUM_DEVICES; chipno++){
	  pinMode(ht1632_cs[chipno], OUTPUT);
	  ht1632_chipfree(chipno); 	/* unselect it */
	  pinMode(ht1632_wrclk, OUTPUT);
	  pinMode(ht1632_data, OUTPUT);
	  ht1632_sendcmd(chipno, HT1632_CMD_SYSDIS);  // Disable system
	  ht1632_sendcmd(chipno, HT1632_CMD_COMS10);  // 16*32, PMOS drivers
	  ht1632_sendcmd(chipno, HT1632_CMD_MSTMD); 	/* Master Mode */
	  ht1632_sendcmd(chipno, HT1632_CMD_SYSON); 	/* System on */
	  ht1632_sendcmd(chipno, HT1632_CMD_LEDON); 	/* LEDs on */
	  for (byte i=0; i<96; i++)
		ht1632_senddata(chipno, i, 0);  // clear the display!
	  // delay(LONGDELAY);
  }
}


#include "img2arduino0.h"
#include "img2arduino1.h"
#include "img2arduino2.h"
#include "img2arduino3.h"

void setup ()  
{
    ht1632_setup();
    
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
}

int brt_ctrl=0;

void loop () {
  int brt;
  int pos;
  int cpt;
  int dis;
  int p, p8, p9, pnew;
  int messageLength;
  unsigned char sincol;
  unsigned char sintmp;

  
  ht1632_sendcmd(0, HT1632_CMD_PWM+15);

  p8 = digitalRead(8);
  p9 = digitalRead(9);
  p  = (p9*2)+p8;
  
  switch(p) {
    case 0:
      messageLength = messageLength0;
      break;
    case 1:
      messageLength = messageLength1;
      break;
    case 2:
      messageLength = messageLength2;
      break;
    case 3:
      messageLength = 2;
      break;
  }
    
  for(pos=0; pos<messageLength; pos++) {

    sensorValue = analogRead(sensorPin);
    delay(sensorValue/30);

    for( dis=0 ; dis<=NUM_DEVICES ; dis++ ) {
      ht1632_chipselect(dis);  // Select chip
      ht1632_writebits(HT1632_ID_WR, 1<<2);  // send ID: WRITE to RAM
      ht1632_writebits(0, 1<<6); // Send address

/*
      for(cpt=(32*dis)+pos; cpt<(32*dis)+pos+32; cpt++) {
        ht1632_writebits(message0[cpt], 1<<7); // send 4 bits of data
      }
*/

      if(p==0) {
        for(cpt=(32*dis)+pos; cpt<(32*dis)+pos+32; cpt++) {
          ht1632_writebits(message0[cpt], 1<<7); // send 4 bits of data
        }
      }
      if(p==1) {
        for(cpt=(32*dis)+pos; cpt<(32*dis)+pos+32; cpt++) {
          
          sincol = message1[cpt];
          sintmp = sinus[(7*pos+3*cpt)%360];
          
          if(sintmp<127) {
            sintmp = 127-sintmp;
            sintmp = sintmp/32;
            sincol=sincol<<sintmp;
          } else {
            sintmp = sintmp-127;
            sintmp = sintmp/32;
            sincol=sincol>>sintmp;
          }

          // pos
          
          ht1632_writebits(sincol, 1<<7); // send 4 bits of data
        }
      }
      if(p==2) {
        for(cpt=(32*dis)+pos; cpt<(32*dis)+pos+32; cpt++) {
          ht1632_writebits(message2[cpt], 1<<7); // send 4 bits of data
        }
      }
      if(p==3) {
        for(cpt=(32*dis)+pos; cpt<(32*dis)+pos+32; cpt++) {
          if(pos) ht1632_writebits(B11111111, 1<<7); // send 4 bits of data
          else    ht1632_writebits(B00000000, 1<<7); // send 4 bits of data
          
          delay(sensorValue/300);
        }
      }
    
      ht1632_chipfree(dis); // done
    }

  


    //delay(10);
    
    p8 = digitalRead(8);
    p9 = digitalRead(9);
    pnew  = (p9*2)+p8;
    
    if(p!=pnew) break;
  }
  
  // brt_ctrl = 1-brt_ctrl;
}

 

