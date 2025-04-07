/*************************************************************************
 *  Challenge_1_Firmware:   PCL6046_maint.c
 *                          Handles periodic register access of PCL6046,
 *                          as specified in challenge description.  It should
 *                          be populated with polling for ASIC interrupts, etc.
 *                          Currently, it's just reading the main status registers,
 *                          which can then be read from external functions
 *                          with the getter routine provided.
 *
 *  Engineer:               Larry Pelton
 *
  ************************************************************************/
#define		PCL6046_MAINT_C

#include	<stdint.h>
#include	<stdbool.h>

#include	"PCL6046.h"
#include    "PCL6046_maint.h"

/*************************************************************************
 *  @brief:     get_axial_status
 *              Get method for status of PCL6046 on a selected axis
 *  @param[in]  axis identifies X, Y, Z, or U axis
 *  @returns    the 16-bit status register value
 ************************************************************************/
uint16_t get_axial_status(MOTION_AXIS axis)
{
    return (PCL6046_mstatus[axis]);
}

/*************************************************************************
 *  @brief      ASIC_maintenance
 *              This RTOS task handles periodic maintenance of the PCL6046 interface.
 *              The period of execution is set via ASIC_MAINT_PERIOD and
 *              is measured in milliseconds.
 *  @param[in]  pvParameters is ignored, currently
 *  @returns    none
 ************************************************************************/
void ASIC_maintenance(void *pvParameters)
{
    if (init_PCL6046_resources() == true)
    {
        TickType_t lastTimeHere = xTaskGetTickCount();

        while (1)
        {
            //  reserve the ASIC comm interface if possible; if not, don't block;
            //  try again later instead
            if (xSemaphoreTake(PCL6046_mutex, 0) ==  pdTRUE)
            {
                //  read the main status registers
                PCL6046_mstatus[AXIS_X] = X_axis->MSTSWr_COMWw;
                PCL6046_mstatus[AXIS_Y] = Y_axis->MSTSWr_COMWw;
                PCL6046_mstatus[AXIS_Z] = Z_axis->MSTSWr_COMWw;
                PCL6046_mstatus[AXIS_U] = U_axis->MSTSWr_COMWw;

                xSemaphoreGive(PCL6046_mutex);
            }

            vTaskDelayUntil(&lastTimeHere, (const TickType_t) ASIC_MAINT_PERIOD);
        }
    }

    //  we should never get here
    destroy_PCL6046_resources();
    vTaskDelete(NULL);
}

