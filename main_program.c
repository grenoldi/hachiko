// PIC18F4431 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal External Oscillator Switchover bit (Internal External Switchover mode enabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled)
// BORV = No Setting

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDPS = 32768     // Watchdog Timer Postscale Select bits (1:32768)
#pragma config WINEN = OFF      // Watchdog Timer Window Enable bit (WDT window disabled)

// CONFIG3L
#pragma config PWMPIN = OFF     // PWM output pins Reset state control (PWM outputs disabled upon Reset (default))
#pragma config LPOL = HIGH      // Low-Side Transistors Polarity (PWM0, 2, 4 and 6 are active-high)
#pragma config HPOL = HIGH      // High-Side Transistors Polarity (PWM1, 3, 5 and 7 are active-high)
#pragma config T1OSCMX = ON     // Timer1 Oscillator MUX (Low-power Timer1 operation when microcontroller is in Sleep mode)

// CONFIG3H
#pragma config FLTAMX = RC1     // FLTA MUX bit (FLTA input is multiplexed with RC1)
#pragma config SSPMX = RC7      // SSP I/O MUX bit (SCK/SCL clocks and SDA/SDI data are multiplexed with RC5 and RC4, respectively. SDO output is multiplexed with RC7.)
#pragma config PWM4MX = RB5     // PWM4 MUX bit (PWM4 output is multiplexed with RB5)
#pragma config EXCLKMX = RC3    // TMR0/T5CKI External clock MUX bit (TMR0/T5CKI external clock input is multiplexed with RC3)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (Enabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-000FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (001000-001FFF) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (002000-002FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (003000-003FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-000FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (001000-001FFF) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (002000-002FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (003000-003FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-000FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (001000-001FFF) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (002000-002FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (003000-003FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <pic18f4431.h>

#include "smctl.h"
#include "strategies.h"

//Global variables
unsigned char     inData;
unsigned char     outData;
unsigned char     option = '#';
unsigned char     message[SIZE];
unsigned char     firstMove = 'a';
unsigned char     stdSearch;

int          gotcha;
int          searchSpeed;

int const    base_speed  = 50;
int const    line_time_b = 100;
int const    line_time_r = 100;
int const    delay_time  = 4600;


RingBuffer*       rbPtr;
RingBuffer        msg  = {message, 0, 0, SIZE};

//function prototypes

char get_char(void);
void send_char(char ch);

//FUNCTIONS
//Interrupt functions
void __interrupt() ISR(void){

    
    if(PIR1bits.RCIF == 1){
        inData = get_char();
        if(inData == '*')
        {
            state = RUNNING;
            //__delay_ms(delay_time);
        }

        else if(inData == '#'){
            state = LISTENING;
        }

        else if(inData == '$'){
            state = TESTING;
        }
        
        else{
            gotcha = insertData(rbPtr, inData);
            
            if(gotcha == -1){
                PORTDbits.RD1 = 0;
                PORTDbits.RD1 = 1;
                //__delay_ms(1000);
                
                PORTDbits.RD1 = 0;
            }
            
            else {
                PORTDbits.RD0 = 1;
                PORTDbits.RD1 = 1;
                __delay_ms(100);

                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 0;
                __delay_ms(100);
            }
        }
    }
    
    
    if(INTCON3bits.INT2F == 1){        //ocorreu uma interrupção 
        if(INTCON2bits.INTEDG2 == 1){  //foi por borda de subida
            state = RUNNING;
            INTCON3bits.INT2F   = 0;
            INTCON3bits.INT2E   = 1;
            INTCON2bits.INTEDG2 = 0;
            //PORTDbits.RD1 = 0;
            //PORTDbits.RD0 = 1;
        }
        
        else if(INTCON2bits.INTEDG2 == 0){
            state = LISTENING;
            INTCON3bits.INT2F   = 0;
            INTCON3bits.INT2E   = 1;
            INTCON2bits.INTEDG2 = 1;
            //PORTDbits.RD1 = 1;
            //PORTDbits.RD0 = 0;
        }
        
    }

    //if(PIR1bits.TXIF) PIR1bits.TXIF = 0;
    
    //if(PIR1bits.TMR1IF == 1){}
}

void send_char(char ch)  
{
    TXSTAbits.TXEN = 1;
    TXREG = ch;
    while(!TXSTAbits.TRMT);
    while(!PIR1bits.TXIF);
    TXSTAbits.TXEN = 0;
}



void putch(char data)
{
    while(!TXIF);
    TXREG = data;
}

char get_char(void)
{
    if(RCSTAbits.OERR) // check for over run error 
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1; //Reset CREN
    }
    
    if(PIR1bits.RCIF==1) //if the user has sent a char return the char (ASCII value)
    {
        while(!PIR1bits.RCIF);  
        return RCREG;
    }
    else //if user has sent no message return 0
        return 0;
}




