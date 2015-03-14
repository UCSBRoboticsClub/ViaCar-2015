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
    
    controlTimer.begin(controlLoop, controlPeriodUs);
    controlTimer.priority(144);

    RadioTerminal::initialize();
    setupCommands();
}


void loop()
{
    delay(10);
}


void controlLoop()
{
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
        (+xr + +xl) * 0.5f,
        (-xr + +xl) * 0.5f,
        (-xr + -xl) * 0.5f
    };

    float rlDiff[] =
    {
        std::fabs( xr - xl + d),
        std::fabs(-xr - xl + d),
        std::fabs(-xr + xl + d)
    };

    float prevDiff[] =
    {
        std::fabs(x - candidates[0]),
        std::fabs(x - candidates[1]),
        std::fabs(x - candidates[2])
    };

    if (rlDiff[0] + prevDiff[0] < rlDiff[1] + prevDiff[1])
    {
        if (rlDiff[0] + prevDiff[0] < rlDiff[2] + prevDiff[2])
            return candidates[0];
        else
            return candidates[2];
    }
    else 
    {
        if (rlDiff[1] + prevDiff[1] < rlDiff[2] + prevDiff[2])
            return candidates[1];
        else
            return candidates[2];
    }
        
}


float volt2dist(float v)
{
    float x2 = c2*h/std::exp(v/c1) - h*h;
    return std::sqrt(x2 > 0.f ? x2 : 0.f);
}

