#ifndef GLOBALS_H
#define GLOBALS_H

#include "./Servo.h"
#include "VNH5019.h"
#include "Button.h"
#include "LowPass.h"


const int sensorRPin = A0;
const int sensorLPin = A1;
const int switchPin = 0;
const int motorFwdPin = 7;
const int motorBackPin = 6;
const int motorSpeedPin = 5;
const int servoPin = 3;

extern float vr;
extern float vl;
extern float xr;
extern float xl;
extern LowPass x;
extern float h;
extern float d;
extern float c1;
extern float c2;
extern float speed;

extern Motor motor;
extern Servo servo;
extern Button calSwitch;
extern PIDController servoController;

const unsigned int controlFreq = 1000; // Hz
const unsigned int controlPeriodUs = 1000000 / controlFreq;
const float dt = 1.f / controlFreq;


#endif // GLOBALS_H
