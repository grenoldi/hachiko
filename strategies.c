#include <xc.h>
#include "strategies.h"

 void delayTMR1(int tms){
    T1CON = 0b10000000;
    for (int i = 0; i < tms; i++){
        TMR1 = 60537;
        T1CONbits.TMR1ON = 1;
        while(PIR1bits.TMR1IF == 0);
        T1CONbits.TMR1ON = 0;
        PIR1bits.TMR1IF  = 0;
        readTarget();
        if(target != 0 && line == 0){
            state = ATTACKING;
            INTCONbits.TMR0IE = 1;
            break;
        }
    }
}

void delay_ms(int tms){
    for(int i = 0; i < tms; i++){
        __delay_ms(1);
        
        if(PORTAbits.RA0 == 0 ||
           PORTAbits.RA1 == 0 ||
           PORTAbits.RA2 == 0 ||
           PORTAbits.RA3 == 0 ||
           PORTCbits.RC0 == 0 ||
           PORTCbits.RC3 == 0 ||
           PORTCbits.RC4 == 0 ||
           PORTAbits.RA5 == 0 ||
           PORTEbits.RE0 == 0){
            break;
        }
    }
}
void searchRoutine(unsigned char fmove, unsigned char fatk, unsigned char stdstg){
    getFirstMove(fmove);
    getFirstAttack(fatk);
    getStrategy(stdstg);
}

/*
 //sensor positioning (target):
  ______front______
 |3       2       1|
 |                 |
 |4               0|
 |                 |
 |                 |
 |        5        |
 
 */

void attackRoutine(unsigned char target){
    if (target == 1){
        moveMotor(right, backward, 100);
        moveMotor(left, forward, 100);
        __delay_ms(50);
    }
    else if (target == 2){
        moveMotor(right, backward, 80);
        moveMotor(left, forward, 80);
    }
    
    else if (target ==  4 || 
             target ==  6 ||
             target == 10 ||
             target == 12 ||
             target == 14){
        
        moveMotor(right, forward, 100);
        moveMotor(left, forward, 100);
    }
    else if (target == 8){
        moveMotor(right, forward, 80);
        moveMotor(left, backward, 80);
    }
    else if (target == 16){
        moveMotor(right, forward, 80);
        moveMotor(left, backward, 80);
        __delay_ms(50);
    }
}

/*
 //sensor positioning(line):
  ______front______
 |1               0|
 |                 |
 |                 |
 |                 |
 |                 |
 |2               3|
 
 */
void saveYourselfRoutine(unsigned char line){
    if (line > 0){
        if(line < 4){
            moveMotor(right, backward, 100);
            moveMotor(left, backward, 100);
            __delay_ms(150);
            stopMotors();
            moveMotor(right, forward, 100);
            moveMotor(left, backward, 100);
            __delay_ms(100);
        }
        else if(line == 4){
            stopMotors();
            moveMotor(left, forward, 100);
            __delay_ms(50);
        }
        else if(line == 6){
            stopMotors();
            moveMotor(left, forward, 100);
            __delay_ms(100);
        }
        else if(line == 8){
            stopMotors();
            moveMotor(right, forward, 100);
            __delay_ms(50);
        }
        else if(line == 9){
            stopMotors();
            moveMotor(right, forward, 100);
            __delay_ms(100);
        }
        else if(line == 12){
            stopMotors();
            moveMotor(right, forward, 100);
            moveMotor(left, forward, 100);
            __delay_ms(50);
        }
    }
}

unsigned char getFirstMove(unsigned char fmove){
    if(fmove == 'a');
    
    else if(fmove == 'b'){ //rotaciona pra direita
        moveMotor(right, backward, 100);
        moveMotor(left, forward, 100);
        delay_ms(50);
    }
    
    else if(fmove == 'c'){ //rotaciona pra esquerda
        moveMotor(right, forward, 100);
        moveMotor(left, backward, 100);
        delay_ms(50);
    }
    
    else if(fmove == 'd'){ //arco tras direita
        moveMotor(right, backward, 60);
        moveMotor(left, backward, 90);
    }
    
    else if(fmove == 'e'){ //arco tras esquerda
        moveMotor(right, backward, 90);
        moveMotor(left, backward, 60);
    }
    else if(fmove == 'f'){ //reto pra tras
        moveMotor(right, backward, 70);
        moveMotor(left, backward, 80);
    }
    return 'a';
}

