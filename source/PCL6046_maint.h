/*************************************************************************
 *  Challenge_1_Firmware:   PCL6046_maint.h
 *
 *  Engineer:               Larry Pelton
 *
 ************************************************************************/
#ifndef         PCL6046_MAINT_H
    #define     PCL6046_MAINT_H

    #define     ASIC_MAINT_PERIOD   10    

    #ifdef      PCL6046_MAINT_C

        //  PCL6046 main status register
        static uint16_t    PCL6046_mstatus[AXISCNT];

    #else
        uint16_t get_axial_status(MOTION_AXIS axis);
        void ASIC_maintenance(void *pvParameters);
    #endif
#endif

