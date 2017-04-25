/* -----------------------------------------------------------------------------

SSD1289 based display driver

Copyright (C) 2015  Stephan Thiele - stephant86@gmail.com

Part of this code is an adaptation from souce code provided by
        Fabio Angeletti - fabio.angeletti89@gmail.com

Part of this code is an adaptation from souce code provided by
        WaveShare - http://www.waveshare.net

Part of this code is an adaptation from souce code provided by
        Michael Margolis - https://code.google.com/p/glcd-arduino/

// TODO: write proper driver
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "SSD1289.h"
#include "delay.h"
#include <stdlib.h>

static void SSD1289_SetCursor(uint16_t x, uint16_t y) {
  #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

  uint16_t temp = x;
  x = y;
  y = (MAX_X - 1) - temp;

  #elif  (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)
  #endif

  SSD1289_WriteReg(SET_GDDRAM_X_ADDRESS_COUNTER, x);
  SSD1289_WriteReg(SET_GDDRAM_Y_ADDRESS_COUNTER, y);
}

void LCD_CtrlLinesConfig(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG |
               RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF,
               ENABLE);
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);  // D2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);  // D3
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);  // NOE -> RD
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);  // NWE -> WR
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,  GPIO_AF_FSMC);  // NE1 -> CS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);  // D13
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);  // D14
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);  // D15
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);  // A16 -> RS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);  // D0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);  // D1

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC);  // D4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC);  // D5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC);  // D6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);  // D7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);  // D8
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);  // D9
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);  // D10
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);  // D11
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);  // D12

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  |
                                GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void LCD_FSMCConfig(void) {
  FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;

  //-----------------------------------------
  // Clock Enable von FSMC
  //-----------------------------------------
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  //-----------------------------------------
  // Structure fuer das Timing
  //-----------------------------------------
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime      = 5;  // 5;
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime       = 5;  // 1;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime         = 10; // 5;
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;  // 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision           = 0;  // 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency           = 0;  // 0'
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode            = FSMC_AccessMode_A;

  //-----------------------------------------
  // Structure fuer Bank-1 / PSRAM-1
  //-----------------------------------------
  FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &FSMC_NORSRAMTimingInitStructure;

  // Config vom FSMC
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  // Enable Bank-1 / PSRAM-1
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}


/* Functions -----------------------------------------------------------------*/
static void SSD1289_Configuration(void) {
  LCD_CtrlLinesConfig();
  LCD_FSMCConfig();
  delayMs(100);
}

uint16_t SSD1289_ReadData(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  uint16_t value;

  Set_Rs;
  Set_nWr;
  Clr_nRd;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(SSD1289_DATA_PORT, &GPIO_InitStructure);

  value = GPIO_ReadInputData(SSD1289_DATA_PORT);
  value = GPIO_ReadInputData(SSD1289_DATA_PORT);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(SSD1289_DATA_PORT, &GPIO_InitStructure);

  Set_nRd;
  return value;
}

// old
#define LCD_REG      (*((volatile unsigned short *) 0x60000000))
#define LCD_RAM      (*((volatile unsigned short *) 0x60020000))
#define LCD_REG_34    0x22
void LCD_WriteRAM_Prepare(void) {
  LCD_REG = LCD_REG_34;
  //delayMs(5);
}
///

void SSD1289_WriteReg(uint16_t SSD1289_Reg, uint16_t SSD1289_RegValue) {
  LCD_REG = SSD1289_Reg;
  LCD_RAM = SSD1289_RegValue;
}

