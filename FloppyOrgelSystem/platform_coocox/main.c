#include <stdio.h>
#include "stm32f4xx.h"
#include "LCD/SSD1289.h"
#include "NesGamePad/NesGamePad.h"
#include "floppyorgel_system.h"

// Ring buffer
#define RING_BUFFER_SIZE 32

typedef struct {
  char ringBuffer[RING_BUFFER_SIZE];
  int wptr;
  int rptr;
} LockFreeFIFO_t;


// static LockFreeFIFO_t fifoUART1;
// static LockFreeFIFO_t fifoUART6;

int getRingBufferDistance(LockFreeFIFO_t* lff) {
  return lff->rptr > lff->wptr ? lff->rptr - lff->wptr : lff->rptr - lff->wptr + RING_BUFFER_SIZE;
}

void writeToRingBuffer(LockFreeFIFO_t* lff, char b) {
  if(getRingBufferDistance(lff) == 1) {
    printf("Ring buffer overflow!\n\r");
    return;
  }

  lff->ringBuffer[lff->wptr] = b;
  lff->wptr = (lff->wptr + 1) % RING_BUFFER_SIZE;
}

char readFromRingBuffer(LockFreeFIFO_t* lff) {
  if(getRingBufferDistance(lff) == RING_BUFFER_SIZE) {
    printf("Ring buffer underflow!\n\r");
    return 0;
  }

  char ret = lff->ringBuffer[lff->rptr];
  lff->rptr = (lff->rptr + 1) % RING_BUFFER_SIZE;
  return ret;
}

void enableDelayTimer() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef timerInitStructure;
  timerInitStructure.TIM_Prescaler = 84 - 1; // 1 Mhz
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = -1; // no period, use maximum of uint32_t
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &timerInitStructure);
  TIM_Cmd(TIM2, ENABLE);
}

void enableMidiTimer() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  TIM_TimeBaseInitTypeDef timerInitStructure;
  timerInitStructure.TIM_Prescaler = 84 - 1; // 1 Mhz
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = -1; // no period, use maximum of uint32_t
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM5, &timerInitStructure);
  TIM_Cmd(TIM5, ENABLE);
}

// UART
void initializeDebugUart(uint32_t baudrate) {
	/* This is a concept that has to do with the libraries provided by ST
	 * to make development easier the have made up something similar to
	 * classes, called TypeDefs, which actually just define the common
	 * parameters that every peripheral needs to work correctly
	 *
	 * They make our life easier because we don't have to mess around with
	 * the low level stuff of setting bits in the correct registers
	 */
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
	USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization

	/* enable APB2 peripheral clock for USART1
	 * note that only USART1 and USART6 are connected to APB2
	 * the other USARTs are connected to APB1
	 */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* enable the peripheral clock for the pins used by
	 * USART1, PB6 for TX and PB7 for RX
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* This sequence sets up the TX and RX pins
	 * so they work correctly with the USART1 peripheral
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) are used
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	/* The RX and TX pins are now connected to their AF
	 * so that the USART1 can take over control of the
	 * pins
	 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* Now the USART_InitStruct is used to define the
	 * properties of USART1
	 */
	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting


	/* Here the USART1 receive interrupt is enabled
	 * and the interrupt controller is configured
	 * to jump to the USART1_IRQHandler() function
	 * if the USART1 receive interrupt occurs
	 */

	/*
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff
	*/

	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
}

void initializeBusUart(uint32_t baudrate) {
  GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
  USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization

  /* enable APB2 peripheral clock for USART1
   * note that only USART1 and USART6 are connected to APB2
   * the other USARTs are connected to APB1
   */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  /* enable the peripheral clock for the pins used by
   * USART6, PC6 for TX and PC7 for RX
   */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* This sequence sets up the TX and RX pins
   * so they work correctly with the USART1 peripheral
   */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) are used
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;       // the pins are configured as alternate function so the USART peripheral has access to them
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;   // this defines the IO speed and has nothing to do with the baudrate!
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     // this defines the output type as push pull mode (as opposed to open drain)
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;     // this activates the pullup resistors on the IO pins
  GPIO_Init(GPIOC, &GPIO_InitStruct);         // now all the values are passed to the GPIO_Init() function which sets the GPIO registers

  /* The RX and TX pins are now connected to their AF
   * so that the USART6 can take over control of the
   * pins
   */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  /* Now the USAR6_InitStruct is used to define the
   * properties of USART1
   */
  USART_InitStruct.USART_BaudRate = baudrate;       // the baudrate is set to the value we passed into this init function
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
  USART_InitStruct.USART_StopBits = USART_StopBits_1;   // we want 1 stop bit (standard)
  USART_InitStruct.USART_Parity = USART_Parity_No;    // we don't want a parity bit (standard)
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
  USART_Init(USART6, &USART_InitStruct);          // again all the properties are passed to the USART_Init function which takes care of all the bit setting

  // finally this enables the complete USART6 peripheral
  USART_Cmd(USART6, ENABLE);
}

int main(void) {
	enableDelayTimer();
   // enableMidiTimer();
    //initializeDebugUart(9600);
    //initializeBusUart(9600);
    setupNesGamePad();
    SSD1289_Init();
    SSD1289_Clear(Black);
	system_main();
}







