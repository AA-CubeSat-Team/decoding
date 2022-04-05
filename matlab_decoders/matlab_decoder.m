%% Decoder for SOC-i's Telemetry Beacon
% This file loads in a wav file and decodes it. Instructions can be found
% be on the AACT's github (https://github.com/AA-CubeSat-Team/decoding),
% and more information about SOC-i can be found at the AACT website
% (AACT.space). Please use the "Run Section" button to run the sections
% below separately.
%
% Nick Melville
% UW Aero Astro Cubesat Team

%% Load in file
% Please comment out the file that you want to decode. Also note that the
% file must be in your matlab path.
clear all; clc; 

IQ_data = audioread('beacon.wav');

% Plot to determine where the packet starts and ends, if the wav file is
% not already trimmed to only include the packet.
plot(IQ_data)
%% Trim the file
% To reduce unnecessary computations, the wav file should be trimmed to get
% rid of long periods of silence before and after the packet transmission.
% The packet is indicated by the obvious spike in the plot of the IQ data. 
% This step is not necessary if the file is already trimmed using another 
% software, such as audacity. 

IQ_data = audioread('beacon.wav');

% Adjust these values as needed to only include one packet
start_ind = 1;
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

%s = s.*transpose(exp(-j*2*pi*0.462E6*[1:1:length(s)]/fs));

% Find center frequency using freqz or some other software such as Audacity
% (freqz requires the signal processing toolbox)
%freqz(s,1,[-10E5:1E3:10E5],fs);


%% Shift the Center Frequency to Zero
% Define how far the center frequency needs to be shifted (change as
% needed).
dc_f = -0.298E6;

% Shift the IQ signal
s = s.*transpose(exp(j*2*pi*dc_f*[1:1:length(s)]/fs));

% Recheck center frequency (it should be centered now)
%freqz(s,1,[-10E5:1E3:10E5],fs);

%% Demodulate
ds = zeros(length(s) - 1,1);
for i = 1:length(s)-1
    
    ds(i) = conj(s(i,:))*s(i+1,:);
    
end

ang = movmean(angle(ds),100);
mag = movmean(abs(ds),100);

mag_mean = mean(mag);


for i = 1:length(ds)
    if (mag(i) > 0.8*mag_mean)
        tstart = i;
        break
    end
end
for i = tstart:length(ds)
    if (mag(i) < 0.8*mag_mean)
        tend = i;
        break
    end
end

ang = ang(tstart:tend);
mag = mag(tstart:tend);

ang_max = 0;
for i = 1:10*fs/4800
    ang_max = max(ang_max,ang(i));
end

for i = 1:length(ang)
    if ang(i) > 0.98*ang_max
    bstart = i;
    break
    end
end

b_ind = bstart:fs/4800:length(ang);

bits_f = ang(b_ind);

f_ave = mean(bits_f);

bits = zeros(length(bits_f),1);
for i = 1:size(bits_f)
    if bits_f(i) > f_ave
        bits(i) = 1;
    else
        bits(i) = -1;
    end
end

%% Decoding
preamble = repmat([1;1;-1;-1],[20,1]);
for i = 80:length(bits)
    if (sum(bits(i-79:i).*preamble) > 76)
        c = i;
    end
end

message = zeros(54,8);

ds = c + [5 165 325];

for i = 1:3
    d = ds(i);
    
    m = bits(d:d+160);
    
    for j = 1:160
        if m(j) == -1
            m(j) = 0;
        end
    end
    
    m2 = zeros(144,1);
    for k = 1:8
        for p = 1:18
            m2((p-1)*8 + k) =  m(2+(k-1)*20 + p);
        end
    end
    
    m = reshape(m2,[8 18])';
    
    message(1 + (i-1)*18:18 + (i-1)*18,:) = m(1:18,:);
end


%% Parse Telemetry

gnc_mode = bin2dec(num2str(message(6,:)))

% Why is there no matlab function to convert from binary to floating point????? :(
battery_temp =  typecast(uint32(bin2dec(num2str(reshape(message(7:10,:)',[4*8,1])'))),'single')

battery_voltage =  typecast(uint32(bin2dec(num2str(reshape(message(11:14,:)',[4*8,1])'))),'single')

battery_current =  typecast(uint32(bin2dec(num2str(reshape(message(15:18,:)',[4*8,1])'))),'single')

battery_charge =  typecast(uint32(bin2dec(num2str(reshape(message(19:22,:)',[4*8,1])'))),'single')

time =  typecast(uint64(bin2dec(num2str(reshape(message(23:30,:)',[8*8,1])'))),'double')

rate1 =  typecast(uint32(bin2dec(num2str(reshape(message(31:34,:)',[4*8,1])'))),'single');
rate2 =  typecast(uint32(bin2dec(num2str(reshape(message(35:38,:)',[4*8,1])'))),'single');
rate3 =  typecast(uint32(bin2dec(num2str(reshape(message(39:42,:)',[4*8,1])'))),'single');
rate = [rate1; rate2; rate3]

quat2 = typecast(uint32(bin2dec(num2str(reshape(message(43:46,:)',[4*8,1])'))),'single');
quat3 = typecast(uint32(bin2dec(num2str(reshape(message(47:50,:)',[4*8,1])'))),'single');
quat4 = typecast(uint32(bin2dec(num2str(reshape(message(51:54,:)',[4*8,1])'))),'single');
quat = [sqrt(1 - quat2^2 - quat3^2 - quat4^2); quat2; quat3; quat4]

%% Package telemetry
beacon_telemetry = struct;

beacon_telemetry.gnc_mode = gnc_mode;
beacon_telemetry.battery_temp = battery_temp;
beacon_telemetry.battery_voltage = battery_voltage;
beacon_telemetry.battery_current = battery_current;
beacon_telemetry.battery_charge = battery_charge;
beacon_telemetry.time = time;
beacon_telemetry.rate = rate;
beacon_telemetry.quat = quat;

save('telemetry.mat','beacon_telemetry');

