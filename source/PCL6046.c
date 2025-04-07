/*************************************************************************
 *	Challenge_1_Firmware:	PCL6046.c
 * 							CPU-to-PCL6046 Interface
 *
 *  Engineer:				Larry Pelton
 *
 * 							NOTE that it is assumed the controller board
 * 							is configured to select Motorola 68000
 * 							communication for STM32 to ASIC.
 ************************************************************************/
#define		PCL6046_C

#include	<stdint.h>
#include	<stdbool.h>

#include	"PCL6046.h"



/*************************************************************************
 * @brief		write_command
 * 				Primitive function for writing a command to PCL6046.
 * @param[in]	command is the command word taken from enumeration ASIC_CMD.
 * @param[in]	axis is a bitfield where axis:bit == X:0, Y:1, Z:2, U:3
 * @returns		none
 ************************************************************************/
void write_command(ASIC_CMD command, uint8_t axis)
{
	//	construct the command word according to section 5.1.3 of the
	//	PCL6046 user manual
	uint16_t commWord = ((uint16_t) axis << 8) + (uint16_t) command;

	//	reserve the motion controller chip's comm interface for use by this
	//	thread
	(void) xSemaphoreTake(PCL6046_mutex, portMAX_DELAY);

	//	the axis is selected by the command bits, so we can write it to the
	//	X axis address space according to section 5.1.3 of the PCL6046 user manual
	X_axis->MSTSWr_COMWw = commWord;

	//	see section 5.1.3 of PCL6046 user manual; assume that WRQ isn't connected
	//	since this is STM32, not 68000; this should block for no more than
	//	203ns, which is 4 cycles of CLK (19.6608MHz); it is significantly less
	//	than the RTOS tick time, so it makes little sense to use an RTOS
	//	delay here
	while (!IFB_HIGH());

	//	release the comm interface
	(void) xSemaphoreGive(PCL6046_mutex);
}


/*************************************************************************
 * @brief		write_register
 * 				Primitive function for writing to a 32-bit PCL6046 ASIC
 * 				register.
 * @param[in]	register is an enumerated register read command taken from
 * 				section 5.3.2.10 of the PCL6046 user manual; the write
 * 				register command is derived by clearing bit 6
 * @param[in]	axis is a bitfield where axis:bit == X:0, Y:1, Z:2, U:3
 * @param[in]	value is what's to be written to the register
 * @returns		none
 ************************************************************************/
void write_register(ASIC_REG register, uint8_t axis, uint32_t value)
{
	uint16_t upperVal = (uint16_t) (value >> 16);
	uint16_t lowerVal = (uint16_t) value;

	//	construct the write register command word, selecting the specified
	//	axes
	uint16_t commWord = ((uint16_t) axis << 8) + (uint16_t) (register ^ 0x40);

	//	reserve the motion controller chip's comm interface for use by this
	//	thread
	(void) xSemaphoreTake(PCL6046_mutex, portMAX_DELAY);

	//	"set the write data in I/O buffer of each axis", section 5.1.4.2
	//	of PCL6046 user manual
	if (axis & 1)
 	{
 		X_axis->BUFW1_reg = upperVal;
 		microsleep();
 		X_axis->BUFW0_reg = lowerVal;
	}

	if (axis & 2)
	{
		Y_axis->BUFW1_reg = upperVal;
		microsleep();
		Y_axis->BUFW0_reg = lowerVal;
	}

	if (axis & 4)
	{
		Z_axis->BUFW1_reg = upperVal;
		microsleep();
		Z_axis->BUFW0_reg = lowerVal;
	}

	if (axis & 8)
	{
		U_axis->BUFW1_reg = upperVal;
		microsleep();
		U_axis->BUFW0_reg = lowerVal;
	}

	microsleep();

	//	the axis is selected by the command bits, so we can write it to the
	//	X axis address space according to section 5.1.3 of the PCL6046 user manual
	X_axis->MSTSWr_COMWw = commWord;

	//	see section 5.1.3 of PCL6046 user manual; assume that WRQ isn't connected
	//	since this is STM32, not 68000; this should block for no more than
	//	203ns, which is 4 cycles of CLK (19.6608MHz); it is significantly less
	//	than the typical RTOS tick time, so it makes little sense to use an RTOS
	//	delay here
	while (!IFB_HIGH());

	//	release the comm interface
	(void) xSemaphoreGive(PCL6046_mutex);
}

