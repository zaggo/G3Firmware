#ifndef ZProbe_h
#define ZProbe_h

/*
  ZProbe.hh
  Author: Eberhard Rensch
*/

#include "Types.hh"

class ZProbe
{
  private:
    uint8_t disengage_angle;
    uint8_t engage_angle;
    
	volatile micros_t timeout_interval_micros;
	volatile micros_t pulse_interval_micros;
	volatile micros_t newPulseInterval; // Double buffered pulse_interval
	volatile micros_t periode_start_micros;
	volatile micros_t baseFreq_interval_micros;
	volatile bool offline;

	uint8_t init();

  public:
    ZProbe();
	
	void doInterrupt(micros_t currentMicros);

	void engage(bool engage);
	void angleAdjust(uint8_t angle);
};

#endif
