/*************************************************************************
 *  Challenge_1_Firmware:   PCL6046_limit.c
 *                          Thread to set limit function of 4 axes to
 *                          prevent collisions, since all 4 axes are
 *                          in the same physical dimension for this application.
 *
 *  Engineer:               Larry Pelton
 *
  ************************************************************************/
#define		PCL6046_LIMIT_C

#include	<stdint.h>
#include	<stdbool.h>

#include	"PCL6046.h"
#include    "PCL6046_maint.h"
#include    "PCL6046_comm.h"
#include    "PCL6046_limit.h"


/*************************************************************************
 *  @brief      ASIC_limit
 *              This RTOS task monitors positions of each axis from the
 *              COUNTER1 values and adjust the software limits of each
 *              axis to prevent collisions.
 *  @param[in]  pvParameters points to the message from the USB user that
 *              caused this thread to be created
 *  @returns    none
 ************************************************************************/

void ASIC_limit(void *pvParameters)
{
    //  create a local copy of the information referenced by the calling task
    USB_ASIC_Q_t queueMsg = *((USB_ASIC_Q_t *) pvParameters);
    
    //  flag to the calling task that the information has been copied/consumed
    (void) xSemaphoreGive(comm_data_consumed);

    //  4 data words are available from the USB message; I'll assume that a common
    // separation is being specified by the user, so only the 1st data word is used
    int32_t userLimit = (int32_t) queueMsg.data1;
    int32_t axialPositions[4] = {0};
    uint32_t temp;

    TickType_t lastTimeHere = xTaskGetTickCount();

    //  ***********************************************************
    //  enable software limits per axis:
    //  ***********************************************************
    //  set axis X for software limit in the + direction only
    temp = ((ReadReg(RENV4, AXIS_X) & 0xFFFF0000) | 0x0038);
    WriteReg(RENV4, AXIS_X, temp);

    //  set axis Y for software limit in both directions
    temp = ((ReadReg(RENV4, AXIS_Y) & 0xFFFF0000) | 0x3838);
    WriteReg(RENV4, AXIS_Y, temp);

    //  set axis Z for software limit in both directions
    temp = ((ReadReg(RENV4, AXIS_Z) & 0xFFFF0000) | 0x3838);
    WriteReg(RENV4, AXIS_Z, temp);

    //  set axis U for software limit in the - direction only
    temp = ((ReadReg(RENV4, AXIS_U) & 0xFFFF0000) | 0x3800);
    WriteReg(RENV4, AXIS_U, temp);
    //  ***********************************************************
    
    while (1)
    {
        //  comparator values for limits between X and Y
        int32_t compValXplus;
        int32_t compValYminus;
        //  comparator values for limits between Y and Z
        int32_t compValYplus;
        int32_t compValZminus;
        //  comparator values for limits between Z and U
        int32_t compValZplus;
        int32_t compValUminus;

        //  COUNTER1 is assumed to hold the current position of each axis; read COUNTER1
        //  of each axis
        read_registers(RCUN1, 0x0F, (uint32_t *) axialPositions);

        //  calculate limits to prevent X and Y from colliding
        //  if there's adequate space between X and Y to set limits ...
        if ((axialPositions[1] - axialPositions[0]) > userLimit)
        {
            compValXplus    = axialPositions[1] - userLimit;
            compValYminus   = axialPositions[0] + userLimit;
        }
        //  else stop now by specifying limit at current position
        else
        {
            compValXplus    = axialPositions[0];
            compValYminus   = axialPositions[1];    
        }

        //  set the limits between X and Y
        WriteReg(RCMP1, AXIS_X, compValXplus);
        WriteReg(RCMP2, AXIS_Y, compValYminus);

        //  calculate limits to prevent Y and Z from colliding
        //  if there's adequate space between Y and Z to set limits ...
        if ((axialPositions[2] - axialPositions[1]) > userLimit)
        {
            compValYplus    = axialPositions[2] - userLimit;
            compValZminus   = axialPositions[1] + userLimit;
        }
        //  else stop now
        else
        {
            compValYplus    = axialPositions[1];
            compValZminus   = axialPositions[2];
        }

        //  set the limits between Y and Z
        WriteReg(RCMP1, AXIS_Y, compValYplus);
        WriteReg(RCMP2, AXIS_Z, compValZminus);

        //  calculate limits to prevent Z and U from colliding
        //  if there's adequate space between Z and U to set limits ...
        if ((axialPositions[3] - axialPositions[2]) > userLimit)
        {
            compValZplus    = axialPositions[3] - userLimit;
            compValUminus   = axialPositions[2] + userLimit;
        }
        //  else stop now
        else
        {
            compValZplus    = axialPositions[2];
            compValUminus   = axialPositions[3];
        }

        //  set the limits between Z and U
        WriteReg(RCMP1, AXIS_Z, compValZplus);
        WriteReg(RCMP2, AXIS_U, compValUminus);

        vTaskDelayUntil(&lastTimeHere, (const TickType_t) POSITION_MONITOR_PERIOD);
    }

    //  this should never be executed
    vTaskDelete(NULL);
}


/*************************************************************************
 *  @brief      ASIC_limit_indicators
 *              This thread assumes 1 LED exists per axis.  An LED is lit
 *              if the motor has stopped due to software limits.
 *  @param[in]  pvParameters is unused here
 *  @returns    none
 ************************************************************************/
void ASIC_limit_indicators(void *pvParameters)
{
    TickType_t lastTimeHere = xTaskGetTickCount();
    MOTION_AXIS axis = AXIS_X;

    while (1)
    {
        uint16_t currStat = get_axial_status(axis);
    
        //  if the comparator 1 or 2 condition is satisfied, indicating STOP
        //  in the forward or reverse direction, then light the corresponding
        //  LED
        if (currStat & 0x30)
        {
            light_LED(axis);
        }

        //  otherwise, extinguish the LED
        else
        {
            extinguish_LED(axis);
        }

        //  advance to the next axis; one axis is handled every LED_UPDATE_PERIOD
        //  milliseconds
        if (++axis >= AXISCNT)
        {
            axis = AXIS_X;
        }      

        vTaskDelayUntil(&lastTimeHere, (const TickType_t) LED_UPDATE_PERIOD);
    }

    vTaskDelete(NULL);
}
