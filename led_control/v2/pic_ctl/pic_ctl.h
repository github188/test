#ifndef MY_PIC__H__
#define MY_PIC__H__
#include <stdint.h>
#include "pic_reg.h"

#define HZ	(16)

#define PIC_CONF_2U	(0x22)
enum {
	PIC_ADDRESS_2U	= (0x30 >> 1),
	PIC_ADDRESS_3U1 = (0x3c >> 1),
	PIC_ADDRESS_3U2 = (0x3e >> 1),
};

enum {
	PERR_SUCCESS	= 0,	/* ok */
	PERR_NODEV	= -1,	/* Can't open i2c-0 */
	PERR_IOERR	= -2,	/* I2C access failed */
	PERR_INVAL	= -3,	/* Invalidate parameter */
};

enum {
	PIC_LED_NUMBER_3U = 16,
	PIC_LED_NUMBER_2U = 8,

	PIC_GP1_MODE1	= 0x01,
	PIC_GP1_MODE2	= 0x03,
	PIC_GP2_MODE1	= 0x11,
	PIC_GP2_MODE2	= 0x13,
	
	PIC_LED_OFF	= 0x1,
	PIC_LED_ON	= 0x0,

	PIC_LED_DISK_MASK_2U	= 0x3,

};


int pic_write_disk_2U(int disk_id, int v);
int pic_write_disk_3U(int disk_id, int v);
int pic_init_3U(void);
int pic_init_2U(void);
void pic_release(void);
int sb_gpio28_set(int);




#endif // MY_PIC__H__
