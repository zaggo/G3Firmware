#include <avr/interrupt.h>
#include "ZProbe.hh"
#include "Configuration.hh"
#include "EepromMap.hh"
#include "ExtruderBoard.hh"

/*
  ZProbe.cc
  Author: Eberhard Rensch
*/

#define LOW_PULSE_uS ((micros_t)388L)
#define HIGH_PULSE_DIFF_uS ((micros_t)1752L)

#define ZPROBE_TIMEOUTINTERVAL ((micros_t)750000L)

// Default Angles in Degree
#define DEFAULT_Z_PROBE_ENGAGE_ANGLE  26 
#define DEFAULT_Z_PROBE_DISENGAGE_ANGLE 5

ZProbe::ZProbe():
timeout_interval_micros(0),
pulse_interval_micros(LOW_PULSE_uS),
newPulseInterval(LOW_PULSE_uS),
periode_start_micros(0),
baseFreq_interval_micros(20000), // 20ms -> 50Hz
offline(true)
{
	init();
}

uint8_t ZProbe::init()
{
	Z_PROBE_PIN.setValue(false);	  // LOW
 	Z_PROBE_PIN.setDirection(true); // OUTPUT

	disengage_angle = eeprom::getEeprom8(eeprom::Z_PROBE_DISENGAGE_ANGLE,DEFAULT_Z_PROBE_DISENGAGE_ANGLE);
	engage_angle = eeprom::getEeprom8(eeprom::Z_PROBE_ENGAGE_ANGLE,DEFAULT_Z_PROBE_ENGAGE_ANGLE);
 
 	engage(false);
  
 	return 1;
}

void ZProbe::doInterrupt(micros_t currentMicros)
{
	//if(!offline)
	{
		//if(currentMicros<timeout_interval_micros)
		{
			micros_t micros_diff = currentMicros-periode_start_micros;
			if(micros_diff>=baseFreq_interval_micros)
			{
				Z_PROBE_PIN.setValue(true);	  // HIGH
				periode_start_micros = currentMicros;
				if( newPulseInterval!= pulse_interval_micros)
					pulse_interval_micros = newPulseInterval;
			}
			else if(micros_diff>=pulse_interval_micros)
			{
				Z_PROBE_PIN.setValue(false);	  // LOW
			}
		}
//		else {
//			Z_PROBE_PIN.setValue(false);	  // LOW
//			offline=true;
//		}
	}

}

void ZProbe::engage(bool engage)
{
	angleAdjust(engage?engage_angle:disengage_angle);
}

void ZProbe::angleAdjust(uint8_t angle)
{
	newPulseInterval = (LOW_PULSE_uS + (HIGH_PULSE_DIFF_uS*(micros_t)angle)/(micros_t)180L);
	timeout_interval_micros = ExtruderBoard::getBoard().getCurrentMicros()+ZPROBE_TIMEOUTINTERVAL;
	offline=false;
}

