/*! @file
  @brief
  Hardware abstraction layer
        for PSoC5LP

  <pre>
  Copyright (C) 2016 Kyushu Institute of Technology.
  Copyright (C) 2016 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/
#define MRBC_NO_TIMER

#ifndef MRBC_SRC_HAL_H_
#define MRBC_SRC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
#include "../mcc_generated_files/mcc.h"
#include "../delay.h"


/***** Local headers ********************************************************/
/***** Constant values ******************************************************/
/***** Macros ***************************************************************/

#ifndef MRBC_NO_TIMER
# define hal_init()        ((void)0)
#define hal_enable_irq()  __builtin_disi(0x0000)
#define hal_disable_irq() __builtin_disi(0x3FFF)
#define hal_idle_cpu()    Idle()

#else // MRBC_NO_TIMER
# define hal_init()        ((void)0)
# define hal_enable_irq()  ((void)0)
# define hal_disable_irq() ((void)0)
# define hal_idle_cpu()    ((delay(1)), mrbc_tick())

#endif



/***** Typedefs *************************************************************/
/***** Global variables *****************************************************/
/***** Function prototypes **************************************************/
int hal_write(int fd, const void *buf, int nbytes);
int hal_flush(int fd);


/***** Inline functions *****************************************************/


#ifdef __cplusplus
}
#endif
#endif // ifndef MRBC_HAL_H_
