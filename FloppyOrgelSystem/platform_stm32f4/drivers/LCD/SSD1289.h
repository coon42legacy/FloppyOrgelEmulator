/* -----------------------------------------------------------------------------

SSD1289 based display driver

Copyright (C) 2015  Stephan Thiele - stephant86@gmail.com

Part of this code is an adaptation from souce code provided by
        Fabio Angeletti - fabio.angeletti89@gmail.com

Part of this code is an adaptation from souce code provided by
        WaveShare - http://www.waveshare.net

Part of this code is an adaptation from souce code provided by
        Michael Margolis - https://code.google.com/p/glcd-arduino/

------------------------------------------------------------------------------*/

#ifndef __SSD1289_H 
#define __SSD1289_H

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "stm32f4xx.h"
#include "AsciiLib.h"

// SSD1289 registers
enum {
  OSCILLATION_START                   = 0x00,
  DRIVER_OUTPUT                       = 0x01,
  LCD_DRIVE_AC                        = 0x02, // LCD-Driving-Waveform Control
  POWER_CONTROL_1                     = 0x03,
  COMPARE_1                           = 0x05,
  COMPARE_2                           = 0x06,
  DISPLAY_CONTROL                     = 0x07,
  FRAME_CYCLE_CONTROL                 = 0x0B,
  POWER_CONTROL_2                     = 0x0C,
  POWER_CONTROL_3                     = 0x0D,
  POWER_CONTROL_4                     = 0x0E,
  GATE_SCAN_START_POSITION            = 0x0F,
  SLEEP_MODE                          = 0x10,
  ENTRY_MODE                          = 0x11,
  OPTIMIZE_ACCESS_SPEED_3             = 0x12,
  GENERIC_INTERFACE_CONTROL           = 0x15,
  HORIZONTAL_PORCH                    = 0x16,
  VERTICAL_PORCH                      = 0x17,
  POWER_CONTROL_5                     = 0x1E,
  RAM_DATA_READ_WRITE                 = 0x22, // Write mode on R/W = 1, D/C = 1, Read mode on R/W = 0, D/C = 1
  RAM_WRITE_DATA_MASK_1               = 0x23,
  RAM_WRITE_DATA_MASK_2               = 0x24,
  FRAME_FREQUENCY                     = 0x25,
  VCOM_OTP28                          = 0x28, // VCOM_OTP is defined twice in datasheet. 28 Suffix means 'register 28'
  OPTIMIZE_ACCESS_SPEED_1             = 0x28,
  VCOM_OTP29                          = 0x29, // VCOM_OTP is defined twice in datasheet. 29 Suffix means 'register 29'
  OPTIMIZE_ACCESS_SPEED_2             = 0x2F,
  GAMMA_CONTROL_1                     = 0x30,
  GAMMA_CONTROL_2                     = 0x31,
  GAMMA_CONTROL_3                     = 0x32,
  GAMMA_CONTROL_4                     = 0x33,
  GAMMA_CONTROL_5                     = 0x34,
  GAMMA_CONTROL_6                     = 0x35,
  GAMMA_CONTROL_7                     = 0x36,
  GAMMA_CONTROL_8                     = 0x37,
  GAMMA_CONTROL_9                     = 0x3A,
  GAMMA_CONTROL_10                    = 0x3B,
  VERTICAL_SCROLL_CONTROL_1           = 0x41,
  VERTICAL_SCROLL_CONTROL_2           = 0x42,
  HORIZONTAL_RAM_ADDRESS_POSITION     = 0x44,
  VERTICAL_RAM_ADDRESS_START_POSITION = 0x45,
  VERTICAL_RAM_ADDRESS_END_POSITION   = 0x46,
  FIRST_WINDOW_START                  = 0x48,
  FIRST_WINDOW_END                    = 0x49,
  SECOND_WINDOW_START                 = 0x4A,
  SECOND_WINDOW_END                   = 0x4B,
  SET_GDDRAM_X_ADDRESS_COUNTER        = 0x4E,
  SET_GDDRAM_Y_ADDRESS_COUNTER        = 0x4F
};

