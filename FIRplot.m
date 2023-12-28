close all
clc 

input = load("inputData.txt");
output = load("outputData.txt");
subplot(2,1,1);
plot(input);
title("滤波前");
subplot(2,1,2);
plot(output);
title("滤波后");