unsigned char getFirstAttack(unsigned char fatk){
    if(fatk == 'a');
    
    else if(fatk == 'b'){ //arco direita
        moveMotor(right, backward, 60);
        moveMotor(left, backward, 90);
        delay_ms(1000);
    }
    
    else if(fatk == 'c'){ //arco esquerda
        moveMotor(right, backward, 90);
        moveMotor(left, backward, 70);
        delay_ms(1000);
    }
}

void getStrategy(unsigned char stdstg){
    if(stdstg == 'a'){ //estrela normal
        moveMotor(right, forward, 80);
        moveMotor(left, forward, 90);
    }
    
    else if(stdstg == 'b'){ //estrela lenta
        moveMotor(right, forward, 50);
        moveMotor(left, forward, 60);
    }
    
    else if(stdstg == 'c'){ //pica pau
        moveMotor(right, forward, 80);
        moveMotor(left, forward, 90);
        delay_ms(100);
        stopMotors();
        delay_ms(1000);
    }
    
    else if(stdstg == 'd'){ //varredura
        moveMotor(right, forward, 50);
        moveMotor(left, backward, 50);
        delay_ms(100);
        
        stopMotors();
        
        moveMotor(right, backward, 50);
        moveMotor(left, backward, 50);
        delay_ms(100);
    }
    
    else if(stdstg == 'e'){ //twister
        moveMotor(right, forward, 70);
        moveMotor(left, backward, 70);
    }
}

void rotate(int side){
    if(side){
        moveMotor(right, backward, 100);
        moveMotor(left,   forward, 100);
    }
    
    else{
        moveMotor(right, forward, 100);
        moveMotor(left, backward, 100);
    }
}

void arch(int side, int direction, int speed){
    if(side){
        if(direction){
            moveMotor(right,  forward, speed - 10);
            moveMotor(left,   forward, speed + 10);
        }
        
        else{
            moveMotor(right,  backward, speed - 10);
            moveMotor(left,   backward, speed + 10);
        }
    }
    
    else {
        if(direction){
            moveMotor(right,  forward, speed + 10);
            moveMotor(left,   forward, speed - 10);
        }
        
        else{
            moveMotor(right,  backward, speed + 10);
            moveMotor(left,   backward, speed - 10);
        }
    }
}

