clc; clearvars;
clear serialPort;
port = 'COM3';
baudRate = 115200;
% Create the serialport object
serialPort = serialport(port, baudRate);
% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds
%% Loading Input Signal from TI-46 dataset
load("TI46-IF.mat");

%% Converting to 16bit value
AudioData = struct();
signalId = 6;
DC_bias = 0.3;
AttenuationFactor = 0.2; 
signal = -(DC_bias+ AttenuationFactor.*DATA(signalId).sig);
for i=1:21248
[AudioData(i).MSB,AudioData(i).LSB] = float2Uint16(-DC_bias, -2.5, 2.5);
end

N = length(signal);
for i=1:N
[AudioData(i).MSB,AudioData(i).LSB] = float2Uint16(signal(i), -2.5, 2.5);
end
%%
NoOfBytesToSend = 2*numel(AudioData); %MSB + LSB
DATA_mat = cell2mat(struct2cell(AudioData));
DATAtoSend= reshape(DATA_mat,1,NoOfBytesToSend); %(MSB,LSB)
%sample_id = 6;
%DATA_LSB = DATA(sample_id).LSB;
%DATA_MSB = DATA(sample_id).MSB;
%NoOfBytesToSend = numel(DATA_LSB)+numel(DATA_MSB);
%DATAtoSend = reshape([DATA_LSB';DATA_MSB'],1,NoOfBytesToSend);
%NoOfBytesToSend_MSB = uint8(floor(NoOfBytesToSend/2^8));
%NoOfBytesToSend_LSB = uint8(NoOfBytesToSend -(2^8).*double(NoOfBytesToSend_MSB));
%% 
try
    % Write the 18 bytes of data to the serial port
    %write(serialPort, uint8(NoOfBytesToSend_LSB), 'uint8');
    %write(serialPort, uint8(NoOfBytesToSend_MSB), 'uint8');
    write(serialPort, DATAtoSend, 'uint8');

    % Display a message indicating successful data transmission
    disp('Data sent successfully:');
  

catch exception
    % Display any errors
    disp('Error:');
    disp(exception.message);
end
fileName = ['audioresponse_sample_id', num2str(sampleID), '.mat'];

% Clean up by clearing the serial port object
clear serialPort;