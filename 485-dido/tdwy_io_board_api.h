#ifndef __TDWY_IO_MCU_API_H__
#define __TDWY_IO_MCU_API_H__

#include <stdint.h>

#define GP0_IN_PORT_REG			0x00	/* GP0_IN_PORT_REG : Read only, 
												This register reflects the respective GP0x pin level */
#define GP0_OUT_PORT_REG			0x01	/* GP0_OUT_PORT_REG : Read/Write */
#define GP0_IO_CFG_REG				0x03	/* GP0_IO_CFG_REG : Read/Write, 
												This register configures the respective GP0x pin as 
 												input mode ( "1"; by default ) or output mode ("0") */


#define GP1_IN_PORT_REG			0x10	/* GP1_IN_PORT_REG : Read only, 
												This register reflects the respective GP0x pin level */
#define GP1_OUT_PORT_REG			0x11	/* GP1_OUT_PORT_REG : Read/Write */
#define GP1_IO_CFG_REG				0x13	/* GP1_IO_CFG_REG : Read/Write, 
												This register configures the respective GP0x pin as 
 												input mode ( "1"; by default ) or output mode ("0") */									

#define GP2_IN_PORT_REG			0x20	/* GP2_IN_PORT_REG : Read only, 
												This register reflects the respective GP0x pin level */	
#define GP2_OUT_PORT_REG			0x21	/* GP2_OUT_PORT_REG : Read/Write */	
#define GP2_IO_CFG_REG				0x23	/* GP2_IO_CFG_REG : Read/Write, 
												This register configures the respective GP0x pin as 
 												input mode ( "1" ) or output mode ("0", by default ) */

/*
 * Init the io board libray
 * return 0 if init success, otherwise, return error code.
*/
int io_board_api_init ( void );

/*
 * Write byte to the register
 * @reg specify the register defined above
 * @value specify the value which will write into the register
 * return 0 if success, otherwise, return error code.
*/
int io_board_write_byte_data ( uint8_t reg, uint8_t value );

/*
 * Read byte from register
 * @reg specify the register defined above
 * @value specify the point which used to store the value read from register
 * return 0 if success, otherwise, return error code.
*/
int io_board_read_byte_data ( uint8_t reg, uint8_t *value );


#endif
