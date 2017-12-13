# Sound-Reactive-NeoPixel-strip-MBED
these are sound reactive lights driven by an MBED processor and they use a mic for analog in.
Sound Reactive Lights
Created By: Arta Razavi

Description
A microphone captures the sounds and feeds it to the MBED. Alongside the VCC and Ground pins, 
an input pin had to be soldered to feed an analog signal to an analog pin on the MBED. 
Pre-programmed patterns determine the intensity of the light emitted from the NeoPixel Strip. 
There are 7 patterns. The lights on the NeoPixel Strip are individually addressable allowing 
them to be programmed to be sound reactive. 

How To Use
Step 1: Select pattern using next and back buttons on board
Step 2: Make sound 

Pattern 1: Blue and Yellow					
Pattern 2: Rainbow
Pattern 3: Pink Blue Hue
Pattern 4: Princess Bubblegum
Pattern 5: Amplitude
Pattern 6: Double Amplitude
Pattern 7: Crawling Ants

button #1 = prev
button #2 = next
If Frozen reset MBED and press next button (button #2)


What I Learned
Detecting sound from a mic and measuring signal strength. Addressing lights individually 
and writing patterns for 144 sequential lights. Learned how to use the NeoPixel and the 
Microphone libraries on MBED. I learned how to debug code on remote MBED processor. 
