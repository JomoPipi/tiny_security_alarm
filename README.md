
# tiny_security_alarm
A tiny security alarm

It's a tiny, Arduino based, versatile alarm. An LDR (light dependent resistor) triggers an annoying sound if there is enough change.

There are three stages: collection, standby, and alert. During collection it gathers around 200 samples of light. During standby it compares the current value of light with the average. If there is a significant change, it moves to stage 3. Stage 3 is an annoying sound that doesn't stop until you turn it off.