void HachikoStdRoutine(){
    /*
     * Sensores individuais
     */
            
    //Sensor linha frontal esquerda
            
    if(PORTCbits.RC3 == 0 && 
       PORTCbits.RC4 == 1 &&
       PORTEbits.RE1 == 1 &&
       //PORTEbits.RE2 == 1 
       PORTCbits.RC5 == 1 
               ){
                
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, backward, 60);
                __delay_ms(100);
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, forward, 60);
                __delay_ms(100);
            }
            
            //Sensor linha frontal direita
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 0 &&
                    PORTEbits.RE1 == 1 &&
                    //PORTEbits.RE2 == 1 
                    PORTCbits.RC5 == 1 
                    ){
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, backward, 60);
                __delay_ms(100);
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, forward, 60);
                __delay_ms(100);
            }
            
            //Sensor linha traseiro esquerda
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 1 &&
                    PORTEbits.RE1 == 0 &&
                    //PORTEbits.RE2 == 1 
                    PORTCbits.RC5 == 1 
                    ){
                stopMotors();
                moveMotor(left, forward, 60);
                __delay_ms(100);
                stopMotors();
                moveMotor(right, forward, 60);
                moveMotor(left, forward, 60);
                __delay_ms(100);
            }
            //Sensor linha traseiro direita
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 1 &&
                    PORTEbits.RE1 == 1 &&
                    //PORTEbits.RE2 == 0
                    PORTCbits.RC5 == 0 
                    ){
                stopMotors();
                moveMotor(right, forward, 60);
                __delay_ms(100);
                stopMotors();
                moveMotor(right, forward, 60);
                moveMotor(left, forward, 60);
                __delay_ms(100);
            }
            
            /*
             * Sensores em pares
             */
            
            // os dois frontais
            else if(PORTCbits.RC3 == 0 && 
                    PORTCbits.RC4 == 0 &&
                    PORTEbits.RE1 == 1 &&
                    //PORTEbits.RE2 == 1
                    PORTCbits.RC5 == 1
                    ){
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, backward,  60);
                __delay_ms(100);
                stopMotors();
                moveMotor(right, backward, 60);
                moveMotor(left, forward,   60);
                __delay_ms(100);
            }
            
            // os dois laterais direita
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 0 &&
                    PORTEbits.RE1 == 1 &&
                    //PORTEbits.RE2 == 0
                    PORTCbits.RC5 == 0
                    ){
                stopMotors();
                moveMotor(right, forward, 60);
                __delay_ms(100);
                stopMotors();
            }
            
            // os dois laterais esquerda
            else if(PORTCbits.RC3 == 0 && 
                    PORTCbits.RC4 == 1 &&
                    PORTEbits.RE1 == 0 &&
                    //PORTEbits.RE2 == 1
                    PORTCbits.RC5 == 1 
                    ){
                stopMotors();
                moveMotor(left, forward, 60);
                __delay_ms(100);
                stopMotors();
            }
            
            // os dois traseiros
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 1 &&
                    PORTEbits.RE1 == 0 &&
                    //PORTEbits.RE2 == 0
                    PORTCbits.RC5 == 0
                    ){
                stopMotors();
                moveMotor(right, forward, 60);
                moveMotor(left, forward, 60);
                __delay_ms(100);
                stopMotors();
            }
            
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 1 &&
                    PORTEbits.RE1 == 1 &&
                    //PORTEbits.RE2 == 1
                    PORTCbits.RC5 == 1 
                    ){
                
                if(PORTAbits.RA0 == 1 &&
                   PORTAbits.RA1 == 1 &&
                   PORTAbits.RA5 == 1 &&
                   PORTAbits.RA2 == 1 &&
                   PORTAbits.RA3 == 1 ){
                    moveMotor(right, forward, 60);
                    moveMotor(left, forward, 60);
                }
                else if(PORTAbits.RA0 == 0 &&
                        PORTAbits.RA1 == 1 &&
                        PORTAbits.RA5 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA3 == 1 ){
                    moveMotor(right, backward, 80);
                    moveMotor(left, forward, 80);
                    __delay_ms(50);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTAbits.RA5 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA3 == 1 ){
                    moveMotor(right, backward, 100);
                    moveMotor(left, forward,   100);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTAbits.RA5 == 1 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA3 == 1 ){
                    moveMotor(right, forward, 80);
                    moveMotor(left, backward, 80);
                    __delay_ms(50);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTAbits.RA5 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA3 == 0 ){
                    moveMotor(right, forward, 100);
                    moveMotor(left, backward, 100);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTAbits.RA5 == 0 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA3 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTAbits.RA5 == 0 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA3 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTAbits.RA5 == 0 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA3 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTAbits.RA5 == 0 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA3 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                } 
                else{
                    stopMotors();
                }
            }
}

