#include <IntervalTimer.h>
#include <SPI.h>
#include "Globals.h"
#include "RadioTerminal.h"
#include "Commands.h"
#include "ADC.h"
#include <cmath>


IntervalTimer controlTimer;
ADC adc;

void controlLoop();
float getPosition();
float volt2dist(float v);


void setup()
{
    asm(".global _printf_float");

    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);
    pinMode(led4Pin, OUTPUT);
    
    x.setCutoffFreq(dt, 10.f);
    minScore.setTimeConst(dt, 0.1f);
    thetalp.setTimeConst(dt, 1.f);

    adc.setResolution(12, ADC_0);
    adc.setConversionSpeed(ADC_HIGH_SPEED, ADC_0);
    adc.setSamplingSpeed(ADC_HIGH_SPEED, ADC_0);
    adc.setAveraging(16, ADC_0);
    adc.setResolution(12, ADC_1);
    adc.setConversionSpeed(ADC_HIGH_SPEED, ADC_1);
    adc.setSamplingSpeed(ADC_HIGH_SPEED, ADC_1);
    adc.setAveraging(16, ADC_1);
    adc.startSynchronizedContinuous(sensorRPin, sensorLPin);

    RadioTerminal::initialize();
    setupCommands();

    servoController.setOutputLimits(-30.f, 30.f);
    servoController.setTuning(100.f, 0.f, 10.f);
    servo.calibrate(1188, 1788, 35.f, -35.f);

    controlTimer.begin(controlLoop, controlPeriodUs);
    controlTimer.priority(144);
}

float buzFreq = 110.f;

void loop()
{
    delay(10);

    button.update();
    switch1.update();
    switch2.update();
    
    if (switch1.pressEdge())
        c2 = 0.f;

    const float pi = 3.14159f;
    float rate = (switch1.pressed() ? -1.f : 1.f) * 2.f*pi/1000.f;
    const float pwr = 4.f;
    analogWrite(led1Pin, int(65535.f * std::pow((std::sin(millis()*rate) + 1.f) * 0.5f, pwr)));
    analogWrite(led2Pin, int(65535.f * std::pow((std::sin(millis()*rate - pi*0.5f) + 1.f) * 0.5f, pwr)));
    analogWrite(led3Pin, int(65535.f * std::pow((std::sin(millis()*rate - pi) + 1.f) * 0.5f, pwr)));
    analogWrite(led4Pin, int(65535.f * std::pow((std::sin(millis()*rate - pi*1.5f) + 1.f) * 0.5f, pwr)));

    if (switch2.pressEdge())
    {
        buzFreq = 110.f;
        tone(buzzerPin, buzFreq);
    }

    if (button.releaseEdge() && switch2.pressed())
    {
        buzFreq *= 1.0594631f;
        tone(buzzerPin, buzFreq);
    }

    if (!switch2.pressed())
        noTone(buzzerPin);
        
}


void controlLoop()
{   
    x.push(getPosition());
    controllerOut = servoController.update(x);

    float vdot = 0.f;
    vel = (speed - 0.1f) * 11.f;

    curvature = (controllerOut - vdot*std::sin(thetaest)) /
        (vel*vel*std::cos(thetaest));

    float degrees = curvature / 0.0681f;

    if (controllerEnabled)
        servo = degrees;

    motor = speed;

    const float thetathresh = 1.f; 
    if (std::fabs(theta) < thetathresh || curvature*theta >= 0.f)
        theta += -curvature * dt;
    thetalp.push(theta);
    thetaest = theta - thetalp;
}


float getPosition()
{
    auto adcVals = adc.readSynchronizedContinuous();
    vr = adcVals.result_adc0 / 4096.f * 3.3f;
    vl = adcVals.result_adc1 / 4096.f * 3.3f;

    xr = volt2dist(vr);
    xl = volt2dist(vl);

    float deff = d * std::cos(thetaest);

    if (xr > xmax + deff*0.5f || xl > xmax + deff*0.5f)
        return xmax * (x > 0.f ? 1.f : -1.f);

    // Possible locations of the wire based on the two readings
    float candidates[] =
    {
        ( xr + xl) * 0.5f,
        (-xr + xl) * 0.5f,
        (-xr - xl) * 0.5f
    };

    // Difference between right and left distance measurements
    float rlDiff[] =
    {
         xr - xl + deff,
        -xr - xl + deff,
        -xr + xl + deff
    };

    // Score assigned to each candidate wire location (lower is better)
    float score[3];
    for (int i = 0; i < 3; ++i)
        score[i] = rlDiff[i]*rlDiff[i] + (x-candidates[i])*(x-candidates[i]); 

    // Choose candidate with the lowest score
    int imin = 0;
    for (int i = 1; i < 3; ++i)
        imin = score[i] < score[imin] ? i : imin;

    minScore.push(score[imin]);
    if (minScore > scoreLimit)
        return xmax * (x > 0.f ? 1.f : -1.f);

    return candidates[imin];
}


float volt2dist(float v)
{
    float a = h*std::exp(v/c1);

    if (switch1.pressed())
        c2 = a > c2 ? a : c2;

    float b = c2*std::cos(thetaest)/a - 1.f;
    return h*std::sqrt(b > 0.f ? b : 0.f);
}

