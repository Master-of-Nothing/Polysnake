/**
  * @file UART.h
  * @date June 2025
  * @brief Implements the UART Communication interface that is used to exchange 
  * messages via the serial interface of the micro controller.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef UART_INCLUDE_GUARD
#define UART_INCLUDE_GUARD 

#include "NucleoImp/SerialCom/Ringbuffer.h"
#include "main.h" 
#include <queue>

#define TEMP_BUFFER_SIZE 256 


namespace ELE3312 {
	/** @brief Implements the Communication class interface for the serial (USART) interface of the 
	  * micro controller. It is used to exchange messages between two players (each one with it's own 
	  * NUCLEO development board).
	  */
	class UART {
	public :
		UART();
		void Process_Data();
	private :
			Ringbuffer buffer;// Private methods

	};

} // End of namespace ELE3312

/**
  * @}
  */
#endif
