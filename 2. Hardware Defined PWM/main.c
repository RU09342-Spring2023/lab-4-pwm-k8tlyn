
/** By: Kaitlyn Pounds
 *
You will need to use pins 6.0, 6.1, and 6.2 to drive an RGB LED.

These will need to be configured with a PWM Period of 1ms. = 1kHz

 You need your RGB LED to cycle between the following colors in order:

Red                   R = 50% G = 0%  B = 0%
Orange (Red + Green)  R = 50% G = 50% B = 0%
Green                 R = 0%  G = 50% B = 0%
Cyan (Green + Blue)   R = 0%  G = 50% B = 50%
Blue                  R = 0%  G = 0%  B = 50%
Purple (Red + Blue)   R = 50% G = 0%  B = 50%

You need to cover colors in between them, meaning as you transition from Red to Orange, it shouldn't be just 2 colors.

The amount of colors are up to you, but is needs to appear smooth in transition.

The timing for cycling is up to you to determine as well.

 */

#include <msp430.h> 

void pinSetup();
void TimerB0Setup();
void changeColor();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        pinSetup();
        TimerB3Setup(); // Initialize Timer0
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

void pinSetup(){
    P6OUT &= ~BIT0;         //set pins for RGB output
    P6DIR |= BIT0;          //P6.0 uses TB3.1
    P6OUT &= ~BIT1;
    P6DIR |= BIT1;          //P6.1 uses TB3.2
    P6OUT &= ~BIT2;
    P6DIR |= BIT2;          //P6.2 uses TB3.3
}
void TimerB3Setup(){
        // Configure Timer_B3
            TB3CTL = TBSSEL_2 | MC__UP | TBCLR | TBIE;      // SMCLK, up mode, clear
            TB3CCTL1 = OUTMOD_7;                          // period reset/set
        //TBR, enable interrupt
            TB0CCTL1 |= CCIE;                             // Enable TB3CCR1 Interrupt
            TB3CCR0 = 1000;                               //1kHz frequency
            TB3CCR1 = 500;                                //50% duty cycle

}
void changeColor(){

}

// Timer0 B0 interrupt service routine
#pragma vector = TIMER3_B0_VECTOR
__interrupt void Timer3_B0_ISR(void)
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
      {
        case  0:                              //no interrupt pending
            break;
        case  TB0IV_TBCCR1:                   // CCR1

            break;
        case  TB0IV_TBIFG:                    // TBIFG
            changeColor();                   //change color of light
            break;
        default:
            break;
      }
}


