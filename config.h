#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#include <pic18f4431.h>

//Contant definitions
#define _XTAL_FREQ 20000000
//function prototypes

void ADC_init();
void USART_init();
void INT_init();
void setup_IO_pins();
void setup_power_pwm();


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */

#endif	/* XC_HEADER_TEMPLATE_H */

