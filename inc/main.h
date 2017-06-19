/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/

#ifdef USE_HAL_DRIVER
#include "stm32f4xx_hal.h"
#endif

#include "stm32f4xx.h"
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define HUMIDITY_OUT_L 0x28
#define HUMIDITY_OUT_H 0x29
#define TEMP_OUT_L 0x2A
#define TEMP_OUT_H 0x2B

#define T0_OUT_L 0x3C
#define T0_OUT_R 0x3D
#define T1_OUT_L 0x3E
#define T1_OUT_R 0x3F

#define T0_degC_x8 0x32
#define T1_degC_x8 0x33

/* Exported functions ------------------------------------------------------- */
void set_clk(void);
/* prototype */

#endif /* __MAIN_H */