void SSD1289_Init(void) {
  SSD1289_Configuration();

  RegSSD1289_OSCILLATION_START_t                   oscillation_start;
  RegSSD1289_DRIVER_OUTPUT_t                       driver_output;
  RegSSD1289_LCD_DRIVE_AC_t                        lcd_drive_ac;
  RegSSD1289_POWER_CONTROL_1_t                     power_control_1;
  RegSSD1289_COMPARE_1_t                           compare_1;
  RegSSD1289_COMPARE_2_t                           compare_2;
  RegSSD1289_DISPLAY_CONTROL_t                     display_control;
  RegSSD1289_FRAME_CYCLE_CONTROL_t                 frame_cycle_control;
  RegSSD1289_POWER_CONTROL_2_t                     power_control_2;
  RegSSD1289_POWER_CONTROL_3_t                     power_control_3;
  RegSSD1289_POWER_CONTROL_4_t                     power_control_4;
  RegSSD1289_GATE_SCAN_START_POSITION_t            gate_scan_start_position;
  RegSSD1289_SLEEP_MODE_t                          sleep_mode;
  RegSSD1289_ENTRY_MODE_t                          entry_mode;
  RegSSD1289_OPTIMIZE_ACCESS_SPEED_3_t             optimize_access_speed_3;
  RegSSD1289_GENERIC_INTERFACE_CONTROL_t           generic_interface_control;
  RegSSD1289_HORIZONTAL_PORCH_t                    horizontal_porch;
  RegSSD1289_VERTICAL_PORCH_t                      vertical_porch;
  RegSSD1289_POWER_CONTROL_5_t                     power_control_5;
  RegSSD1289_RAM_DATA_READ_WRITE_t                 ram_data_read_write;
  RegSSD1289_RAM_WRITE_DATA_MASK_1_t               ram_write_data_mask_1;
  RegSSD1289_RAM_WRITE_DATA_MASK_2_t               ram_write_data_mask_2;
  RegSSD1289_FRAME_FREQUENCY_t                     frame_frequency;
  RegSSD1289_VCOM_OTP28_t                          vcom_otp28;
  RegSSD1289_OPTIMIZE_ACCESS_SPEED_1_t             optimize_access_speed_1;
  RegSSD1289_VCOM_OTP29_t                          vcom_otp29;
  RegSSD1289_OPTIMIZE_ACCESS_SPEED_2_t             optimize_access_speed_2;
  RegSSD1289_GAMMA_CONTROL_1_t                     gamma_control_1;
  RegSSD1289_GAMMA_CONTROL_2_t                     gamma_control_2;
  RegSSD1289_GAMMA_CONTROL_3_t                     gamma_control_3;
  RegSSD1289_GAMMA_CONTROL_4_t                     gamma_control_4;
  RegSSD1289_GAMMA_CONTROL_5_t                     gamma_control_5;
  RegSSD1289_GAMMA_CONTROL_6_t                     gamma_control_6;
  RegSSD1289_GAMMA_CONTROL_7_t                     gamma_control_7;
  RegSSD1289_GAMMA_CONTROL_8_t                     gamma_control_8;
  RegSSD1289_GAMMA_CONTROL_9_t                     gamma_control_9;
  RegSSD1289_GAMMA_CONTROL_10_t                    gamma_control_10;
  RegSSD1289_VERTICAL_SCROLL_CONTROL_1_t           vertical_scroll_control_1;
  RegSSD1289_VERTICAL_SCROLL_CONTROL_2_t           vertical_scroll_control_2;
  RegSSD1289_HORIZONTAL_RAM_ADDRESS_POSITION_t     horizontal_ram_address_position;
  RegSSD1289_VERTICAL_RAM_ADDRESS_START_POSITION_t vertical_ram_address_start_position;
  RegSSD1289_VERTICAL_RAM_ADDRESS_END_POSITION_t   vertical_ram_address_end_position;
  RegSSD1289_FIRST_WINDOW_START_t                  first_window_start;
  RegSSD1289_FIRST_WINDOW_END_t                    first_window_end;
  RegSSD1289_SECOND_WINDOW_START_t                 second_window_start;
  RegSSD1289_SECOND_WINDOW_END_t                   second_window_end;
  RegSSD1289_SET_GDDRAM_X_ADDRESS_COUNTER_t        set_gddram_x_address_counter;
  RegSSD1289_SET_GDDRAM_Y_ADDRESS_COUNTER_t        set_gddram_y_address_counter;

  oscillation_start.OSCEN = 1;
  power_control_1.AP = 7; //2;
  power_control_1.DC = 10;
  power_control_1.BT = 4;
  power_control_1.DCT = 10;
  power_control_2.VRC0 = 0;
  power_control_3.VRH = 12;
  power_control_4.VDV = 11;
  power_control_4.VCOMG = 1;
  power_control_5.VCM = 48;
  power_control_5.nOTP = 1;
  driver_output.MUX = 319;
  driver_output.TB = 1; // 1
  driver_output.SM = 0;
  driver_output.BGR = 1;
  driver_output.CAD = 0;
  driver_output.REV = 1;
  driver_output.RL = 0; // ;0;
  lcd_drive_ac.NW = 0;
  lcd_drive_ac.WSMD = 0;
  lcd_drive_ac.EOR = 1;
  lcd_drive_ac.BC = 1;
  lcd_drive_ac.ENWS = 0;
  lcd_drive_ac.FLD = 0;
  sleep_mode.SLP = 0;
  entry_mode.LG = 0;
  entry_mode.AM = 0; // 0
  entry_mode.ID = 3;
  entry_mode.TY = 1;
  entry_mode.DMode = 0;
  entry_mode.WMode = 0;
  entry_mode.OEDef = 0;
  entry_mode.TRANS = 0;
  entry_mode.DFM = 3;
  entry_mode.VSmode = 0;
  compare_1.CPG = 0;
  compare_1.CPR = 0;
  compare_2.CPB = 0;
  horizontal_porch.HBP = 28;
  horizontal_porch.XL = 239;
  vertical_porch.VBP = 3;
  vertical_porch.VFP = 0;
  display_control.D = 3;
  display_control.CM = 0;
  display_control.DTE = 1;
  display_control.GON = 1;
  display_control.SPT = 1;
  display_control.VLE = 0;
  display_control.PT = 0;
  frame_cycle_control.DIV = 0;
  frame_cycle_control.EQ = 0;
  frame_cycle_control.NO = 0;
  frame_cycle_control.RTN = 0;
  frame_cycle_control.SDIV = 0;
  frame_cycle_control.SDT = 0;
  frame_cycle_control.SRTN = 0;
  gate_scan_start_position.SCN = 0;
  vertical_scroll_control_1.VL10_18 = 0;
  vertical_scroll_control_2.VL20_28 = 0;
  first_window_start.SS10_18 = 0;
  first_window_end.SE10_18 = 319;
  second_window_start.SS20_28 = 0;
  second_window_end.SE20_28 = 0;
  horizontal_ram_address_position.HSA = 0;
  horizontal_ram_address_position.HEA = 239;
  vertical_ram_address_start_position.VSA = 0;
  vertical_ram_address_end_position.VEA = 319;
  gamma_control_1.PKP0_2 = 7;
  gamma_control_1.PKP10_12 = 7;
  gamma_control_2.PKP20_22 = 4;
  gamma_control_2.PKP30_32 = 2;
  gamma_control_3.PKP40_42 = 4;
  gamma_control_3.PKP50_52 = 2;
  gamma_control_4.PRP0_2 = 2;
  gamma_control_4.PRP10_12 = 5;
  gamma_control_5.PKN0_2 = 7;
  gamma_control_5.PKN10_12 = 5;
  gamma_control_6.PKN20_22 = 4;
  gamma_control_6.PKN30_32 = 2;
  gamma_control_7.PKN40_42 = 4;
  gamma_control_7.PKN50_52 = 2;
  gamma_control_8.PRN0_2 = 2;
  gamma_control_8.PRN10_12 = 5;
  gamma_control_9.VRP0_3 = 2;
  gamma_control_9.VRP10_14 = 3;
  gamma_control_10.VRN0_3 = 2;
  gamma_control_10.VRN10_14 = 3;
  ram_write_data_mask_1.WMG = 0;
  ram_write_data_mask_1.WMR = 0;
  ram_write_data_mask_2.WMB = 0;
  frame_frequency.OSC = 8;
  set_gddram_x_address_counter.XAD = 0;
  set_gddram_y_address_counter.YAD = 0;
  optimize_access_speed_1.setThisVariableTo_0x06 = 0x10; // 0x06 0r 0x10?
  optimize_access_speed_2.setThisVariableTo_0x12BE = 0x12BE;
  optimize_access_speed_3.setThisVariableTo_0x16CEB = 0x16CEB; // not a 16 bit value? (will be truncated)

  SSD1289_WriteReg(OSCILLATION_START,                   *(uint16_t*)&oscillation_start); // Enable SSD1289 Oscillator
  SSD1289_WriteReg(POWER_CONTROL_1,                     *(uint16_t*)&power_control_1);
  SSD1289_WriteReg(POWER_CONTROL_2,                     *(uint16_t*)&power_control_2);
  SSD1289_WriteReg(POWER_CONTROL_3,                     *(uint16_t*)&power_control_3);
  SSD1289_WriteReg(POWER_CONTROL_4,                     *(uint16_t*)&power_control_4);
  SSD1289_WriteReg(POWER_CONTROL_5,                     *(uint16_t*)&power_control_5);
  SSD1289_WriteReg(DRIVER_OUTPUT,                       *(uint16_t*)&driver_output); // 320*240 0x2B3F
  SSD1289_WriteReg(LCD_DRIVE_AC,                        *(uint16_t*)&lcd_drive_ac);
  SSD1289_WriteReg(SLEEP_MODE,                          *(uint16_t*)&sleep_mode);
  SSD1289_WriteReg(ENTRY_MODE,                          *(uint16_t*)&entry_mode);
  SSD1289_WriteReg(COMPARE_1,                           *(uint16_t*)&compare_1);
  SSD1289_WriteReg(COMPARE_2,                           *(uint16_t*)&compare_2);
  SSD1289_WriteReg(HORIZONTAL_PORCH,                    *(uint16_t*)&horizontal_porch);
  SSD1289_WriteReg(VERTICAL_PORCH,                      *(uint16_t*)&vertical_porch);
  SSD1289_WriteReg(DISPLAY_CONTROL,                     *(uint16_t*)&display_control);
  SSD1289_WriteReg(FRAME_CYCLE_CONTROL,                 *(uint16_t*)&frame_cycle_control);
  SSD1289_WriteReg(GATE_SCAN_START_POSITION,            *(uint16_t*)&gate_scan_start_position);
  SSD1289_WriteReg(VERTICAL_SCROLL_CONTROL_1,           *(uint16_t*)&vertical_scroll_control_1);
  SSD1289_WriteReg(VERTICAL_SCROLL_CONTROL_2,           *(uint16_t*)&vertical_scroll_control_2);
  SSD1289_WriteReg(FIRST_WINDOW_START,                  *(uint16_t*)&first_window_start);
  SSD1289_WriteReg(FIRST_WINDOW_END,                    *(uint16_t*)&first_window_end);
  SSD1289_WriteReg(SECOND_WINDOW_START,                 *(uint16_t*)&second_window_start);
  SSD1289_WriteReg(SECOND_WINDOW_END,                   *(uint16_t*)&second_window_end);
  SSD1289_WriteReg(HORIZONTAL_RAM_ADDRESS_POSITION,     *(uint16_t*)&horizontal_ram_address_position);
  SSD1289_WriteReg(VERTICAL_RAM_ADDRESS_START_POSITION, *(uint16_t*)&vertical_ram_address_start_position);
  SSD1289_WriteReg(VERTICAL_RAM_ADDRESS_END_POSITION,   *(uint16_t*)&vertical_ram_address_end_position);
  SSD1289_WriteReg(GAMMA_CONTROL_1,                     *(uint16_t*)&gamma_control_1);
  SSD1289_WriteReg(GAMMA_CONTROL_2,                     *(uint16_t*)&gamma_control_2);
  SSD1289_WriteReg(GAMMA_CONTROL_3,                     *(uint16_t*)&gamma_control_3);
  SSD1289_WriteReg(GAMMA_CONTROL_4,                     *(uint16_t*)&gamma_control_4);
  SSD1289_WriteReg(GAMMA_CONTROL_5,                     *(uint16_t*)&gamma_control_5);
  SSD1289_WriteReg(GAMMA_CONTROL_6,                     *(uint16_t*)&gamma_control_6);
  SSD1289_WriteReg(GAMMA_CONTROL_7,                     *(uint16_t*)&gamma_control_7);
  SSD1289_WriteReg(GAMMA_CONTROL_8,                     *(uint16_t*)&gamma_control_8);
  SSD1289_WriteReg(GAMMA_CONTROL_9,                     *(uint16_t*)&gamma_control_9);
  SSD1289_WriteReg(GAMMA_CONTROL_10,                    *(uint16_t*)&gamma_control_10);
  SSD1289_WriteReg(RAM_WRITE_DATA_MASK_1,               *(uint16_t*)&ram_write_data_mask_1);
  SSD1289_WriteReg(RAM_WRITE_DATA_MASK_2,               *(uint16_t*)&ram_write_data_mask_2);
  SSD1289_WriteReg(FRAME_FREQUENCY,                     *(uint16_t*)&frame_frequency);
  SSD1289_WriteReg(SET_GDDRAM_X_ADDRESS_COUNTER,        *(uint16_t*)&set_gddram_x_address_counter);
  SSD1289_WriteReg(SET_GDDRAM_Y_ADDRESS_COUNTER,        *(uint16_t*)&set_gddram_y_address_counter);

  // Does not have any effect!?
  SSD1289_WriteReg(OPTIMIZE_ACCESS_SPEED_1,             *(uint16_t*)&optimize_access_speed_1); // or 0xA???
  SSD1289_WriteReg(OPTIMIZE_ACCESS_SPEED_2,             *(uint16_t*)&optimize_access_speed_2);
  SSD1289_WriteReg(OPTIMIZE_ACCESS_SPEED_3,             *(uint16_t*)&optimize_access_speed_3);

  SSD1289_Clear(Black);
}

