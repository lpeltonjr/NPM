/*************************************************************************
 *  Challenge_1_Firmware:   PCL6046_limit.h
 *
 *  Engineer:               Larry Pelton
 *
 ************************************************************************/
#ifndef     PCL6046_LIMIT_H
    #define PCL6046_LIMIT_H

    #define POSITION_MONITOR_PERIOD     50
    #define LED_UPDATE_PERIOD           25

    //  TODO:   populate these macros based on the hardware used;
    //          x is 0 to 3, implying 4 LEDs
    #define light_LED(x)        ()
    #define extinguish_LED(x)   ()

    #ifdef  PCL6046_LIMIT_C

    #else
        void ASIC_limit(void *pvParameters);
        void ASIC_limit_indicators(void *pvParameters);
    #endif
#endif


