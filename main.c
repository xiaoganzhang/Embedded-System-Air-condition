/* 
 * File:   lab4.c
 * Author: xiaogan
 *
 * Created on November 28, 2021, 5:16 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "defines.h"
#include "hd44780.h"
#include "lcd.h"


FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);




int ADCV = 0;
int x = 0;
void in_temp() {
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX &= 0;
    ADMUX |= (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
    ADCSRA |= (1 << ADSC);
    ADCSRA |= (1 << ADIF);
    ADCV = ADC;
}

uint16_t ex_temp() {
    ADMUX &= 0;
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADIF);

    return (ADC);
}


uint8_t button1 (void){
    return !(PINC & (1<<3));
}
/* returns 1 if button 1 is pressed, connected to PC3 */

uint8_t button2 (void){
    return !(PINC & (1<<4));
}
/* returns 1 if button 2 is pressed, connected to PC4 */

uint8_t button3 (void){
    return !(PINC & (1<<5));
}
/* returns 1 if button 3 is pressed, connected to PC5 */



void LEDon(void){
    DDRB |= (1<<0);
    PORTB |= (1<<0);
}
/* turn logic high on PB0, which connects to LED */

void LEDoff(void){
    DDRB |= (1<<0);
    PORTB &= ~(1<<0);
}
/* turn logic low on PB0, turning off LED */





int main(int argc, char** argv) {
   

//    fprintf(&lcd_str ,"hello world");
   
    lcd_init();

    DDRC &= 0;
    PORTC |= ((1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));//make PC 345 as input.  
    
    DDRC &= (1<<PORTC3);
    DDRC &= (1<<PORTC4);
    DDRC &= (1<<PORTC5);

    
    
    PCICR |= (1 << PCIE0);
    ADCSRA |= (1<<ADSC);
    ADCSRA |= (1 << ADEN);
    // enable the ADC
    
    stderr = &lcd_str;
    
       
    uint16_t adc_v = ex_temp();
    float internal_tempr = 0;
    float temp = (adc_v * 5 * 100);
    float denomtr = 1024;
    float tempr = 0;
    
    uint8_t designed_temp = 22;//set the default value.
    uint8_t deadband_H = 0;
    uint8_t deadband_L = 0;
    
    in_temp();
    

    
    while (1){
        


        
        if(button1() || button2() || button3()){
            if(button2()){
                designed_temp ++;  
                //while(button2()){};
            }
            else if(button3()){
                designed_temp -- ;
                //while(button3()){};
            }
            else if (button1()){
                x ++ ;
                if (x ==2 ){
                    x = 0;// choose internal or external 
                }
            
            }
}
    
        

 

        internal_tempr =0.078*ADCV + 24.3;
        //linear function that i found.
        fprintf(stderr, "%2.1f\xDF", internal_tempr);
        fprintf(stderr, "C");
        
        //print the internal temperature
        
        
        
        
        if (x == 0){
                fprintf(stderr, "->");
        //seperated 
        }
        if (x == 1){
                fprintf(stderr, "<-");
        }
        
        

        adc_v = ex_temp();
        temp = (adc_v * 5 * 100);
        
        tempr = temp / denomtr;
        deadband_H = tempr +2;
        deadband_L = tempr -2;
        fprintf(stderr, " %2.1f\xDF", tempr);
        fprintf(stderr, "C ");
      
        fprintf(stderr , "\x1b\xc0");
      
        
        fprintf(stderr, "     %d\xDF C   ", designed_temp);
  
        
        if(designed_temp > deadband_H){
            LEDon();
            fprintf(stderr , "ON \x1b\xc0 ");
            fprintf(stderr , "\x1b\x02");
        
        }
        
 
        if(designed_temp < deadband_L){
            LEDoff();
            fprintf(stderr , "OFF\x1b\xc0");
        
        }
        
        
     
        fprintf(stderr , "\x1b\x02");
        
        
        
        
        
        
        
        
        
        
     _delay_ms(100);   
    
        

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
      
     }
   
   
   
   
   
   
    
   
       
 
   
   
   
   
   
   
   

   
    return (EXIT_SUCCESS);
}