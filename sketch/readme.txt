File size of fractal.sk = 241kB

My program uses run length encoding on the x-axis to convert each row of pixels into LINE commands.

Colour is scaled from 0-255 to RGBA by shifting bits left by 8,16 and 24 and adding them together as this will repeat the same 8 bits 3 times and greyscale uses the same value for R, G and B, I add 1 to this value for the A so the opacity is 0%.

Data is added by shifting bits right by a multiple of 6 and masking the 2 most significant bits as these are used for the data command. The maximum number of data calls to fill the 32 bit value is 6, to avoid unnecessary calls I only add a data command to the .sk file if there has been at least one bit of data added not equal to 0.
