
/** By: Kaitlyn Pounds

 * According to datasheet for 9g microservo using a 50 Hz (20 ms) PWM       --> TB3CCR0 = 20000
       ~1ms duty cycle will make -90°     1ms dutycycle/20ms period=5%            5% OF 20000 --> TB3CCR1 = 1000
       ~1.5ms duty cycle will make 0°     1.5ms dutycycle/20ms period=7.5%      7.5% OF 20000--> TB3CCR1 = 1500
       ~2ms duty cycle will make 90°          2ms dutycycle/20ms period=10%      10% OF 20000--> TB3CCR1 = 2000
 * Button 2.1 will make duty cycle -=100
 * Button 4.3 will make duty cycle +=100
 */

#include <msp430.h> 

void pinSetup();
void ButtonSetup();
void TimerB3Setup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        pinSetup(); // Initialize our pin
        ButtonSetup();  // Initialize our button
        TimerB3Setup(); // Initialize Timer0
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

void pinSetup(){
    P6OUT &= ~BIT0;
    P6DIR |= BIT0;             //output pin that servo will connect to
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;           //this pin looks at TB3.1
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

void TimerB3Setup(){
        // Configure Timer_B3
            TB3CTL = TBSSEL_2 | MC__UP | TBCLR | TBIE;      // SMCLK, up mode, clear
            TB3CCTL0 = OUTMOD_7;                          // CCR1 reset/set
        //TBR, enable interrupt
            TB3CCTL1 |= CCIE;                             // Enable TB0 CCR2 Interrupt
            TB3CCR0 = 20000;                               //50Hz frequency
            TB3CCR1 = 1500;                               //initial duty cycle of 1.5 ms
}
// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG

    if(TB3CCR1 > 1000)
        TB3CCR1 -= 100;                             //change position by 18° left
    else
        TB3CCR1 = TB3CCR1;                          //servo can't move further than -90°, TB3CCR1 < 1000 is past the range of the servo,
}                                                   //so at CCR1 = 1000 the value remains the same

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG

    if(TB3CCR1 < 2000)
        TB3CCR1 += 100;                             //change position by 18° right
    else
        TB3CCR1 = TB3CCR1;                          //servo can't move further than 90°, TB3CCR1 > 2000 is past the range of the servo,
}                                                   //so at CCR1 = 2000 the value remains the same
}

// Timer3 B1 interrupt service routine
#pragma vector = TIMER3_B1_VECTOR
__interrupt void Timer3_B1_ISR(void)
{
    switch(__even_in_range(TB3IV,TB3IV_TBIFG))
      {
        case  TB3IV_TBCCR1:                   // CCR1
            P6OUT &= ~BIT0;                   //set P6.0 to 0
            break;
        default:
            break;
      }
}


