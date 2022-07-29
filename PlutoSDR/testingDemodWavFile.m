%% Load in file
% Please comment out the file that you want to decode. Also note that the
% file must be in your matlab path.
clear all; clc; 

IQ_data = audioread('plutosdr_matlab_cabled-69datten-take2_trimmed.wav');

% Plot to determine where the packet starts and ends, if the wav file is
% not already trimmed to only include the packet.
plot(IQ_data)
%% Trim the file
% To reduce unnecessary computations, the wav file should be trimmed to get
% rid of long periods of silence before and after the packet transmission.
% The packet is indicated by the obvious spike in the plot of the IQ data. 
% This step is not necessary if the file is already trimmed using another 
% software, such as audacity. 

IQ_data = audioread('plutosdr_matlab_cabled-69datten-take2_trimmed.wav');

% Adjust these values as needed to only include one packet
start_ind = 4364550 + 83328 + 1604;
end_ind = length(IQ_data);

IQ_data = IQ_data(start_ind:end_ind,:);

plot(IQ_data)

%% Check Center Frequency
% If is often necessary to shift the frequency to the expected frequency of 
% 435.55 MHz. This might be the case if there is doppler shift, or if your
% SDR setup has bias. It is a good idea to check that the frequency is
% centered.

% The sampling frequency of the SDR (change if needed)
fs = 2400000;

% Convert to complex number
s = IQ_data(:,1) + i*IQ_data(:,2);

s = s.*transpose(exp(-j*2*pi*0.462E6*[1:1:length(s)]/fs));

% Find center frequency using freqz or some other software such as Audacity
% (freqz requires the signal processing toolbox)
freqz(s,1,[-10E5:1E3:10E5],fs);


%% Shift the Center Frequency to Zero
% Define how far the center frequency needs to be shifted (change as
% needed).
dc_f = 466000;

% Shift the IQ signal
s = s.*transpose(exp(j*2*pi*dc_f*[1:1:length(s)]/fs));

% Recheck center frequency (it should be centered now)
freqz(s,1,[-10E5:1E3:10E5],fs);
%% Demodulating with built-in matlab function
gmskdemodulator = comm.GMSKDemodulator('BitOutput',true', 'BandwidthTimeProduct', 0.5, 'SamplesPerSymbol', 5);
reshape = IQ_data(:, 2)
demodSignal = gmskdemodulator(reshape);

%% Convert to Hex
% Use this section if you want to see what the message is in hexadecimal.
message_hex = dec2hex(bin2dec(num2str(demodSignal)))

