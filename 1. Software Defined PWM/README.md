# Software PWM Part 1
There are three TB0CCRx registers, one for the period of the clk, and one for each duty cycle controlling the brightness of the LED. TB0 is using SMCLK in up mode. 

*TB0CCR0 is set to 1000 so there is 1kHz frequency. 
*TB0CCR1 and TB0CCR2 are set to 500 so that the LEDs have 50% duty cycle initially.

The timer interrupt turns on both LEDs.

There are two button interrupts which increment TB0CCR1 and TB0CCR2 by 100 every button press, to change the duty cycle by 10%. If the value in CCR1 or CCR2 goes above 1000, it wraps back around and becomes 0.