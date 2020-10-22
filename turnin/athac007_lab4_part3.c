/*	Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 3
 *	Exercise Description: A household has a digital combination deadbolt lock system on the doorway. The system has buttons on a keypad. Button 'X' connects to PA0, 'Y' to PA1, and '#' to PA2. Pressing and releasing '#', then pressing 'Y', should unlock the door by setting PB0 to 1. Any other sequence fails to unlock. Pressing a button from inside the house (PA7) locks the door (PB0=0). For debugging purposes, give each state a number, and always write the current state to PORTC (consider using the enum state variable). Also, be sure to check that only one button is pressed at a time.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, wait, s1, s2, unlock, lock} currState;

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
                currState = lock;
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
            if((tmpA & 0x87) == 0x02)
            {
                currState = unlock;
            }
            else if((tmpA & 0x87) == 0x00)
            {
                currState = s2;
            }
            else
            {
                currState = wait;
            }
            break;
        case unlock:
            if((tmpA & 0x02) == 0x02)
            {
                currState = unlock;
            }
            else
            {
                currState = wait;
            }
            break;
        case lock:
            if((tmpA & 0x80) == 0x80)
            {
                currState = lock;
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
        case unlock:
            tmpC = 3;
            tmpB = 1;
            break;
        case lock:
            tmpC = 4;
            tmpB = 0;
            break;
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
