clc; clearvars;
clear serialPort;
port = 'COM3';
baudRate = 115200;
% Create the serialport object
serialPort = serialport(port, baudRate);
% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds
%% Sampling sinewave
fin = 115.89;
N = 21248;
fs = 12500;
t = 0:1/fs:(N-1)/fs;
signal = -0.3 + (0.1).*sin(2*pi*fin.*t);
%figure;
%plot(t,signal);
%% Converting to 16bit value
DATA = [];
for i=1:N
[DATA(i).MSB,DATA(i).LSB] = float2Uint16(signal(i), -2.5, 2.5);
end
%%
NoOfBytesToSend = 2*numel(DATA); %MSB + LSB
DATA_mat = cell2mat(struct2cell(DATA));
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

% Clean up by clearing the serial port object
clear serialPort;