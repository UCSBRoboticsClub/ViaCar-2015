#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "./Servo.h"
#include "VNH5019.h"
#include "Button.h"
#include "LowPass.h"


const int servoPin = 3;
const int motorSpeedPin = 5;
const int motorBackPin = 6;
const int motorFwdPin = 7;
const int sensorRPin = A1;
const int sensorLPin = A2;
const int buzzerPin = 14;
const int switch2Pin = 17;
const int switch1Pin = 18;
const int buttonPin = 19;
const int led1Pin = 20;
const int led2Pin = 21;
const int led3Pin = 22;
const int led4Pin = 23;

extern LowPass x;
extern float xmax;
extern float xdot;
extern float speed;
extern float vel;
extern float velfch;
extern float velfcl;
extern float theta;
extern float thetaint;
extern LowPass thetalp;
extern float thetamax;
extern float curvature;
extern float kp;
extern float kd;
extern bool controllerEnabled;
extern float vr;
extern float vl;
extern float xr;
extern float xl;
extern float h;
extern float d;
extern float c1;
extern float c2;
extern LowPass minScore;
extern float scoreLimit;

extern Motor motor;
extern Servo servo;
extern Button button;
extern Button switch1;
extern Button switch2;

const unsigned int controlFreq = 1000; // Hz
const unsigned int controlPeriodUs = 1000000 / controlFreq;
const float dt = 1.f / controlFreq;


#endif // GLOBALS_H
