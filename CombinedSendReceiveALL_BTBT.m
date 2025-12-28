clc; clearvars;
clear serialPort;
port = 'COM3';
baudRate = 230400;
% Create the serialport object
serialPort = serialport(port, baudRate);
% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds
%% Loading Input Signal from TI-46 dataset
load("TI46-IF.mat");
%%
%% Converting to 16bit value
AudioData = struct();
SpikeResponse = struct();
%signalId = 6;
DC_bias = 1.6;
AttenuationFactor = 0.3; 
fileName = './chip4Audio22March/BTBT_response/audioresponse_v2.mat';
for signalId = 1:500
        disp(signalId);
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
        %% 
        try
            write(serialPort, DATAtoSend, 'uint8');
        
            % Display a message indicating successful data transmission
            %disp('Data sent successfully:');
          
        
        catch exception
            % Display any errors
            disp('Error:');
            disp(exception.message);
        end
    
    
       %% DATA reception
       % Configure the serial port (optional, adjust settings as needed)
    %configureTerminator(serialPort, 'LF');
    %serialPort.Timeout = 3; % Timeout in seconds
    
    try
        % Wait for the first byte to be available
        while serialPort.NumBytesAvailable == 0
            pause(0.1); % Small pause to prevent tight loop
        end
    
        % Read the first byte (you can choose to discard or use it)
        length_data =70000;
        Received_data = read(serialPort, length_data, 'uint8');
        % Display the received data
        %disp('Received data:');
        
    catch exception
        % Display any errors
        disp('Error:');
        disp(exception.message);
    end
    
    % Clean up by clearing the serial port object
    %clear serialPort;
    %% 
    SpikeResponse(signalId).Response = Received_data ;
    pause(1);
end
save(fileName,'SpikeResponse');
clear serialPort;