/*	Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, wait, plusHold, minusHold, restart} currState;

unsigned char  tmpA;
unsigned char tmpC;

void Tick()
{
    switch(currState) //transitions
    {
        case start:
            tmpC = 7; //initial output at PORTC
            currState = wait;
            break;
        case wait:
            if(((tmpA & 0x01) == 0x01) && ((tmpA & 0x02) == 0x00))
            {
                currState = plusHold; //will increase tmpC if < 9
                if(tmpC < 9)
                {
                    tmpC++;
                }
            }
            else if (((tmpA & 0x02) == 0x02) && ((tmpA & 0x01) == 0x00))
            {
                currState = minusHold; //will decrease tmpC if > 0
                if(tmpC > 0)
                {
                    tmpC--;
                }
            }
            else if((tmpA & 0x03) == 0x03)
            {
                currState = restart;
            }
            else
            {
                currState = wait;
            }
            break;
        case plusHold:
            if((tmpA & 0x01) == 0x01)
            {
                currState = plusHold;
            }
            else
            {
                currState = wait;
            }
            break;
        case minusHold:
            if((tmpA & 0x02) == 0x02)
            {
                currState = minusHold;
            }
            else
            {
                currState = wait;
            }
            break;
        case restart:
            if((tmpA & 0x03) == 0x03)
            {
                currState = restart;
            }
            else
            {
                currState = wait;
            }
            break;
        default:
            currState = start;
            break;
    }
    
    switch(currState) //actions incrementing/decrementing at wrong time????
    {
        case wait:
            break;
        case plusHold:
            /*if(tmpC < 9)
            {
                tmpC++;
            }*/
            break;
        case minusHold:
            /*if(tmpC > 0)
            {
                tmpC--;
            }*/
            break;
        case restart:
            break;
        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRC = 0xFF;
    PORTC = 0x00;
    
    tmpC = 0x00;
    currState = start;
    /* Insert your solution below */
    while (1) {
        tmpA = PINA;
        Tick(); //calls SM
        PORTC = tmpC;
    }
    return 1;
}
