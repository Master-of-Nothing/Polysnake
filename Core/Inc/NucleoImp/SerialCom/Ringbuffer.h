/**
  * @file Ringbuffer.h 
  * @date June 2025
  * @brief Declares the Ringbuffer class that is used to store incoming bytes from 
  * the communication interface.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef RINGBUFFER_INCLUDE_GUARD
#define RINGBUFFER_INCLUDE_GUARD

#define RING_BUFFER_SIZE 512

#include <cstdint>
#include <cstdlib>

namespace ELE3312 {

/** @brief The Ringbuffer class implements a circular buffer that is used to 
  * store incoming bytes from the communication interface.
  */
class Ringbuffer {
public:
	Ringbuffer(); 
	void setup();
	~Ringbuffer() ;
	bool write(uint8_t data);
	bool read(uint8_t *data);


private:
	uint8_t buffer[RING_BUFFER_SIZE];
	volatile uint16_t head; // Volatile car modifié dans l'interruption
	volatile uint16_t tail;
	volatile uint16_t size;
};

} // End namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