typedef struct {
  uint16_t
  OSCEN : 1;
} RegSSD1289_OSCILLATION_START_t;

typedef struct {
  uint16_t
  MUX : 9,
  TB  : 1,
  SM  : 1,
  BGR : 1,
  CAD : 1,
  REV : 1,
  RL  : 1;
} RegSSD1289_DRIVER_OUTPUT_t;

typedef struct {
  uint16_t
  NW   : 8,
  WSMD : 1,
  EOR  : 1,
  BC   : 1,
  ENWS : 1,
  FLD  : 1;
} RegSSD1289_LCD_DRIVE_AC_t;

typedef struct {
  uint16_t
      : 1,
  AP  : 3,
  DC  : 4,
      : 1,
  BT  : 3,
  DCT : 4;
} RegSSD1289_POWER_CONTROL_1_t;

typedef struct {
  uint16_t
      : 2,
  CPG : 6,
      : 2,
  CPR : 6;
} RegSSD1289_COMPARE_1_t;

typedef struct {
  uint16_t
      : 2,
  CPB : 6;
} RegSSD1289_COMPARE_2_t;

typedef struct {
  uint16_t
  D   : 2,
      : 1,
  CM  : 1,
  DTE : 1,
  GON : 1,
      : 2,
  SPT : 1,
  VLE : 2,
  PT  : 2;
} RegSSD1289_DISPLAY_CONTROL_t;

typedef struct {
  uint16_t
  RTN  : 4,
  SRTN : 1,
  SDIV : 1,
  DIV  : 2,
  EQ   : 3,
       : 1,
  SDT  : 2,
  NO   : 2;
} RegSSD1289_FRAME_CYCLE_CONTROL_t;

typedef struct {
  uint16_t
  VRC0 : 3;
} RegSSD1289_POWER_CONTROL_2_t;

typedef struct {
  uint16_t
  VRH : 4;
} RegSSD1289_POWER_CONTROL_3_t;

typedef struct {
  uint16_t
        : 8,
  VDV   : 5,
  VCOMG : 1;
} RegSSD1289_POWER_CONTROL_4_t;

typedef struct {
  uint16_t
  SCN : 9;
} RegSSD1289_GATE_SCAN_START_POSITION_t;

typedef struct {
  uint16_t
  SLP : 1;
} RegSSD1289_SLEEP_MODE_t;

typedef struct {
  uint16_t
  LG     : 3,
  AM     : 1,
  ID     : 2,
  TY     : 2,
  DMode  : 2,
  WMode  : 1,
  OEDef  : 1,
  TRANS  : 1,
  DFM    : 2,
  VSmode : 1;
} RegSSD1289_ENTRY_MODE_t;

typedef struct {
  uint16_t
  setThisVariableTo_0x16CEB;
} RegSSD1289_OPTIMIZE_ACCESS_SPEED_3_t;

typedef struct {
  uint16_t
  INVVS : 1,
  INNVHS : 1,
  INVDEN : 1,
  INVDOT : 1;
} RegSSD1289_GENERIC_INTERFACE_CONTROL_t;

typedef struct {
  uint16_t
  HBP : 8,
  XL  : 8;
} RegSSD1289_HORIZONTAL_PORCH_t;

typedef struct {
  uint16_t
  VBP : 8,
  VFP : 8;
} RegSSD1289_VERTICAL_PORCH_t;

typedef struct {
  uint16_t
  VCM  : 6,
       : 1,
  nOTP : 1;
} RegSSD1289_POWER_CONTROL_5_t;

typedef struct {
  uint16_t
  Data : 16;
} RegSSD1289_RAM_DATA_READ_WRITE_t;

typedef struct {
  uint16_t
      : 2,
  WMG : 6,
      : 2,
  WMR : 6;
} RegSSD1289_RAM_WRITE_DATA_MASK_1_t;

typedef struct {
  uint16_t
      : 2,
  WMB : 6;
} RegSSD1289_RAM_WRITE_DATA_MASK_2_t;

typedef struct {
  uint16_t
      : 12,
  OSC : 4;
} RegSSD1289_FRAME_FREQUENCY_t;

