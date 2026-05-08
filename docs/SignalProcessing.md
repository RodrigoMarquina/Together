First we want to test JACK's audio thread. We cant just add a simple print because that would involve the OS and it would kill performance. So we'll test by sending the amplitude of a middle C note each sample via the callback function. For that its important to understand a few concepts first. A complete cycle of a sin wave is always 2π or a circle unwrapped over time, If JACK is sampling 48000 times each second, and a C note is 440hz for each second, that means that the C note completes 440 cycles in a second. 

This also means that for each cycle of the C note, there will be 109 samples since 48000 samples / 440 hz = 109 samples each cycle. 
- One cycle = 2π radians
- One cycle = 109 samples (at 440 Hz, 48000 sample rate)

Now the formula to get the amplitude in a specific second is:
sin(2π * frequency * t)
sin(2π * x) when x goes from 0 to 1, sin(2π * x) completes exactly one full cycle.

This also means that in 1 second, we would have completed the frequency's amount of cycles. If frequency is 440hz, then in one second we would have completed 440 cycles. We multiply by 2π to convert the fraction of time into radians and then the sin function transform radians into amplitude. 

But we dont have time, we have samples. So if 1 second is equal to 48000 samples, to get the value of time we need to divide the sample_index by sample_rate. So finally we have the final formula which is:

sin(2π * frequency * sample_index/sample_rate)

The output of sin is always between -1 and 1, representing the displacement of the wave at that moment — positive is compression, negative is rarefaction.