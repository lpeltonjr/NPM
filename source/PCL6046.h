/*************************************************************************
 *	Challenge_1_Firmware:	PCL6046.h
 * 							Definitions for CPU-to-PCL6046 Interface
 *
 *  Engineer:				Larry Pelton
 *
 * 							NOTE that it is assumed the controller board
 * 							is configured to select Motorola 68000
 * 							communication for STM32 to ASIC.
 ************************************************************************/
#ifndef		PCL6046_H
	#define	PCL6046_H

	#define	BASE_TASK_PRI   2

	//	TODO:	populate this macro from the motion controller schematic;
	//	hardware-specific macro for reading IFB (Interface Busy) pin of PCL6046
	#define	IFB_HIGH()		()

	//	TODO:	populate this macro with a call to implement > 1us sleep,
	//			to satisfy timing requirements of PCL6046 communications
	//			("2 cycles of CLK signals", section 5.1.4.2)
	#define microsleep()	()

	typedef enum
	{
		AXIS_X	=	0,
		AXIS_Y	=	1,
		AXIS_Z	=	2,
		AXIS_U	=	3,
		AXISCNT	=	4
	}	MOTION_AXIS;

	//	create a type representing the PCL6046 register block for a single
	//	axis
	typedef struct
	{
		uint32_t	RIPS_reg;		//	offset 0x00
		uint32_t	unused_0;		//	offset 0x04
		uint32_t	RCIC_reg;		//	offset 0x08
		uint32_t	RCI_reg;		//	offset 0x0C
		uint32_t	unused_1[5];	//	offset 0x10 - 0x23
		uint32_t	RSDC_reg;		//	offset 0x24
		uint32_t	RSPD_reg;		//	offset 0x28
		uint32_t	RPLS_reg;		//	offset 0x2C
		uint32_t	RIST_reg;		//	offset 0x30
		uint32_t	REST_reg;		//	offset 0x34
		uint32_t	RSTS_reg;		//	offset 0x38
		int32_t		RLTC4_reg;		//	offset 0x3C
		int32_t		RLTC3_reg;		//	offset 0x40
		int32_t		RLTC2_reg;		//	offset 0x44
		int32_t		RLTC1_reg;		//	offset 0x48
		uint32_t	RIRQ_reg;		//	offset 0x4C
		int32_t		RCMP5_reg;		//	offset 0x50
		int32_t		RCMP4_reg;		//	offset 0x54
		int32_t		RCMP3_reg;		//	offset 0x58
		int32_t		RCMP2_reg;		//	offset 0x5C
		int32_t		RCMP1_reg;		//	offset 0x60
		int32_t		RCUN4_reg;		//	offset 0x64
		int32_t		RCUN3_reg;		//	offset 0x68
		int32_t		RCUN2_reg;		//	offset 0x6C
		int32_t		RCUN1_reg;		//	offset 0x70
		uint32_t	RENV7_reg;		//	offset 0x74
		uint32_t	RENV6_reg;		//	offset 0x78
		uint32_t	RENV5_reg;		//	offset 0x7C
		uint32_t	RENV4_reg;		//	offset 0x80
		uint32_t	RENV3_reg;		//	offset 0x84
		uint32_t	RENV2_reg;		//	offset 0x88
		uint32_t	RENV1_reg;		//	offset 0x8C
		uint32_t	RFA_reg;		//	offset 0x90
		uint32_t	RDS_reg;		//	offset 0x94
		uint32_t	RUS_reg;		//	offset 0x98
		int32_t		RIP_reg;		//	offset 0x9C
		uint32_t	RMD_reg;		//	offset 0xA0
		uint32_t	RDP_reg;		//	offset 0xA4
		uint32_t	RMG_reg;		//	offset 0xA8
		uint32_t	RDR_reg;		//	offset 0xAC
		uint32_t	RUR_reg;		//	offset 0xB0
		uint32_t	RFH_reg;		//	offset 0xB4
		uint32_t	RFL_reg;		//	offset 0xB8
		int32_t		RMV_reg;		//	offset 0xBC
		uint32_t	unused02;		//	offset 0xC0
		uint32_t	PRCI_reg;		//	offset 0xC4
		uint32_t	PRCP5_reg;		//	offset 0xC8
		uint32_t	PRDS_reg;		//	offset 0xCC
		uint32_t	PRUS_reg;		//	offset 0xD0
		uint32_t	PRIP_reg;		//	offset 0xD4
		uint32_t	PRMD_reg;		//	offset 0xD8
		uint32_t	PRDP_reg;		//	offset 0xDC
		uint32_t	PRMG_reg;		//	offset 0xE0
		uint32_t	PRDR_reg;		//	offset 0xE4
		uint32_t	PRUR_reg;		//	offset 0xE8
		uint32_t	PRFH_reg;		//	offset 0xEC
		uint32_t	PRFL_reg;		//	offset 0xF0
		uint32_t	PRMV_reg;		//	offset 0xF4
		uint16_t	BUFW1_reg;		//	offset 0xF8
		uint16_t	BUFW0_reg;		//	offset 0xFA
		uint16_t	SSTSWr_OTPWw;	//	offset 0xFC: read SSTSW, write OTPW
		uint16_t	MSTSWr_COMWw;	//	offset 0xFE: read MSTSW, write COMW

	}	AXIS_MAP;

	//	all PCL6046 commands
	typedef enum
	{
		AC_NOP	=	0x00,
		SRST 	=	0x04,
		CMEMG	=	0x05,	//	emergency stop
		CMSTA	=	0x06,	//	energize CSTA pin
		CMSTP	=	0x07,	//	simultaneous stop

		P0RST	=	0x10,
		P1RST	=	0x11,
		P2RST	=	0x12,
		P3RST	=	0x13,
		P4RST	=	0x14,
		P5RST	=	0x15,
		P6RST	=	0x16,
		P7RST	=	0x17,
		P0SET	=	0x18,
		P1SET	=	0x19,
		P2SET	=	0x1A,
		P3SET	=	0x1B,
		P4SET	=	0x1C,
		P5SET	=	0x1D,
		P6SET	=	0x1E,
		P7SET	=	0x1F,
		CUN1R	=	0x20,
		CUN2R	=	0x21,
		CUN3R	=	0x22,
		CUN4R	=	0x23,
		ERCOUT	=	0x24,
		ERCRST	=	0x25,
		PRECAN	=	0x26,
		PCPCAN	=	0x27,
		STAON 	=	0x28,
		LTCH	=	0x29,
		SPSTA	=	0x2A,
		PRESHF	=	0x2B,
		PCPSHF	=	0x2C,
		SENIR	=	0x2D,
		SEORR	=	0x2E,

		STOP	=	0x49,
		SDSTP	=	0x4A,

		PRSET	=	0x4F,

		STAFL	=	0x50,	//	FL constant speed start
		STAFH	=	0x51,	//	FH constant speed start
		STAD	=	0x52,	//	high speed start 1
		STAUD	=	0x53,	//	high speed start 2

		CNTFL	=	0x54,
		CNTFH	=	0x55,
		CNTD	=	0x56,
		CNTUD	=	0x57,

		LASTCMD	=	0xFF

	}	ASIC_CMD;


	//	enumeration for reading/writing registers indirectly;
	//	to read, use the enumerated value; to write (if permissible),
	//	XOR the enumerated value with 0x40
	typedef enum
	{
		PRMV	=	0xC0,
		PRFL	=	0xC1,
		PRFH	=	0xC2,
		PRUR	=	0xC3,
		PRDR	=	0xC4,
		PRMG	=	0xC5,
		PRDP	=	0xC6,
		PRMD	=	0xC7,
		PRIP	=	0xC8,
		PRUS	=	0xC9,
		PRDS	=	0xCA,
		PRCP5	=	0xCB,
		PRCI	=	0xCC,

		RMV		=	0xD0,
		RFL		=	0xD1,
		RFH		=	0xD2,
		RUR		=	0xD3,
		RDR		=	0xD4,
		RMG		=	0xD5,
		RDP		=	0xD6,
		RMD		=	0xD7,
		RIP		=	0xD8,
		RUS		=	0xD9,
		RDS		=	0xDA,
		RFA		=	0xDB,
		RENV1	=	0xDC,
		RENV2	=	0xDD,
		RENV3	=	0xDE,
		RENV4	=	0xDF,
		RENV5	=	0xE0,
		RENV6	=	0xE1,
		RENV7	=	0xE2,
		RCUN1	=	0xE3,
		RCUN2	=	0xE4,
		RCUN3	=	0xE5,
		RCUN4	=	0xE6,
		RCMP1	=	0xE7,
		RCMP2	=	0xE8,
		RCMP3	=	0xE9,
		RCMP4	=	0xEA,
		RCMP5	=	0xEB,
		RIRQ	=	0xEC,
		RLTC1	=	0xED,
		RLTC2	=	0xEE,
		RLTC3	=	0xEF,
		RLTC4	=	0xF0,
		RSTS	=	0xF1,
		REST	=	0xF2,
		RIST	=	0xF3,
		RPLS	=	0xF4,
		PSPD	=	0xF5,
		RSDC	=	0xF6,
		RCI		=	0xFC,
		RCIC	=	0xFD,
		RIPS	=	0xFF

	}	ASIC_REG;

	//	axis selection written to upper byte of COMW register
	#define	AXIS_X_MASK	1
	#define AXIS_Y_MASK 2
	#define AXIS_Z_MASK 4
	#define	AXIS_U_MASK	8

	#ifdef	PCL6046_C

		//	assume that the PCL6046 ASIC is interfaced to the STM32 via the
		//	FMC external memory controller, memory bank #1;
		//	the STM32 would be initialized in code I haven't written, dependent
		//	on the particular microcontroller chosen
		AXIS_MAP *X_axis = (AXIS_MAP *) 0x60000000;
		AXIS_MAP *Y_axis = (AXIS_MAP *) 0x60000100;
		AXIS_MAP *Z_axis = (AXIS_MAP *) 0x60000200;
		AXIS_MAP *U_axis = (AXIS_MAP *) 0x60000300;

		//	mutex for preventing conflicting access to PCL6046 ASIC
		static SemaphoreHandle_t PCL6046_mutex = NULL;


	#else
		void write_register(ASIC_REG register, uint8_t axis, uint32_t value);
		void read_registers(ASIC_REG register, uint8_t axis, uint32_t *results);
		uint32_t ReadReg(ASIC_REG RegName, MOTION_AXIS axis);
		void WriteReg(ASIC_REG RegName, MOTION_AXIS axis, uint32_t value);
		bool init_PCL6046_resources(void);
		void destroy_PCL6046_resources(void);

	#endif

#endif