typedef struct {
  uint16_t
  setThisVariableTo_0x6; // or 10 ????
} RegSSD1289_VCOM_OTP28_t;

typedef struct {
  uint16_t
  setThisVariableTo_0x06;
} RegSSD1289_OPTIMIZE_ACCESS_SPEED_1_t;

typedef struct {
  uint16_t
  setThisVariableTo_0x80C0;
} RegSSD1289_VCOM_OTP29_t;

typedef struct {
  uint16_t
  setThisVariableTo_0x12BE;
} RegSSD1289_OPTIMIZE_ACCESS_SPEED_2_t;

typedef struct {
  uint16_t
  PKP0_2   : 3,
           : 5,
  PKP10_12 : 3;
} RegSSD1289_GAMMA_CONTROL_1_t;

typedef struct {
  uint16_t
  PKP20_22 : 3,
           : 5,
  PKP30_32 : 3;
} RegSSD1289_GAMMA_CONTROL_2_t;

typedef struct {
  uint16_t
  PKP40_42 : 3,
           : 5,
  PKP50_52 : 3;
} RegSSD1289_GAMMA_CONTROL_3_t;

typedef struct {
  uint16_t
  PRP0_2   : 3,
           : 5,
  PRP10_12 : 3;
} RegSSD1289_GAMMA_CONTROL_4_t;

typedef struct {
  uint16_t
  PKN0_2   : 3,
           : 5,
  PKN10_12 : 3;
} RegSSD1289_GAMMA_CONTROL_5_t;

typedef struct {
  uint16_t
  PKN20_22 : 3,
           : 5,
  PKN30_32 : 3;
} RegSSD1289_GAMMA_CONTROL_6_t;

typedef struct {
  uint16_t
  PKN40_42 : 3,
           : 5,
  PKN50_52 : 3;
} RegSSD1289_GAMMA_CONTROL_7_t;

typedef struct {
  uint16_t
  PRN0_2   : 3,
           : 5,
  PRN10_12 : 3;
} RegSSD1289_GAMMA_CONTROL_8_t;

typedef struct {
  uint16_t
  VRP0_3   : 4,
           : 4,
  VRP10_14 : 4;
} RegSSD1289_GAMMA_CONTROL_9_t;

typedef struct {
  uint16_t
  VRN0_3   : 4,
           : 4,
  VRN10_14 : 4;
} RegSSD1289_GAMMA_CONTROL_10_t;

typedef struct {
  uint16_t
  VL10_18;
} RegSSD1289_VERTICAL_SCROLL_CONTROL_1_t;

typedef struct {
  uint16_t
  VL20_28;
} RegSSD1289_VERTICAL_SCROLL_CONTROL_2_t;

typedef struct {
  uint16_t
  HSA : 8,
  HEA : 8;
} RegSSD1289_HORIZONTAL_RAM_ADDRESS_POSITION_t;

typedef struct {
  uint16_t
  VSA : 9;
} RegSSD1289_VERTICAL_RAM_ADDRESS_START_POSITION_t;

typedef struct {
  uint16_t
  VEA : 9;
} RegSSD1289_VERTICAL_RAM_ADDRESS_END_POSITION_t;

typedef struct {
  uint16_t
  SS10_18 : 9;
} RegSSD1289_FIRST_WINDOW_START_t;

typedef struct {
  uint16_t
  SE10_18 : 9;
} RegSSD1289_FIRST_WINDOW_END_t;

typedef struct {
  uint16_t
  SS20_28 : 9;
} RegSSD1289_SECOND_WINDOW_START_t;

typedef struct {
  uint16_t
  SE20_28 : 9;
} RegSSD1289_SECOND_WINDOW_END_t;

typedef struct {
  uint16_t
  XAD : 8;
} RegSSD1289_SET_GDDRAM_X_ADDRESS_COUNTER_t;

typedef struct {
  uint16_t
  YAD : 9;
} RegSSD1289_SET_GDDRAM_Y_ADDRESS_COUNTER_t;
// end of coon extensions



/* Private define ------------------------------------------------------------*/

