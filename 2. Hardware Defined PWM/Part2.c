
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
void TimeB3Setup();
void changeColor();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        pinSetup();
        TimeB3Setup(); // Initialize Timer0
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

void pinSetup(){
    P6OUT &= ~BIT0;                       //set pins for RGB output
    P6OUT &= ~BIT1;
    P6OUT &= ~BIT2;
    P6DIR |= BIT0 + BIT1 + BIT2;          //P6.0, P6.1, and P6.2 uses TB3.1

}
void TimeB3Setup(){
        // Configure Timer_B3
            TB3CTL = TBSSEL_2 | MC__UP | TBCLR | TBIE;      // SMCLK, up mode, clear
        //Output Mode
            TB3CCTL1 = OUTMOD_7;                          // period reset/set
            TB3CCTL2 = OUTMOD_7;                          // period reset/set
            TB3CCTL3 = OUTMOD_7;                          // period reset/set
        //enable interrupt
            TB3CCTL1 |= CCIE;                             // Enable TB3CCR1 Interrupt
            TB3CCTL2 |= CCIE;                             // Enable TB3CCR1 Interrupt
            TB3CCTL3 |= CCIE;                             // Enable TB3CCR1 Interrupt
        //TBxCCR values set
            TB3CCR0 = 1000;                               //1kHz frequency
            TB3CCR1 = 500;                                //50% duty cycle for RED
            TB3CCR2 = 0;                                //0% duty cycle for GREEN
            TB3CCR3 = 0;                                //0% duty cycle for BLUE

}

// Timer3 B1 interrupt service routine
#pragma vector = TIMER3_B1_VECTOR
__interrupt void Timer3_B1_ISR(void)
{
    switch(__even_in_range(TB3IV, TB3IV_TBIFG))
      {
        case  0:                              //no interrupt pending
            break;
        case  TB3IV_TBCCR1:                   // CCR1
            P6OUT &= ~BIT0;                   //set P6.0 to 0
            break;
        case  TB3IV_TBCCR2:                   // CCR2
            P6OUT &= ~BIT1;                   //set P6.1 to 0
            break;
        case  TB3IV_TBCCR3:                   // CCR3
            P6OUT &= ~BIT2;                   //set P6.2 to 0
            break;
        case  TB3IV_TBIFG:                    // TBIFG

            P6OUT |= BIT0 + BIT1 + BIT2;      //set pins to output

           if ((TB3CCR1 > 1) && (TB3CCR3 < 2) && (TB3CCR2 < 499)){  //as long as there is some red, no blue, and green isn't all the way on
               TB3CCR2++;           //more green                    //should start as red and then become orange and then become green
               TB3CCR1--;           //less red
           }
           else if((TB3CCR2 > 1) && (TB3CCR1 < 2) && (TB3CCR3 < 499)){ //as long as there is some green, no red, and blue isn't all the way on
               TB3CCR3++;           //more blue                     //should start as green and then become cyan and then become blue
               TB3CCR2--;           //less green
           }
           else if((TB3CCR3 > 1) && (TB3CCR2 < 2) && (TB3CCR1 < 499)){ //as long as there is some blue, no green, and red isn't all the way on
               TB3CCR1++;           //more red                      //should start as blue and then become purple and then become red
               TB3CCR3--;           //less blue
           }

            break;
        default:
            break;
      }
}



