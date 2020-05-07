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
	// No action
	NONE = 0x0,
	// Clockwise step.
	CW = 0x1,
	// Counter-clockwise step.
	CCW = 0x2,
	// Button pressed
    BTN_DOWN = 0x4,
	// Button released
	BTN_SHORT = 0x8,
	// Button pressed more than 500ms
	BTN_LONG = 0x10
};

enum class RotaryStateHalfStep : uint8_t
{
	START = 0x0,
	CCW_BEGIN = 0x1,
	CW_BEGIN = 0x2,
	START_CW = 0x10,
	START_CCW = 0x20,
	
	START_M = 0x3,
	CW_BEGIN_M = 0x4,
	CCW_BEGIN_M = 0x5,
	START_M_CW = 0x13,
	START_M_CCW = 0x23
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
	START_CW = 0x10,
	START_CCW = 0x20
};

enum class ButtonState : uint8_t
{
	BTN_PRESSED,
	BTN_RELEASED,
	BTN_IGNORE
};

class Rotary
{
  public:
    Rotary();
    RotaryOutput process(bool a, bool b, bool btn);
  private:
    RotaryState state;
	ButtonState btnState;
	uint32_t btn_down_millis;
};

#endif
 
