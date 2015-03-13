function [ V, P ] = lm22670Calc( R1, R2 )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
V=R1*(5+5E-4*R2)/R2+5;
P=(V/(R1+R2))^2*(R1+R2);

end

