/*    Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name:
 *    Lab Section: 24
 *    Assignment: Lab 4  Exercise 2
 *    Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
 *
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, doNothing, plusHold, minusHold, restart} currState;

unsigned char tmpC = 7; //initial output PORTC

void Tick()
{
    unsigned char tmpA = PINA;
    switch(currState) //transitions
    {
        case start:
            currState = doNothing;
            break;
        case doNothing:
            if((tmpA & 0x03) == 0x00)
            {
                currState = doNothing;
            }
            else if((tmpA & 0x03) == 0x01)
            {
                currState = plusHold;
                if(tmpC != 9)
                {
                    ++tmpC;
                    PORTC = tmpC;
                }
            }
            else if((tmpA & 0x03) == 0x02)
            {
                currState = minusHold;
                if(tmpC != 0)
                {
                    --tmpC;
                    PORTC = tmpC;
                }
            }
            else
            {
                currState = restart;
                tmpC = 0;
                PORTC = tmpC;
            }
            break;
        case minusHold:
            if((tmpA & 0x03) == 0x00)
            {
                currState = doNothing;
            }
            else if((tmpA & 0x03) == 0x00)
            {
                currState = plusHold;
                if(tmpC != 9)
                {
                    ++tmpC;
                    PORTC = tmpC;
                }
            }
            else if((tmpA & 0x03) == 0x02)
            {
                currState = minusHold;
            }
            else
            {
                currState = restart;
                tmpC = 0;
                PORTC = tmpC;
            }
            break;
        case plusHold:
            if((tmpA & 0x03) == 0x00)
            {
                currState = doNothing;
            }
            else if((tmpA & 0x03) == 0x01)
            {
                currState = plusHold;
            }
            else if((tmpA & 0x03) == 0x02)
            {
                currState = minusHold;
                if(tmpC != 0)
                {
                    --tmpC;
                    PORTC = tmpC;
                }
            }
            else
            {
                currState = restart;
                tmpC = 0;
                PORTC = tmpC;
            }
            break;
        case restart:
            if((tmpA & 0x03) == 0x00)
            {
                currState = doNothing;
            }
            else if((tmpA & 0x03) == 0x01)
            {
                currState = plusHold;
                if(tmpC != 9)
                {
                    ++tmpC;
                    PORTC =  tmpC;
                }
            }
            else if((tmpA & 0x03) == 0x02)
            {
                currState = minusHold;
                if(tmpC != 0)
                {
                    --tmpC;
                    PORTC = tmpC;
                }
            }
            else
            {
                currState  = restart;
                tmpC = 0;
                PORTC = tmpC;
            }
            break;
        default:
            currState = start;
            break;
    }
    
    switch(currState) //actions
    {
        case start:
            PORTC =  tmpC;
            break;
        case doNothing:
            break; //litreally does nothing
        case plusHold:
            break; //transition actoin above
        case minusHold:
            break; //transition actioin above
        case restart:
            break; //transition action above
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
    
    currState = start;
    /* Insert your solution below */
    while (1) {
        Tick(); //calls SM
    }
    return 1;
}
