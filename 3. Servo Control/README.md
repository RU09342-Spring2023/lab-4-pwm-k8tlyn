# Explanation

I am using SMCLK in upmode.

Every time P2.3 is pressed the servo should move 18° degrees left, and when P4.1 is pressed it should move 18° degrees right.  I figured these numbers out from the datasheet for a 9 g micro-servo. The datasheet said that the PWM should have a period of 50Hz or 20ms. It also said that:
 *1ms duty cycle will make servo rotate -90°    
 *1.5ms duty cycle will make servo rotate 0°
 *2ms duty cycle will make servo rotate 90°

Based on these numbers I calculated (in top comment of main) that TB3CCR0 should be 20000 in order to have 20 ms period and TB3CCR1 will range from 1000-2000 in order to make it turn.

When P2.3 is pressed, as long as CCR1 > 1000 the servo will move left 18° (CCR1 -=100)
When P4.1 is pressed, as long as CCR1 < 2000 the servo will move right 18° (CCR1 +=100)
