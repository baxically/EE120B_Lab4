/*	Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: PB0 and PB1 each connect to an LED, and PB0's LED is initially on. Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release. Pressing the button again turns off PB1's LED and turns on PB0's LED.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, aRelease, aHold, bRelease, bHold} currState;

unsigned char  tmpA;
unsigned char tmpB;

void Tick()
{
    switch(currState) //transitions
    {
        case start: //B1 led on, B0 led off
            currState = aRelease;
            break;
        case aRelease:
            if((tmpA & 0x01) == 0x01) //move onto light LED stage
            {
                currState = bHold;
            }
            else
            {
                currState = aRelease;
            }
            break;
        case bHold:
            if((tmpA & 0x01) == 0x01)
            {
                currState = bHold;
            }
            else
            {
                currState = bRelease;
            }
            break;
        case bRelease:
            if((tmpA & 0x01) == 0x01)
            {
                currState = aHold;
            }
            else
            {
                currState = bRelease;
            }
            break;
        case aHold:
            if((tmpA & 0x01) == 0x01)
            {
                currState = aHold;
            }
            else
            {
                currState = aRelease;
            }
            break;
        default:
            currState = start;
            break;
    }
    
    switch(currState) //state actions
    {
        case aRelease:
            tmpB = tmpB | 0x01; //B0 = 1
            tmpB = tmpB & 0xFD; //A0 = 0
            break;
        case bHold:
            tmpB = tmpB | 0x02;
            tmpB = tmpB & 0xFE;
            break;
        case bRelease:
            tmpB = tmpB | 0x02;
            tmpB = tmpB & 0xFE;
            break;
        case aHold:
            tmpB = tmpB | 0x01;
            tmpB = tmpB & 0xFD;
        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    
    tmpB = 0x00;
    currState = start;
    /* Insert your solution below */
    while (1) {
        tmpA = PINA;
        Tick(); //calls SM
        PORTB = tmpB;
    }
    return 1;
}
