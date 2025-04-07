/*************************************************************************
 *	Challenge_1_Firmware:	PCL6046_comm.h
 *
 *  Engineer:				Larry Pelton
 *
  ************************************************************************/
#ifndef     PCL6046_COMM_H
    #define PCL6046_COMM_H

    //  the enumerated values represent actions that the USB user can demand be
    //  performed by the ASIC
    typedef enum
    {
        INDICATE_STOPS  =   1,
        ANTI_COLLIDE    =   2,
        LAST_MSG        =   0xFF
    }   USB_ASIC_e;
    
    //  ASIC_comm_queue elements are of this type;  this allows the USB interface
    //  to specify an action (via opcode) and supply 4 pieces of 32-bit data, one
    //  per axis of the motion controller; this might need reworking, depending on
    //  the needs of an actual system
    typedef struct
    {
        USB_ASIC_e  opcode;
        uint32_t    data1;
        uint32_t    data2;
        uint32_t    data3;
        uint32_t    data4;
    
    }   USB_ASIC_Q_t;

    //  for now, assume that 16 elements is enough that the queue won't overrun
    #define ASIC_COMM_QUEUE_SIZE    16

    //  wait this many milliseconds for a created task to copy the queue'd USB user
    //  data, to ensure the data isn't destroyed before that happens
    #define TASK_STARTUP_DELAY      25

    #ifdef  PCL6046_COMM_C
        
        //  queue handle for messages received over USB and applicable to ASIC operations
        QueueHandle_t       ASIC_comm_queue     = (QueueHandle_t) NULL;
        SemaphoreHandle_t   comm_data_consumed  = (SemaphoreHandle_t) NULL;

    #else

        extern QueueHandle_t        ASIC_comm_queue;
        extern SemaphoreHandle_t    comm_data_consumed;

        void ASIC_comm(void *pvParameters);
    #endif
#endif