void SSD1289_Clear(uint16_t Color) {
  uint32_t index = 0;
  SSD1289_SetCursor(0,0);
  LCD_WriteRAM_Prepare();

  for(index = 0; index < MAX_X * MAX_Y; index++)
    LCD_RAM = Color; // Use DMA-Controller here?
}

void SSD1289_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t point) {
  if( Xpos >= MAX_X || Ypos >= MAX_Y )
    return;

  SSD1289_SetCursor(Xpos, Ypos);
  SSD1289_WriteReg(RAM_DATA_READ_WRITE, point);
}

void SSD1289_Backlight(uint32_t val) {
  if(val > 0)
    Set_Backlight;
  else
    Clr_Backlight;
}

// The following functions are generic drawing functions and therefore not part of the driver.
// TODO: remove?
void SSD1289_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 , uint16_t color) {
    int delta_x = x2 - x1;
    // if x1 == x2, then it does not matter what we set here
    signed char const ix = (delta_x > 0) - (delta_x < 0);
    delta_x = abs(delta_x) << 1;

    int delta_y = y2 - y1;
    // if y1 == y2, then it does not matter what we set here
    signed char const iy = (delta_y > 0) - (delta_y < 0);
    delta_y = abs(delta_y) << 1;

    SSD1289_SetPoint(x1, y1, color);

    if (delta_x >= delta_y) {
      // error may go below zero
      int error = delta_y - (delta_x >> 1);

      while (x1 != x2){
        if ((error >= 0) && (error || (ix > 0))) {
            error -= delta_x;
            y1 += iy;
        }
        // else do nothing

        error += delta_y;
        x1 += ix;

        SSD1289_SetPoint(x1, y1, color);
      }
    }
    else {
      // error may go below zero
      int error = delta_x - (delta_y >> 1);

      while (y1 != y2) {
        if ((error >= 0) && (error || (iy > 0))) {
            error -= delta_y;
            x1 += ix;
        }
        // else do nothing

        error += delta_x;
        y1 += iy;

        SSD1289_SetPoint(x1, y1, color);
      }
  }
}

