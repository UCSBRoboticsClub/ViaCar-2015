#ifndef GLOBALS_H
#define GLOBALS_H


const unsigned int controlFreq = 200; // Hz
const unsigned int controlPeriodUs = 1000000 / controlFreq;
const float dt = 1.f / controlFreq;


#endif // GLOBALS_H
