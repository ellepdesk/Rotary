/* Rotary encoder handler for arduino.
 *
 * Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
 * Contact: bb@cactii.net
 *
 */

#include "Rotary.h"

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */

//#define START 0x0

#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)

const unsigned char ttable[6][4] = {
  // START (00)
  {START_M,     CW_BEGIN,     CCW_BEGIN,  START},
  // CCW_BEGIN
  {START_M_CCW, START,        CCW_BEGIN,  START},
  // CW_BEGIN
  {START_M_CW,  CW_BEGIN,     START,      START},
  // START_M (11)
  {START_M,     CCW_BEGIN_M,  CW_BEGIN_M, START},
  // CW_BEGIN_M
  {START_M,     START_M,      CW_BEGIN_M, START_CW},
  // CCW_BEGIN_M
  {START_M,     CCW_BEGIN_M,  START_M,    START_CCW},
};
#else
// Use the full-step state table (emits a code at 00 only)


const RotaryState ttable[7][4] = {
  // START
  {RotaryState::START,    RotaryState::CW_BEGIN,  RotaryState::CCW_BEGIN, RotaryState::START},
  // RotaryState::CW_FINAL
  {RotaryState::CW_NEXT,  RotaryState::START,     RotaryState::CW_FINAL,  RotaryState::START_CW},
  // RotaryState::CW_BEGIN
  {RotaryState::CW_NEXT,  RotaryState::CW_BEGIN,  RotaryState::START,     RotaryState::START},
  // RotaryState::CW_NEXT
  {RotaryState::CW_NEXT,  RotaryState::CW_BEGIN,  RotaryState::CW_FINAL,  RotaryState::START},
  // RotaryState::CCW_BEGIN
  {RotaryState::CCW_NEXT, RotaryState::START,     RotaryState::CCW_BEGIN, RotaryState::START},
  // RotaryState::CCW_FINAL
  {RotaryState::CCW_NEXT, RotaryState::CCW_FINAL, RotaryState::START,     RotaryState::START_CCW},
  // RotaryState::CCW_NEXT
  {RotaryState::CCW_NEXT, RotaryState::CCW_FINAL, RotaryState::CCW_BEGIN, RotaryState::START},
};
#endif

Rotary::Rotary() {
  state = RotaryState::START;
  btnState = ButtonState::BTN_RELEASED;
}

RotaryOutput Rotary::process(bool a, bool b, bool btn) {
	RotaryOutput result = RotaryOutput::NONE;
	switch(btnState)
	{
		case ButtonState::BTN_RELEASED: 
		{
			if (btn)
			{
				btnState = ButtonState::BTN_PRESSED;
				btn_down_millis = millis();
				result = RotaryOutput::BTN_DOWN;
			}
		}
		break;
		
		case ButtonState::BTN_PRESSED:
		{
			uint32_t duration = millis() - btn_down_millis;
			if (duration > 500)
			{
				btnState = ButtonState::BTN_IGNORE;
				result = RotaryOutput::BTN_LONG;
			}
			if (!btn) 
			{
				btnState = ButtonState::BTN_RELEASED;
				result = RotaryOutput::BTN_SHORT;
			}
		}
		break;
		
		case ButtonState::BTN_IGNORE:
		{
			if (!btn)
				btnState = ButtonState::BTN_RELEASED;
		}
		break;
	}
	
	if (result != RotaryOutput::NONE)
	{
		return result;
	}
	
	// Grab state of input pins.
	uint8_t pinstate = (b << 1) | a;
	// Determine new state from the pins and state table.
	state = ttable[static_cast<int>(state) & 0xf][pinstate];
	if (state == RotaryState::START_CW)
		result = RotaryOutput::CW;
	if (state == RotaryState::START_CCW)
		result = RotaryOutput::CCW;
	return result;
}
