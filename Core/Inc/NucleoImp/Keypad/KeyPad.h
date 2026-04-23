#ifndef KEYPAD_H
#define KEYPAD_H


#include "main.h"


namespace ELE3312{

enum class KeyCode : int{
		// First row
		ONE = 0x1,
		TWO = 0x2,
		THREE = 0x4,
		A = 0x8,
		// Second row
		FOUR = 0x10,
		FIVE = 0x20,
		SIX = 0x40,
		B = 0x80,
		// Third row
		SEVEN = 0x100,
		EIGHT = 0x200,
		NINE = 0x400,
		C = 0x800,
		// Fourth row
		STAR = 0x1000,
		ZERO = 0x2000,
		OCTOTHORPE = 0x4000,
		D = 0x8000,
		// Special value
		UNKNOWN = 0xFFFF
	};

class KeyPad {
	public :
		KeyPad();
		~KeyPad() =  default;
		void setup(GPIO_TypeDef * gpio);
		KeyCode update();
		KeyCode keyPress();
	private :
		GPIO_TypeDef * gpio = nullptr;
		uint8_t keypressed = 0;
		KeyCode key;
		const uint32_t NumRow = 4;
		const uint32_t NumColum = 4;

		KeyCode keyboard[4][4] = {
				{ELE3312::KeyCode::ONE, ELE3312::KeyCode::TWO, ELE3312::KeyCode::THREE, ELE3312::KeyCode::A}, // Row 0
				{ELE3312::KeyCode::FOUR, ELE3312::KeyCode::FIVE, ELE3312::KeyCode::SIX, ELE3312::KeyCode::B}, // Row 1
				{ELE3312::KeyCode::SEVEN, ELE3312::KeyCode::EIGHT, ELE3312::KeyCode::NINE, ELE3312::KeyCode::C}, // Row 2
				{ELE3312::KeyCode::STAR, ELE3312::KeyCode::ZERO, ELE3312::KeyCode::OCTOTHORPE, ELE3312::KeyCode::D} // Row 3
		};
	};

}

#endif
