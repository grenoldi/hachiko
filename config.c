/*
 * File:   config.h
 * Author: Guilherme "Bob" Renoldi
 *
 * Created on 26 de Maio de 2018, 17:52
 */


#include <xc.h>
#include "config.h"

void ADC_init(){
    ADCON1 = 0b00000000;
    ADCON2 = 0b10011101;
    ADCON3 = 0b00000000;
}

void USART_init(){
    TXSTA   = 0b00000110;
    RCSTA   = 0b10010000;
    BAUDCTLbits.BRG16 = 0;
    SPBRG   = 129;
    SPBRGH  = 0;
}

void INT_init(){
    RCONbits.IPEN = 0;
    INTCON  = 0b11000000;
    INTCON2 = 0b00010000; //lembrar de setar o bit 2 para os sensores de linha terem alta prioridade
    INTCON3 = 0b00010000;
    PIE1    = 0b00110000;
    PIE3    = 0b00000000;
    IPR1    = 0b00100000;
}

void setup_IO_pins(){
    ANSEL0 = 0b00000000; //registrador para ativar/destivar ADC: 1 ANALOG / 0 DIGITAL
    ANSEL1 = 0b00000000;
    TRISA  = 0b11111111;
    TRISB  = 0b00000000;
    TRISC  = 0b11111111;
    TRISD  = 0b00000000;
    TRISE  = 0b00000111;
}

void setup_power_pwm(){
    PTCON0  = 0b00000000;
    PTCON1  = 0b10000000;
    PWMCON0 = 0b01011111;
    PWMCON1 = 0b00000001;
    PTPERL  = 0b11111111;
    PTPERH  = 0b00000000;
    DTCON   = 0b00010000;
    OVDCONS = 0b00000000;
    OVDCOND = 0b01010101;
    PORTB               =0x00;
    TRISBbits.TRISB0    =0x00; //	PWM0
    TRISBbits.TRISB1    =0x00; //	PWM1

    TRISBbits.TRISB2    =0x00; //	PWM2
    TRISBbits.TRISB3    =0x00; //	PWM3

    TRISBbits.TRISB5    =0x00; //	PWM4
    TRISBbits.TRISB4    =0x00; //	PWM5

    TRISDbits.TRISD6    =0x00; //	PWM6
    TRISDbits.TRISD7    =0x00; //	PWM7

    PTMRL   =0X00;			//	PWM Time Base (lower 8 bits)
    PTMRH   =0X00;			//	PWM Time Base (upper 4 bits)
}

