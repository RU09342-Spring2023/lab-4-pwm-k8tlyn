
/** By: Kaitlyn Pounds
 *
 * You need to generate a 1kHz PWM signal with a duty cycle between 0% and 100%.
 * Upon the processor starting up, you should PWM both of the on-board LEDs at a 50% duty cycle.
 * Upon pressing the on-board buttons, the duty cycle of the LEDs should increase by 10%, based on which button you press.
 * Once you have reached 100%, your duty cycle should go back to 0% on the next button press.

Button 2.1 Should control LED 1.0
Button 4.3 should control LED 6.6
 */

#include <msp430.h> 

void LEDSetup();
void ButtonSetup();
void TimerB0Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        LEDSetup(); // Initialize our LEDS
        ButtonSetup();  // Initialize our button
        TimerB0Setup(); // Initialize Timer0
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

void LEDSetup(){
    // Configure RED LED on P1.0 as Output
             P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
             P1DIR |= BIT0;                          // Set P1.0 to output direction
    // Configure Green LED on P6.6 as Output
             P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
             P6DIR |= BIT6;                          // Set P6.6 to output direction
}

void ButtonSetup (){
    // Configure Button on P2.3 as input with pullup resistor
             P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
             P2REN |= BIT3;                          // P2.3 pull-up register enable
             P2IES &= ~BIT3;                         // P2.3 Low --> High edge
             P2IE |= BIT3;                           // P2.3 interrupt enabled

    // Configure Button on P4.1 as input with pullup resistor
             P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
             P4REN |= BIT1;                          // P4.1 pull-up register enable
             P4IES &= ~BIT1;                         // P4.1 Low --> High edge
             P4IE |= BIT1;                           // P4.1 interrupt enabled

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


// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG

    if(TB0CCR1 >= 1000)
        TB0CCR1 = 0;                             //wrap around back to 0
    else
        TB0CCR1 += 100;                          //increment duty cycle by 10%
}

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG

    if(TB0CCR2 >= 1000)
        TB0CCR2 = 0;                             //wrap around back to 0
    else
        TB0CCR2 += 100;                          //increment duty cycle by 10%
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


