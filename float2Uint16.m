function [MSB,LSB] = float2Uint16(floatNum, minVal, maxVal)
    nBits = 16;
    maxIntVal = 2^nBits - 1;
    % Clip the floatNum to be within minVal and maxVal
    floatNum = max(min(floatNum, maxVal), minVal);
    % Scale and quantize the float number
    scaledInt = round((floatNum - minVal) / (maxVal - minVal) * maxIntVal);
    % Convert the integer to a binary string
    binStr = dec2bin(scaledInt, nBits);
    %disp(['12-bit binary representation: ', binaryStr]);
    %data16bit = append('0000',binStr);
    data16bit = binStr;
    MSB = uint8(bin2dec(data16bit(1:8)));
    LSB = uint8(bin2dec(data16bit(9:16)));

end