void MetalGarurumonChampsTC2018(){
    readLine();
    if(line > 0){
        if(rightMLastOrder == forward || leftMLastOrder == forward){
            stopMotors();
        }
        moveMotor(right, backward, 100);
        moveMotor(left, backward, 100);
        __delay_ms(150);
        stopMotors();
        moveMotor(right, forward, 100);
        moveMotor(left, backward, 100);
        __delay_ms(100);
    }
   
    else if(line == 0){
        readTarget();
        if(target == 0){
            if(rightMLastOrder == backward || leftMLastOrder == backward){
                stopMotors();
            }
            
            moveMotor(right, forward, 70);
            moveMotor(left, forward, 80);
        }
        else if(target == 1){
            if(rightMLastOrder == forward || leftMLastOrder == backward){
                stopMotors();
            }
            moveMotor(right, backward, 100);
            moveMotor(left, forward, 100);
            __delay_ms(50);
        }
        else if(target == 2){
            if(rightMLastOrder == forward || leftMLastOrder == backward){
                stopMotors();
            }
            moveMotor(right, backward, 80);
            moveMotor(left, forward, 80);
        }
        
        else if(target == 4){
            stopMotors();
        }
        
        else if(target == 6){
            stopMotors();
        }

        else if(target == 8){
            if(rightMLastOrder == backward || leftMLastOrder == forward){
                stopMotors();
            }
            moveMotor(right, forward, 80);
            moveMotor(left, backward, 80);
        }
        
        else if(target == 10){
            stopMotors();
        }

        else if(target == 12){
            stopMotors();
        }

        else if(target == 14){
            stopMotors();
        }

        else if(target == 16){
            if(rightMLastOrder == backward || leftMLastOrder == forward){
                stopMotors();
            }
            moveMotor(right, forward, 100);
            moveMotor(left, backward, 100);
            __delay_ms(50);
        }
    }
        /*
        if(PORTAbits.RA0 == 1 && 
           PORTAbits.RA1 == 1 && 
           PORTAbits.RA2 == 1 && 
           PORTAbits.RA3 == 1 &&
           PORTCbits.RC0 == 1){
            if(rightMLastOrder == backward || leftMLastOrder == backward){
                stopMotors();
            }
            moveMotor(right, forward, 50);
            moveMotor(left, forward, 50);
        }
        else if(PORTAbits.RA0 == 0 && 
                PORTAbits.RA1 == 1 && 
                PORTAbits.RA2 == 1 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 1){
            if(rightMLastOrder == forward || leftMLastOrder == backward){
                stopMotors();
            }
            moveMotor(right, backward, 100);
            moveMotor(left, forward, 100);
            __delay_ms(50);
        }
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 0 && 
                PORTAbits.RA2 == 1 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 1){
            if(rightMLastOrder == forward || leftMLastOrder == backward){
                stopMotors();
            }
            moveMotor(right, backward, 80);
            moveMotor(left, forward, 80);
        }
        //else if(target == 3){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 1 && 
                PORTAbits.RA2 == 1 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 0){
            stopMotors();
        }
        //else if(target == 5){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 0 && 
                PORTAbits.RA2 == 1 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 0){
            stopMotors();
        }
        //else if(target == 7){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 1 && 
                PORTAbits.RA2 == 0 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 1){
            if(rightMLastOrder == backward || leftMLastOrder == forward){
                stopMotors();
            }
            moveMotor(right, forward, 80);
            moveMotor(left, backward, 80);
        }
        //else if(target == 9){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 0 && 
                PORTAbits.RA2 == 0 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 1){
            stopMotors();
        }
        //else if(target == 11){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 1 && 
                PORTAbits.RA2 == 0 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 0){
            stopMotors();
        }
        //else if(target == 13){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 0 && 
                PORTAbits.RA2 == 0 && 
                PORTAbits.RA3 == 1 &&
                PORTCbits.RC0 == 0){
            stopMotors();
        }
        //else if(target == 15){}
        else if(PORTAbits.RA0 == 1 && 
                PORTAbits.RA1 == 1 && 
                PORTAbits.RA2 == 1 && 
                PORTAbits.RA3 == 0 &&
                PORTCbits.RC0 == 1){
            if(rightMLastOrder == backward || leftMLastOrder == forward){
                stopMotors();
            }
            moveMotor(right, forward, 100);
            moveMotor(left, backward, 100);
            __delay_ms(50);
        }
        //else if(target == 17){}
        //else if(target == 18){}
        //else if(target == 19){}
        //else if(target == 20){}
        //else if(target == 21){}
        //else if(target == 22){}
        //else if(target == 23){}
        //else if(target == 24){}
        //else if(target == 25){}
        //else if(target == 26){}
        //else if(target == 27){}
        //else if(target == 28){}
        //else if(target == 29){}
        //else if(target == 30){}
        //else if(target == 31){}
    }*/
}
