clc; clearvars;
clear serialPort;
port = 'COM3';
baudRate = 230400;
% Create the serialport object
serialPort = serialport(port, baudRate);
% Configure the serial port (optional, adjust settings as needed)
configureTerminator(serialPort, 'LF');
serialPort.Timeout = 1; % Timeout in seconds
%% DAC 0 data
% if value is x the DAC will give x + 3mv due to offset 
Vref = 1.8; GND = 0; 
D0CH1 = 0.45;            %VDD_0p4 
D0CH2 = 0.85;            %VSSH = 0.9
D0CH3 = D0CH2 + 0.6;     %VDDL = VSSH + 0.6
D0CH4 = 1.8;             %VDD1p8 = 1.8
D0CH5 = 0;               %VDD_R = 0
D0CH6 = 0.3;             %VB
D0CH7 = 0;               %VS
D0CH8 = 0.6;             %VDD_0p6

[DAC0CH1_MSB,DAC0CH1_LSB]=float2Uint10_2byte(D0CH1,GND,Vref);
[DAC0CH2_MSB,DAC0CH2_LSB]=float2Uint10_2byte(D0CH2,GND,Vref);
[DAC0CH3_MSB,DAC0CH3_LSB]=float2Uint10_2byte(D0CH3,GND,Vref);
[DAC0CH4_MSB,DAC0CH4_LSB]=float2Uint10_2byte(D0CH4,GND,Vref);
[DAC0CH5_MSB,DAC0CH5_LSB]=float2Uint10_2byte(D0CH5,GND,Vref);
[DAC0CH6_MSB,DAC0CH6_LSB]=float2Uint10_2byte(D0CH6,GND,Vref);
[DAC0CH7_MSB,DAC0CH7_LSB]=float2Uint10_2byte(D0CH7,GND,Vref);
[DAC0CH8_MSB,DAC0CH8_LSB]=float2Uint10_2byte(D0CH8,GND,Vref);


%% DAC 0 data
D1CH1 = 0;           %DAC_0p3V_AAK // DC bias for filter
D1CH2 = 0;           %I400u with buffer 1.5v
D1CH4 = 0;           %I10n PCB lacation 10n in front of VO2
D1CH3 = 0.316;           %I10n_splitter PCB lacation 10n in front of VO3 //chip 1 = 0.316V
D1CH5 = 0; 
D1CH6 = 0; 
D1CH7 = 0.0;         %I5u with buffer 1V
D1CH8 = 1;           %VDD1p0
[DAC1CH1_MSB,DAC1CH1_LSB]=float2Uint10_2byte(D1CH1,GND,Vref);
[DAC1CH2_MSB,DAC1CH2_LSB]=float2Uint10_2byte(D1CH2,GND,Vref);
[DAC1CH3_MSB,DAC1CH3_LSB]=float2Uint10_2byte(D1CH3,GND,Vref);
[DAC1CH4_MSB,DAC1CH4_LSB]=float2Uint10_2byte(D1CH4,GND,Vref);
[DAC1CH5_MSB,DAC1CH5_LSB]=float2Uint10_2byte(D1CH5,GND,Vref);
[DAC1CH6_MSB,DAC1CH6_LSB]=float2Uint10_2byte(D1CH6,GND,Vref);
[DAC1CH7_MSB,DAC1CH7_LSB]=float2Uint10_2byte(D1CH7,GND,Vref);
[DAC1CH8_MSB,DAC1CH8_LSB]=float2Uint10_2byte(D1CH8,GND,Vref);

DATAtoSend = [DAC0CH1_MSB,DAC0CH1_LSB, DAC0CH2_MSB,DAC0CH2_LSB, DAC0CH3_MSB,DAC0CH3_LSB, DAC0CH4_MSB,DAC0CH4_LSB,...
              DAC0CH5_MSB,DAC0CH5_LSB, DAC0CH6_MSB,DAC0CH6_LSB, DAC0CH7_MSB,DAC0CH7_LSB, DAC0CH8_MSB,DAC0CH8_LSB,...
              DAC1CH1_MSB,DAC1CH1_LSB, DAC1CH2_MSB,DAC1CH2_LSB, DAC1CH3_MSB,DAC1CH3_LSB, DAC1CH4_MSB,DAC1CH4_LSB,...
              DAC1CH5_MSB,DAC1CH5_LSB, DAC1CH6_MSB,DAC1CH6_LSB, DAC1CH7_MSB,DAC1CH7_LSB, DAC1CH8_MSB,DAC1CH8_LSB];
%%
NoOfBytesToSend = numel(DATAtoSend);

%% 
try
    % Write the 18 bytes of data to the serial port
    %write(serialPort, uint8(NoOfBytesToSend_LSB), 'uint8');
    %write(serialPort, uint8(NoOfBytesToSend_MSB), 'uint8');
    write(serialPort, DATAtoSend, 'uint8');

    % Display a message indicating successful data transmission
    disp('DAC loaded');
  

catch exception
    % Display any errors
    disp('Error:');
    disp(exception.message);
end

% Clean up by clearing the serial port object
clear serialPort;