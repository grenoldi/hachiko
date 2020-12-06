/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STRATEGIES
#define	STRATEGIES

#include <xc.h> 
#include "smctl.h"
typedef enum possible_states{LISTENING, SEARCHING, RUNNING, ATTACKING, TESTING} State;

State state = LISTENING;

void delayTMR1(int tms);
void delay_ms(int tms);
void searchRoutine(unsigned char fmove, unsigned char fatk, unsigned char stdstg);
void attackRoutine(unsigned char target);
void saveYourselfRoutine(unsigned char line);
unsigned char getFirstMove(unsigned char fmove);
unsigned char getFirstAttack(unsigned char fatk);
void getStrategy(unsigned char stdstg);
void rotate(int side);
void arch(int side, int direction, int speed);
void HachikoStdRoutine();
void MetalGarurumonChampsTC2018();
State getState(int line, int target, char opt);

#endif