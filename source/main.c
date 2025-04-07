/*************************************************************************
 *	Challenge_1_Firmware:   main.c
 * 	                        This is only to demostrate how to use the PCL6046
 *                          interface.  There is no expectation that it will
 *                          run as written because the project is only partially
 *                          complete.  This hasn't been compiled in STM32Cube,
 *                          so it has only been checked for syntax errors by
 *                          my reading over it.
 *
 *  Engineer:               Larry Pelton
 *
 ************************************************************************/
#define		MAIN_C

#include	<stdint.h>
#include	<stdbool.h>

#include	"PCL6046.h"
#include    "PCL6046_maint.h"
#include    "PCL6046_comm.h"



void main(void)
{
    //  TODO:   hardware initialization, including setting-up parallel interface from STM32
    //          to PCL6046, setting-up USB connection, etc.
    //  hw_init();

    //  create the ASIC quantities and its periodic maintenance task
    if (xTaskCreate(ASIC_maintenance, "maint6046", configMINIMAL_STACK_SIZE, (void *) NULL, (BASE_TASK_PRI + 1), (TaskHandle_t *) NULL) == pdPASS)
    {
        //  create the task for USB-to-ASIC communication
        if (xTaskCreate(ASIC_comm, "comm6046", configMINIMAL_STACK_SIZE, (void *) NULL, BASE_TASK_PRI, (TaskHandle_t *) NULL) == pdPASS)
        {

            //  start-up the RTOS scheduler
            vTaskStartScheduler();
        }
    }
}



