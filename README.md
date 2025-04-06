# NPM
NPM Firmware Challenge

The challenge is described in Challenge_1_Firmware.pdf.  The source code I've created is in the source directory.

Main.c creates an ASIC comm task and a periodic task that reads the ASIC status in every axis.

PCL6046.c/.h contains primitive functions for reading and writing to the ASIC, assuming a parallel interface configured for a Motorola 68000 microprocessor.

PCL6046_maint.c/.h contains the periodic function that reads the ASIC status.

PCL6046_comm.c/.h contains the ASIC comm task that receives queued instructions from the hypothetical USB interface and launches tasks to implement the required ASIC functions.

PCL6046_limit.c/.h contains my approach (using what I've been able to figure out from the ASIC datasheet regarding its operation) to implementing software limits for preventing 4 carriers on a common track from colliding.  I've also included a task for lighting 1 of 4 hypothetical LEDs whenever a carrier is stopped by a limit.

The code is thoroughly documented in comments.
