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
/* Exported functions ------------------------------------------------------- */
void set_clk(void);
/* prototype */

#endif /* __MAIN_H */
