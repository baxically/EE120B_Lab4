/*	Author: Ally Thach, athac007@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab 4  Exercise 4
 *	Exercise Description: Extend the above door (exercise 4) to require the 4-button sequence #-X-Y-X rather than the earlier 2-button sequence. To avoid excessive states, store the correct button sequence in an array, and use a looping SM.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, locked, rightButton, rightButtonRel, unlocked} currState;

unsigned char tmpA;
unsigned char tmpB;
unsigned char tmpC;
unsigned char array[4] = {0x04, 0x01, 0x02, 0x01};
unsigned char cnt = 0;

void Tick()
{
    switch(currState) //transitions
    {
        case start:
            tmpB = 0;
            currState = locked;
            break;
        case locked:
            cnt = 0;
            if((tmpA & 0x07) == 0x04)
            {
                currState = rightButton;
                ++cnt;
            }
            else
            {
                currState = locked;
            }
            break;
        case rightButton:
            if((tmpA & 0x07) == array[cnt])
            {
                currState = rightButton;
            }
            else if((tmpA & 0x07) == 0x00)
            {
                ++cnt;
                currState = rightButtonRel;
            }
            else
            {
                currState = locked;
                cnt = 0;
            }
            break;
        case rightButtonRel:
            if((tmpA & 0x07) == 0x00)
            {
                currState = rightButtonRel;
            }
            else if((cnt == 3) && (tmpA & 0x07) == array[cnt])
            {
                currState = unlocked;
            }
            else if((tmpA & 0x07) == array[cnt])
            {
                currState = rightButton;
            }
            else
            {
                currState = locked;
                cnt = 0;
            }
            break;
        case unlocked:
            if((tmpA & 0x80) == 0x80)
            {
                cnt = 0;
                currState = locked;
            }
            else
            {
                currState = unlocked;
            }
            break;
        default:
            currState = start;
            break;
    }
    
    switch(currState)
    {
        case locked:
            tmpB = 0;
            tmpC = 0;
            break;
        case rightButton:
            tmpC = 1;
            break;
        case rightButtonRel:
            tmpC = 2;
            break;
        case unlocked:
            tmpB = 1;
            tmpC = 3;
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
    DDRC = 0xFF;
    PORTC = 0x00;
    
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
