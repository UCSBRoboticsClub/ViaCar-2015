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
    
    x.setCutoffFreq(50.f, dt);

    adc.setResolution(12);
    adc.setConversionSpeed(ADC_HIGH_SPEED);
    adc.setSamplingSpeed(ADC_HIGH_SPEED);
    adc.setAveraging(16);

    RadioTerminal::initialize();
    setupCommands();

    servoController.setOutputLimits(-60.f, 60.f);
    servoController.setTuning(1000.f, 0.f, 100.f);

    controlTimer.begin(controlLoop, controlPeriodUs);
    controlTimer.priority(144);
}


void loop()
{
    delay(10);
}


void controlLoop()
{
    float lastx = x;
    x.push(getPosition());

    
}


float getPosition()
{
    vr = adc.analogRead(sensorRPin) / 4096.f * 3.3f;
    vl = adc.analogRead(sensorLPin) / 4096.f * 3.3f;

    xr = volt2dist(vr);
    xl = volt2dist(vl);

    float candidates[] =
    {
        ( xr + xl) * 0.5f,
        (-xr + xl) * 0.5f,
        (-xr - xl) * 0.5f
    };

    float rlDiff[] =
    {
         xr - xl + d,
        -xr - xl + d,
        -xr + xl + d
    };

    float score[3];
    for (int i = 0; i < 3; ++i)
        score[i] = rlDiff[i]*rlDiff[i] + (x-candidates[i])*(x-candidates[i]); 

    int imin = 0;
    for (int i = 1; i < 3; ++i)
        imin = score[i] < score[imin] ? i : imin;

    return candidates[imin];
}


float volt2dist(float v)
{
    float xsq = c2*h/std::exp(v/c1) - h*h;
    return std::sqrt(xsq > 0.f ? xsq : 0.f);
}

