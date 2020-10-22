/*	Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 4
 *	Exercise Description: Extend the above door (exercise 3) so that it can also be locked by entering the earlier code.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, wait, s1, s2, outUnlock, outLock, inside} currState;

unsigned char  tmpA;
unsigned char tmpB;
unsigned char tmpC;

void Tick()
{
    switch(currState) //transitions
    {
        case start:
            tmpB = 0;
            currState = wait;
            break;
        case wait:
            if((tmpA & 0x04) == 0x04) //PA2 is on, then start s1
            {
                currState = s1;
            }
            else if((tmpA & 0x80) == 0x80) //PA7 is on, lock
            {
                currState = inside;
            }
            else
            {
                currState = wait;
            }
            break;
        case s1:
            if((tmpA & 0x87) == 0x00) //PA2 released? => s2
            {
                currState = s2;
            }
            else if((tmpA & 0x04) == 0x04)
            {
                currState = s1;
            }
            else
            {
                currState = wait;
            }
            break;
        case s2:
            if((tmpA & 0x87) == 0x02 && (tmpB == 0))
            {
                currState = outUnlock;
            }
            else if((tmpA & 0x87) == 0x02 && (tmpB == 1))
            {
                currState = outLock;
            }
            else if((tmpA == 0x87) == 0x00)
            {
                currState = s2;
            }
            else
            {
                currState = wait;
            }
            break;
        case outUnlock:
            if((tmpA & 0x02) == 0x02)
            {
                currState = outUnlock;
            }
            else
            {
                currState = wait;
            }
            break;
        case outLock:
            if((tmpA & 0x02) == 0x02)
            {
                currState = outLock;
            }
            else
            {
                currState = wait;
            }
            break;
        case inside:
            if((tmpA = 0x80) == 0x80)
            {
                currState = inside;
            }
            else
            {
                currState = wait;
            }
        default:
            currState = start;
            break;
    }
    
    switch(currState) //actions
    {
        case wait:
            tmpC = 0;
            break;
        case s1:
            tmpC = 1;
            break;
        case s2:
            tmpC = 2;
            break;
        case outUnlock:
            tmpC = 3;
            tmpB = 1;
            break;
        case outLock:
            tmpC = 4;
            tmpB = 0;
            break;
        case inside:
            tmpC = 5;
            tmpB = 0;
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
    tmpC = 0x00;
    currState = start;
    /* Insert your solution below */
    while (1) {
        tmpA = PINA;
        Tick(); //calls SM
        PORTB = tmpB;
        PORTC = tmpC;
    }
    return 1;
}