void SSD1289_DrawPicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pic)
{
	uint16_t x_index, y_index;
	uint32_t pic_index = 0;

    //for(y_index = y; y_index < (y+h); y_index++)
    //if wrong orientation
    for(y_index = (y+h); y_index > 0 ; y_index--)
    {
        for(x_index = x; x_index < (x+w); x_index++)
        {
			SSD1289_SetPoint(x_index, y_index, pic[pic_index]);
			pic_index++;
		}
	}
}

void SSD1289_DrawPicture8bit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *pic)
{
	uint16_t color;
	uint16_t x_index, y_index;
	uint32_t pic_index = 0;

    //for(y_index = y; y_index < (y+h); y_index++)
    //if wrong orientation
	for(y_index = (y+h); y_index > 0 ; y_index--)
	{
		for(x_index = x; x_index < (x+w); x_index++)
		{
			color = (pic[pic_index+1] << 8) | (pic[pic_index]);
			SSD1289_SetPoint(x_index, y_index, color);
			pic_index += 2;
		}
	}
}

void SSD1289_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
	int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    SSD1289_SetPoint(x0, y0 + r, color);
    SSD1289_SetPoint(x0, y0 - r, color);
    SSD1289_SetPoint(x0 + r, y0, color);
    SSD1289_SetPoint(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1289_SetPoint(x0 + x, y0 + y, color);
        SSD1289_SetPoint(x0 - x, y0 + y, color);
        SSD1289_SetPoint(x0 + x, y0 - y, color);
        SSD1289_SetPoint(x0 - x, y0 - y, color);

        SSD1289_SetPoint(x0 + y, y0 + x, color);
        SSD1289_SetPoint(x0 - y, y0 + x, color);
        SSD1289_SetPoint(x0 + y, y0 - x, color);
        SSD1289_SetPoint(x0 - y, y0 - x, color);
    }
}