/*************************************************************************
 * @brief		read_registers
 * 				Primitive function for reading from 32-bit PCL6046 ASIC
 * 				register in 1 to 4 axes.
 * @param[in]	register is an enumerated register read command taken from
 * 				section 5.3.2.10 of the PCL6046 user manual
 * @param[in]	axis is a bitfield where axis:bit == X:0, Y:1, Z:2, U:3
 * @param[out]	results points to an array of 4, 32-bit register values;
 * 				index 0 corresponds to the X axis, index 3 corresponds to
 * 				the U axis
 * @returns		none
 ************************************************************************/
void read_registers(ASIC_REG register, uint8_t axis, uint32_t *results)
{
	//	construct the read register command word, selecting the specified
	//	axes
	uint16_t commWord = ((uint16_t) axis << 8) + (uint16_t) register;

	//	reserve the motion controller chip's comm interface for use by this
	//	thread
	(void) xSemaphoreTake(PCL6046_mutex, portMAX_DELAY);

	//	the axis is selected by the command bits, so we can write it to the
	//	X axis address space according to section 5.1.3 of the PCL6046 user manual
	X_axis->MSTSWr_COMWw = commWord;

	//	see section 5.1.3 of PCL6046 user manual; assume that WRQ isn't connected
	//	since this is STM32, not 68000; this should block for no more than
	//	203ns, which is 4 cycles of CLK (19.6608MHz); it is significantly less
	//	than the typical RTOS tick time, so it makes little sense to use an RTOS
	//	delay here
	while (!IFB_HIGH());

	//	collect a result for each specified axis, writing it to the result
	//	array in the calling routine
	if (axis & 1)
 	{
 		*(results + 0) = ((uint32_t) X_axis->BUFW1_reg << 16) + (uint32_t) X_axis->BUFW0_reg;
	}

	if (axis & 2)
	{
		*(results + 1) = ((uint32_t) Y_axis->BUFW1_reg << 16) + (uint32_t) Y_axis->BUFW0_reg;
	}

	if (axis & 4)
	{
		*(results + 2) = ((uint32_t) Z_axis->BUFW1_reg << 16) + (uint32_t) Z_axis->BUFW0_reg;
	}

	if (axis & 8)
	{
		*(results + 3) = ((uint32_t) U_axis->BUFW1_reg << 16) + (uint32_t) U_axis->BUFW0_reg;
	}

	//	release the comm interface
	(void) xSemaphoreGive(PCL6046_mutex);
}

/*************************************************************************
 * @brief		ReadReg
 * 				This is the read register routine required by the challenge.
 * 				I think the axis is required, so I added it as an argument.
 * @param[in]	RegName is the register name, taken from enumeration ASIC_REG
 * @param[in]	axis identifies the X, Y, Z, or U axis
 * @returns		the value read from the specified register
 ************************************************************************/
uint32_t ReadReg(ASIC_REG RegName, MOTION_AXIS axis)
{
	uint32_t regSet[AXISCNT];

	read_registers(RegName, (uint8_t) (1 << axis), regSet);

	return (regSet[axis]);
}

/*************************************************************************
 * @brief		WriteReg
 * 				This is the write register routine required by the challenge.
 * 				I think the axis is required, so I added it as an argument.
 * @param[in]	RegName is the register name, taken from enumeration ASIC_REG
 * @param[in]	axis identifies the X, Y, Z, or U axis
 * @param[in]	value is the value to be written
 * @returns		none
 ************************************************************************/
void WriteReg(ASIC_REG RegName, MOTION_AXIS axis, uint32_t value)
{
	write_register(RegName, (uint8_t) (1 << axis), value);
}

/*************************************************************************
 * @brief		init_PCL6046_resources
 * 				Creates static RTOS entities needed to for the PCL6046 interface.
 * @returns		true, if no errors were encountered; false, otherwise
 ************************************************************************/
bool init_PCL6046_resources(void)
{
	bool success = true;

	//	create the comm mutex, if it hasn't been created yet
	if (PCL6046_mutex == (SemaphoreHandle_t) NULL)
	{
		//	if the mutex can't be created, that's a fatal error
		if ((PCL6046_mutex = xSemaphoreCreateMutex()) == NULL)
		{
			success = false;
		}
	}

	return (success);
}

/*************************************************************************
 * @brief		destroy_PCL6046_resources
 * 				Deletes static RTOS entities needed to for the PCL6046 interface.
 * @returns		none
 ************************************************************************/
void destroy_PCL6046_resources(void)
{
	if (PCL6046_mutex != (SemaphoreHandle_t) NULL)
	{
		vSemaphoreDelete(PCL6046_mutex);
		PCL6046_mutex = (SemaphoreHandle_t) NULL;
	}
}
