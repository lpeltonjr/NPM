/*************************************************************************
 *	Challenge_1_Firmware:	PCL6046_comm.c
 * 							Handles USB and SOC/ASIC communication, as
 *                          specified in challenge description
 *
 *  Engineer:				Larry Pelton
 *
  ************************************************************************/
#define		PCL6046_COMM_C

#include	<stdint.h>
#include	<stdbool.h>

#include	"PCL6046.h"
#include    "PCL6046_maint.h"
#include    "PCL6046_comm.h"
#include    "PCL6046_limit.h"


/*************************************************************************
 * @brief		ASIC_comm
 * 				This RTOS task handles execution of ASIC tasks demanded
 *              by the user over USB.  This task will pend until something
 *              is received from the USB interface, which isn't shown and
 *              only theorized for the purpose of this challenge.
 * @param[in]	pvParameters is ignored, currently
 * @returns		none
 ************************************************************************/
void ASIC_comm(void *pvParameters)
{
    //  create the queue for routing messages from the USB interface to this
    //  task
    if ((ASIC_comm_queue = xQueueCreate(ASIC_COMM_QUEUE_SIZE, (UBaseType_t) sizeof(USB_ASIC_Q_t))) != NULL)
    {
        if ((comm_data_consumed = xSemaphoreCreateBinary()) != NULL)
        {
            //  a current message extracted from the queue is written to this local
            //  variable for processing
            USB_ASIC_Q_t queueMsg = {0};
            //  keep a handle on the software limits task, because it must be deleted and restarted if the
            //  user updates the limits
            TaskHandle_t limitTask = (TaskHandle_t) NULL;

            while (1)
            {
                //  wait for the external USB handler to send an ASIC-related message, and copy
                //  it to queueMsg when that happens
                (void) xQueueReceive(ASIC_comm_queue, (void *) &queueMsg, portMAX_DELAY);

                //  process the message
                switch (queueMsg.opcode)
                {
                    //  this is the feature required by the challenge; execute it with a task
                    case ANTI_COLLIDE:
                        //  if it's already running, delete it and create a new instance
                        //  TODO:   Ensure whether the task is deleted immediately or whether we
                        //          must loop checking the task status until it's dead; I seem to remember
                        //          having issues with this in the past
                        if (limitTask != (TaskHandle_t) NULL)
                        {
                            vTaskDelete(limitTask);
                            limitTask = (TaskHandle_t) NULL;    
                        }
                        (void) xTaskCreate(ASIC_limit, "limit", configMINIMAL_STACK_SIZE, (void *) &queueMsg, (uxTaskPriorityGet(NULL) + 1), &limitTask);
                        break;

                    //  light a corresponding LED whenever a motor stops due to software limits (low priority)
                    case INDICATE_STOPS:
                        (void) xTaskCreate(ASIC_limit_indicators, "leds", configMINIMAL_STACK_SIZE, (void *) NULL, BASE_TASK_PRI, NULL);                   
                        break;

                    default:
                        break;
                }

                //  wait for whatever task was created above to copy the data it needs from
                //  queueMsg before allowing it to be overwritten
                xSemaphoreTake(comm_data_consumed, TASK_STARTUP_DELAY);
            }
        }
    }

    if (comm_data_consumed != (SemaphoreHandle_t) NULL)
    {
        vSemaphoreDelete(comm_data_consumed);
        comm_data_consumed = (SemaphoreHandle_t) NULL;
    }

    if (ASIC_comm_queue != (QueueHandle_t) NULL)
    {
        vQueueDelete(ASIC_comm_queue);
        ASIC_comm_queue = (QueueHandle_t) NULL;
    }

    vTaskDelete(NULL);
}

