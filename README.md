# Decoding
This repository provides code and instructions for decoding SOC-i's telemetry beacon.
# Requirements
1. An IQ baseband recording of SOC-i's beacon with a sufficiently high signal to noise ratio.
2. Audacity (https://www.audacityteam.org/download/), or an equivalent audio editing software.
3. Octave (https://www.gnu.org/software/octave/download) or Matlab.
# Testing
An IQ baseband recording of a telemetry beacon from SOC-i (transmitted in our lab) called beacon.wav is included in this repository. To check the decoding software, download all the required software, download the appropriate folder based on what software you are using, make sure beacon.wav is in the path, and run the code. Then you can check that the telemetry values from the code match the values in telemtry.mat. 
# Decoding
## Trimming the wav file
The wav file should be trimmed to only include one telemetry packet, and not much else. The packet is indicated by a 0.2 second long spike in the magnitude of the IQ data. This can be done in Audacity or in Matlab/Octave using indexing. The image below shows what a trimmed beacon looks like in Audacity: ![image](https://user-images.githubusercontent.com/77480142/161694635-51c778af-e72b-4da8-9860-8d6b158166d4.png) This saves computation time.
## Centering the peak frequency
The peak frequency should be centered at 0 Hz. This might not be the case because of doppler shifting or bias in the radio. To check the center frequency, open the recording in Audacity, hit ctrl-a, then click analyze -> plot spectrum. If the peak frequency is not at 0, then change the parameter dc_f to shift the center frequency by a certain amount. Since audacity does not show the sign of the error in the center frequency, you will have to guess the sign (positive or negative). Then if the decoding does not work, try changing the sign. Freqz.m can also be used if Matlab's signal processing toolbox is available.
## Demodulation
The demodulation code should automatically detect the appropriate gains to distinguish between 0's and 1's. If not, please submit an issue including the IQ recording.
## Decoding
The 1's and 0's are automatically decoded and the telemetry values are saved in a struct called telemetry.mat. The decoding process includes de-interleaving, and converting bytes into single and double precision floats and integers. More information about this can be found at aact.space. 
## Submitting Data
Please email this data to aact@uw.edu, with your latitude, longitude, and altitude. We really appreciate your help in getting extra data from SOC-i!


