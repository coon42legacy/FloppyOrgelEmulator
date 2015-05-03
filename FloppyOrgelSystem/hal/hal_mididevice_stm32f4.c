#include <stdint.h>
#include "stm32f4xx.h"
#include "hal_mididevice.h"

static LockFreeFIFO_t* pFifoDebugPort = 0;

static uint16_t midiNote2Ticks[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    64282,
    60675,
    57269,
    54055,
    51021,
    48157,
    45455,
    42903,
    40495,
    38223,
    36077,
    34052,
    32141,
    30337,
    28635,
    27027,
    25511,
    24079,
    22727,
    21452,
    20248,
    19111,
    18039,
    17026,
    16071,
    15169,
    14317,
    13514,
    12755,
    12039,
    11364,
    10726,
    10124,
    9556,
    9019,
    8513,
    8035,
    7584,
    7159,
    6757,
    6378,
    6020,
    5682,
    5363,
    5062,
    4778,
    4510,
    4257,
    4018,
    3792,
    3579,
    3378,
    3189,
    3010,
    2841,
    2681,
    2531,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

void hal_midiDeviceInit() {
}

void hal_midiDeviceFree() {
}

static void _sendBusData(unsigned char* data, int dataSize) {
  for(int i = 0; i < dataSize; i++) {
    while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET); // Wait until transmit finishes
      USART_SendData(USART6, data[i]);
  }
}

void hal_midiDeviceNoteOn(int32_t channel, int32_t note, int32_t velocity) {
  uint16_t ticks = velocity ? midiNote2Ticks[note] : 0;
  unsigned char data[5] = {0x55, 0xAA, channel, ((char*)&ticks)[1], ((char*)&ticks)[0]};
  _sendBusData(data, sizeof(data));
}

void hal_midiDeviceNoteOff(int32_t channel, int32_t note) {
  hal_midiDeviceNoteOn(channel, note, 0);
}

uint32_t hal_midiDeviceMessage(int32_t iStatus, int32_t iChannel, int32_t iData1, int32_t iData2) {
  // Not used on stm32f4
  return 0;
}

void hal_rs485init(LockFreeFIFO_t* pFifo) {
  pFifoDebugPort = pFifo;
}

void hal_rs485Send(char dataByte) {
  while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
  USART_SendData(USART6, dataByte);
}

void USART1_IRQHandler() {
  // check if the USART1 receive interrupt flag was set
  if(USART_GetITStatus(USART1, USART_IT_RXNE))
    writeToRingBuffer(pFifoDebugPort, USART1->DR); // The Interrupt gets released just by reading USART1->DR
}
