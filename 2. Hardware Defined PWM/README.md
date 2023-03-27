# Hardware PWM - Part 2


Timer 3 is set to SMCLK in up mode. It counts up to CCR0 = 1000. P6.0 connects to the red LED w/ duty cycle in CCR1, P6.1 connects to the green LED w/ duty cycle in CCR2, and P6.2 connects to the blue LED w/ duty cycle in CCR3. 

In order to cycle through:

- Red
- Orange (Red + Green)
- Green
- Cyan (Green + Blue)
- Blue
- Purple (Red + Blue)

Initially, red is on while green and blue are off, then green increases and red increases (eventually orange is formed) until only green is on. Then green begins to decrease and blue begins to increase (forming cyan during the process) until only blue is on. Finally, blue is decreased while red is increased (purple is in there somewhere) until only red is on and process repeats.