void SSD1289_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
	int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    int16_t i;

    for (i=y0-r; i<=y0+r; i++)
        SSD1289_SetPoint(x0, i, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        for (i=y0-y; i<=y0+y; i++) {
            SSD1289_SetPoint(x0+x, i, color);
            SSD1289_SetPoint(x0-x, i, color);
        }
        for (i=y0-x; i<=y0+x; i++) {
            SSD1289_SetPoint(x0+y, i, color);
            SSD1289_SetPoint(x0-y, i, color);
        }
    }
}

void SSD1289_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x+w) > MAX_X)
		w = MAX_X - x;

	if ((y+h) > MAX_Y)
		h = MAX_Y - y;

	SSD1289_DrawLine(x, y, x, y + h, color);
	SSD1289_DrawLine(x, y, x+w, y, color);
	SSD1289_DrawLine(x+w, y+h, x, y+h, color);
	SSD1289_DrawLine(x+w, y+h, x+w, y, color);
}

void SSD1289_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint16_t x_index, y_index;

	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x+w) > MAX_X)
		w = MAX_X - x;

	if ((y+h) > MAX_Y)
		h = MAX_Y - y;

	for(x_index = x; x_index < x+w; x_index++)
	{
		for(y_index = y; y_index < y+h; y_index++)
		{
			SSD1289_SetPoint(x_index, y_index, color);
		}
	}

}

void SSD1289_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if (x0 > MAX_X)
		x0 = MAX_X;
	if (y0 > MAX_Y)
		y0 = MAX_Y;
	if (x1 > MAX_X)
		x1 = MAX_X;
	if (y1 > MAX_Y)
		y1 = MAX_Y;
	if (x2 > MAX_X)
		x2 = MAX_X;
	if (y2 > MAX_Y)
		y2 = MAX_Y;

	SSD1289_DrawLine(x0, y0, x1, y1, color);
	SSD1289_DrawLine(x1, y1, x2, y2, color);
	SSD1289_DrawLine(x2, y2, x0, y0, color);
}

void SSD1289_DrawCross(uint16_t Xpos, uint16_t Ypos, uint16_t in_color, uint16_t out_color)
{
  SSD1289_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,in_color);
  SSD1289_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,in_color);
  SSD1289_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,in_color);
  SSD1289_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,in_color);

  SSD1289_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,out_color);
  SSD1289_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,out_color);

  SSD1289_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,out_color);
  SSD1289_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,out_color);

  SSD1289_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,out_color);
  SSD1289_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,out_color);

  SSD1289_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,out_color);
  SSD1289_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,out_color);
}