//#define DISP_HOR_RESOLUTION				320
//#define DISP_VER_RESOLUTION				240

//#define DISP_ORIENTATION					0
#define DISP_ORIENTATION					90
//#define DISP_ORIENTATION					180
//#define DISP_ORIENTATION					270

/* Private define ------------------------------------------------------------*/

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/* PORTs and PINs definitions ------------------------------------------------*/
#define SSD1289_CTRL_PORT		GPIOD
#define SSD1289_DATA_PORT		GPIOE
#define SSD1289_BACKLIGHT_PORT		GPIOA

#define SSD1289_CS_PIN			GPIO_Pin_14
#define SSD1289_RS_PIN			GPIO_Pin_15
#define SSD1289_nWR_PIN			GPIO_Pin_12
#define SSD1289_nRD_PIN			GPIO_Pin_13
#define SSD1289_RESET_PIN		GPIO_Pin_11
#define SSD1289_BACKLIGHT_PIN		GPIO_Pin_0

#define Set_Cs        		GPIO_SetBits(SSD1289_CTRL_PORT, SSD1289_CS_PIN)
#define Clr_Cs        		GPIO_ResetBits(SSD1289_CTRL_PORT, SSD1289_CS_PIN)

#define Set_Rs        		GPIO_SetBits(SSD1289_CTRL_PORT, SSD1289_RS_PIN)
#define Clr_Rs        		GPIO_ResetBits(SSD1289_CTRL_PORT, SSD1289_RS_PIN)

#define Set_nWr       		GPIO_SetBits(SSD1289_CTRL_PORT, SSD1289_nWR_PIN)
#define Clr_nWr       		GPIO_ResetBits(SSD1289_CTRL_PORT, SSD1289_nWR_PIN)

#define Set_nRd       		GPIO_SetBits(SSD1289_CTRL_PORT, SSD1289_nRD_PIN)
#define Clr_nRd       		GPIO_ResetBits(SSD1289_CTRL_PORT, SSD1289_nRD_PIN)

#define Set_Reset       	GPIO_SetBits(SSD1289_CTRL_PORT, SSD1289_RESET_PIN)
#define Clr_Reset       	GPIO_ResetBits(SSD1289_CTRL_PORT, SSD1289_RESET_PIN)

#define Set_Backlight       	GPIO_SetBits(SSD1289_BACKLIGHT_PORT, SSD1289_BACKLIGHT_PIN)
#define Clr_Backlight       	GPIO_ResetBits(SSD1289_BACKLIGHT_PORT, SSD1289_BACKLIGHT_PIN)

/* 16bit RGB565 colors */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

// macro to convert RGB color in 565RGB format
#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* Function prototypes -------------------------------------------------------*/
void SSD1289_WriteReg(uint16_t SSD1289_Reg, uint16_t SSD1289_RegValue);
void SSD1289_Init(void); // controller initialization
void SSD1289_Clear(uint16_t Color); // fills screen with chosen color

uint16_t SSD1289_GetPoint(uint16_t Xpos, uint16_t Ypos); // return the color of requested point
void SSD1289_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t point); // places a point with selected color
void SSD1289_Backlight(uint32_t val); // backlight ON/OFF
void SSD1289_DrawPicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pic); // draws a picture saved as 16bit array
void SSD1289_DrawPicture8bit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *pic); // draws a picture saved as 2x8bit array
void SSD1289_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ); // draws a line starting at x0,y0 and ending at x1,y1 with selected color
void SSD1289_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color); // draws a circle with center x0,y0 radius r and selected color
void SSD1289_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color); // draws a filled circle with center x0,y0 radius r and selected color
void SSD1289_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color); // draws a rectangle starting at x,y with width w and height h with selected color
void SSD1289_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color); // draws a filled rectangle starting at x,y with width w and height h with selected color
void SSD1289_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color); // draws a triangle with coordinates x0,y0 x1,y1 x2,y2 with selected color
void SSD1289_DrawCross(uint16_t Xpos, uint16_t Ypos, uint16_t in_color, uint16_t out_color); // draws a crosshair at Xpos,Ypos with 2 selected colors

#endif 
