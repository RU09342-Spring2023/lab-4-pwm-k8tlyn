
/** By: Kaitlyn Pounds
 *
You will need to use pins 6.0, 6.1, and 6.2 to drive an RGB LED.

These will need to be configured with a PWM Period of 1ms.

 You need your RGB LED to cycle between the following colors in order:

Red
Orange (Red + Green)
Green
Cyan (Green + Blue)
Blue
Purple (Red + Blue)

You need to cover colors in between them, meaning as you transition from Red to Orange, it shouldn't be just 2 colors.

The amount of colors are up to you, but is needs to appear smooth in transition.

The timing for cycling is up to you to determine as well.

 */

#include <msp430.h> 

void pinSetup();
void TimerB0Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        pinSetup();
        TimerB0Setup(); // Initialize Timer0
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

void pinSetup(){
    P6OUT &= ~BIT0;
    P6DIR |= BIT0;
    P6OUT &= ~BIT1;
    P6DIR |= BIT1;
    P6OUT &= ~BIT2;
    P6DIR |= BIT2;
}
void TimerB0Setup(){
        // Configure Timer_B0
            TB0CTL = TBSSEL_2 | MC__UP | TBCLR | TBIE;      // SMCLK, up mode, clear
            TB0CCTL0 = OUTMOD_7;                          // CCR1 reset/set
        //TBR, enable interrupt
            TB0CCTL1 |= CCIE;                             // Enable TB0 CCR2 Interrupt
            TB0CCTL2 |= CCIE;                             // Enable TB0 CCR3 Interrupt
            TB0CCR0 = 1000;                               //1kHz frequency
            TB0CCR1 = 500;                          //initial duty cycle (50%) for red LED
            TB0CCR2 = 500;                          //same but for green LED
}

// Timer0 B0 interrupt service routine
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void)
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
      {
        case  0:                              //no interrupt pending
            break;
        case  TB0IV_TBCCR1:                   // CCR1
            P1OUT &= ~BIT0;                   //set P1.0 to 0
            break;
        case  TB0IV_TBCCR2:                   // CCR2
            P6OUT &= ~BIT6;                   //set P6.6 to 0
            break;
        case  TB0IV_TBIFG:                    // TBIFG
            P1OUT ^= BIT0;                    //toggle red LED
            P6OUT ^= BIT6;                    //toggle green LED
            break;
        default:
            break;
      }
}


