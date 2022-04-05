# Decoding
This repository provides code and instructions for decoding SOC-i's telemetry beacon.
# Requirements
1. An IQ baseband recording of SOC-i's beacon with a sufficiently high signal to noise ratio.
2. Audacty (https://www.audacityteam.org/download/), or an equivalent audio editing software.
3. Octave (https://www.gnu.org/software/octave/download) or Matlab.

# Decoding
## Trimming the wav file
The wav file should be trimmed to only include one telemetry packet, and not much else. This can be done in Audacity. The image below shows what a trimmed beacon looks like in Audacity: ![image](https://user-images.githubusercontent.com/77480142/161694635-51c778af-e72b-4da8-9860-8d6b158166d4.png) This saves computation time.
## Centering the peak frequency
The peak frequency should be centered at 0 Hz. This might not be the because of doppler shifting or bias in the radio. To check the center frequency, open the recording in Audacity, hit ctrl-a, then click analyze -> plot spectrum. If the peak frequency is not at 0, then change the parameter  