void main(){
    
    ADC_init();
    USART_init();
    INT_init();
    setup_IO_pins();
    setup_power_pwm();
    rbPtr = &msg;
    
    turnMotors(ON);
    stopMotors();
    while (1){

        if(state == LISTENING){
            stopMotors();
            
            PORTDbits.RD1 = 1;
            PORTDbits.RD0 = 0;
        }
        
        else if(state == RUNNING){
                        
            /*
     * Sensores individuais
     */
            
    //Sensor linha frontal esquerda
            
    if(PORTCbits.RC3 == 0 && 
       PORTCbits.RC4 == 1
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
                    PORTCbits.RC4 == 0
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
            
            
            
            /*
             * Sensores em pares
             */
            
            // os dois frontais
            else if(PORTCbits.RC3 == 0 && 
                    PORTCbits.RC4 == 0
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
            
            
            
            else if(PORTCbits.RC3 == 1 && 
                    PORTCbits.RC4 == 1 
                    ){
                
                if(PORTAbits.RA0 == 1 &&
                   PORTAbits.RA1 == 1 &&
                   PORTCbits.RC0 == 1 &&
                   PORTAbits.RA2 == 1 &&
                   PORTAbits.RA4 == 1 ){
                    moveMotor(right, forward, 60);
                    moveMotor(left, forward, 60);
                }
                else if(PORTAbits.RA0 == 0 &&
                        PORTAbits.RA1 == 1 &&
                        PORTCbits.RC0 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA4 == 1 ){
                    moveMotor(right, backward, 80);
                    moveMotor(left, forward, 80);
                    __delay_ms(50);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTCbits.RC0 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA4 == 1 ){
                    moveMotor(right, backward, 100);
                    moveMotor(left, forward,   100);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTCbits.RC0 == 1 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA4 == 1 ){
                    moveMotor(right, forward, 80);
                    moveMotor(left, backward, 80);
                    __delay_ms(50);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTCbits.RC0 == 1 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA4 == 0 ){
                    moveMotor(right, forward, 100);
                    moveMotor(left, backward, 100);
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTCbits.RC0 == 0 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA4 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTCbits.RC0 == 1 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA4 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTCbits.RC0 == 0 &&
                        PORTAbits.RA2 == 1 &&
                        PORTAbits.RA4 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 1 &&
                        PORTCbits.RC0 == 0 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA4 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                }
                
                else if(PORTAbits.RA0 == 1 &&
                        PORTAbits.RA1 == 0 &&
                        PORTCbits.RC0 == 0 &&
                        PORTAbits.RA2 == 0 &&
                        PORTAbits.RA4 == 1 ){
                    //moveMotor(right, forward, 100);
                    //moveMotor(left, forward, 100);
                    stopMotors();
                } 
                else{
                    stopMotors();
                }
            }
        }
        
        
        //Espaço de testes
        else if(state == TESTING){
            
            moveMotor(right, forward, 60);
            moveMotor(left,  forward, 60);
            __delay_ms(500);
            stopMotors();
            //__delay_ms(500);
            moveMotor(right, backward, 60);
            moveMotor(left,  backward, 60);
            __delay_ms(500);
            stopMotors();
            //__delay_ms(500);
            if(inData == 'a'){
                PORTDbits.RD1 = 1;
                PORTDbits.RD0 = 1;
            }
            
            else if(inData == 'b'){
                PORTDbits.RD1 = 0;
                PORTDbits.RD0 = 1;
            }
            
            else if(inData == 'c'){
                PORTDbits.RD1 = 1;
                PORTDbits.RD0 = 0;
            }
            /*
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 1;
            
            if(PORTEbits.RE1 == 0){
                moveMotor(right, forward, 50);
            }
                
            else if(PORTCbits.RC3 == 0){
                moveMotor(left, forward, 50);
            }
                        
            else if(PORTCbits.RC4 == 0){
                moveMotor(right, forward, 50);
                moveMotor(left, forward, 50);
            }
            else if(PORTCbits.RC5 == 0){
                moveMotor(right, forward, 50);
                moveMotor(left, forward, 50);
            }
            
            else if(PORTAbits.RA2 == 0){
                moveMotor(left, forward, 50);
            }
            
            else if(PORTAbits.RA4 == 0){
                moveMotor(left, backward, 50);
            }
             * /
            
            else stopMotors();
            

            /*
            if(PORTCbits.RC1 == 0){
                
                moveMotor(left, backward, 50);
               
            }
            
            else if(PORTCbits.RC1 == 1) stopMotors();
             */
        }
    }
}
