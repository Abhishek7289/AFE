clc; clearvars;
clear serialPort;
port = 'COM3';
baudRate = 115200;
% Create the serialport object
serialPort = serialport(port, baudRate);
% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds
%%
   %% DATA reception
   % Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
%serialPort.Timeout = 3; % Timeout in seconds

try
    % Wait for the first byte to be available
    while serialPort.NumBytesAvailable == 0
        pause(0.1); % Small pause to prevent tight loop
    end

    % Read the first byte (you can choose to discard or use it)
    length_data =60000;
    Received_data = read(serialPort, length_data, 'uint8');
    % Display the received data
    disp('Received data:');
    
catch exception
    % Display any errors
    disp('Error:');
    disp(exception.message);
end

% Clean up by clearing the serial port object
clear serialPort;
signalFreq = 100; 
fileName = ['./TestRespo_6Mar/audioresponse_sample_freq', num2str(signalFreq), '.mat'];
%% 
save(fileName,"Received_data");
%%
%% 
data = uint8(Received_data); % Ensure the data is in uint8
reshapedData = reshape(data, 8, []).';
% Convert the 4 uint8 numbers to uint32 considering the 4th byte as MSB
neuron_id = reshapedData(:, 1); % Extract the first byte
times = reshapedData(:, 5:end); % Extract the next 4 bytes
%%
%index_uint32 = zeros(size(neuron_id,1),1);
time_uint32 = zeros(size(times,1),1);
for i = 1:numel(neuron_id)
    %index_uint32(i) = typecast(uint8((indices(i,:))), 'uint32'); % Convert to uint32
    time_uint32(i) = typecast(uint8((times(i,:))), 'uint32'); % Convert to uint32
end
%%
time = 169984000-time_uint32;
%% 
figure; % Create a new figure
plot(time*1e-8, neuron_id, 'k.', 'MarkerSize', 5); % Raster plot using dots
% Formatting the plot
xlabel('Time');
ylabel('Neuron Index');
title('Raster Plot');

ylim([0 36]); % Set y-axis limits
%xlim([5/5000 15/5000])
set(gca, 'YDir', 'reverse'); % Optional: Invert y-axis to align with convention
grid on;