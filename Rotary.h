/*
 * Rotary encoder library for Arduino.
 */
#pragma once 
#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"
// Enable this to emit codes twice per step.
// #define HALF_STEP

enum class RotaryOutput : uint8_t
{
	// Values returned by 'process'
	// No complete step yet.
	NONE = 0x0,
	// Clockwise step.
	CW = 0x10,
	// Counter-clockwise step.
	CCW = 0x20,
	// Button
    BTN_DOWN = 0x40,
	BTN_UP = 0x80
};


enum class RotaryStateHalfStep : uint8_t
{
	START = 0x0,
	CCW_BEGIN = 0x1,
	CW_BEGIN = 0x2,
	START_CC = START | static_cast<uint8_t>(RotaryOutput::CW),
	START_CCW = START | static_cast<uint8_t>(RotaryOutput::CCW),
	
	START_M = 0x3,
	CW_BEGIN_M = 0x4,
	CCW_BEGIN_M = 0x5,
	START_M_CC = START_M | static_cast<uint8_t>(RotaryOutput::CW),
	START_M_CCW = START_M | static_cast<uint8_t>(RotaryOutput::CCW)
};
	

enum class RotaryState : uint8_t
{
	START = 0x0,
	CW_FINAL = 0x1,
	CW_BEGIN = 0x2,
	CW_NEXT = 0x3,
	CCW_BEGIN = 0x4,
	CCW_FINAL = 0x5,
	CCW_NEXT = 0x6,
	START_CW = START | static_cast<uint8_t>(RotaryOutput::CW),
	START_CCW = START | static_cast<uint8_t>(RotaryOutput::CCW)
};

class Rotary
{
  public:
    Rotary();
    RotaryOutput process(bool a, bool b, bool btn);
  private:
    RotaryState state;
	bool btnState;
};

#endif
 
