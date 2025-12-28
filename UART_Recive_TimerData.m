% Define the serial port object (adjust 'COM1' to your port and settings)
clc;  clearvars;
clear serialPort;
port = 'COM3';
baudRate = 115200;
Freq = 200;
fileName = ['SineWaveResponse18March_FreqVDDL0p6', num2str(Freq), '.mat'];
%baudRate = 230400;
% Create the serialport object
serialPort = serialport(port, baudRate);

% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds

try
    % Wait for the first byte to be available
    while serialPort.NumBytesAvailable == 0
        pause(0.1); % Small pause to prevent tight loop
    end

    % Read the first byte (you can choose to discard or use it)
    %length = read(serialPort, 1, 'uint8');
    length_data =60000;
    % Read the next 18 bytes
    Received_data = read(serialPort, length_data, 'uint8');

    % Display the received data
    disp('Received data:');
    %disp(Received_data);
    
catch exception
    % Display any errors
    disp('Error:');
    disp(exception.message);
end

% Clean up by clearing the serial port object
clear serialPort;
%% 

save(fileName,"Received_data");
%%
%% 
% Example input: a double array with 4 uint8 indices followed by 4 uint8 time values
%data = [01, 00, 00, 00, 00, 01, 00, 00]; % Replace with your actual array

% Reshape the array into 4x2, assuming the first four bytes are indices, and the next four are time values
data = uint8(Received_data); % Ensure the data is in uint8
reshapedData = reshape(data, 8, []).';

% Convert the 4 uint8 numbers to uint32 considering the 4th byte as MSB
neuron_id = reshapedData(:, 1); % Extract the first 4 bytes
times = reshapedData(:, 5:end); % Extract the next 4 bytes
%%
%index_uint32 = zeros(size(neuron_id,1),1);
time_uint32 = zeros(size(times,1),1);
for i = 1:numel(neuron_id)
    %index_uint32(i) = typecast(uint8((indices(i,:))), 'uint32'); % Convert to uint32
    time_uint32(i) = typecast(uint8((times(i,:))), 'uint32'); % Convert to uint32
end
%%
%if time_uint32 ~= 0
    time = 100000000 - time_uint32;
%end
%% 
figure; % Create a new figure
plot(time*1e-8, neuron_id, 'k.', 'MarkerSize', 5); % Raster plot using dots
% Formatting the plot
xlabel('Time');
ylabel('Neuron Index');
title('Raster Plot');

ylim([0 35]); % Set y-axis limits
%xlim([5/5000 15/5000])
set(gca, 'YDir', 'reverse'); % Optional: Invert y-axis to align with convention
grid on;