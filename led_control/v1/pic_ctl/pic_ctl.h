#ifndef MY_PIC__H__
#define MY_PIC__H__
#include <stdint.h>
#include "pic_reg.h"

#define HZ	(16)

enum {
	PERR_SUCCESS	= 0,	/* ok */
	PERR_NODEV	= -1,	/* Can't open i2c-0 */
	PERR_IOERR	= -2,	/* I2C access failed */
	PERR_INVAL	= -3,	/* Invalidate parameter */
};

enum {
	PIC_LED_NUMBER	= 16,
	
	PIC_LED_OFF	= 0x00,
	PIC_LED_ON	= 0x01,
	PIC_LED_BLINK	= 0x02,
	PIC_LED_B2	= 0x03,

	PIC_LED_FREQ_NORMAL = PIC_HZ / 2,
	PIC_LED_FREQ_SLOW = PIC_HZ,
	PIC_LED_FREQ_FAST = PIC_HZ / 8,


	PIC_LED_STS_MASK	= 0x03,
	PIC_LED_FREQ_SHIFT	= 2,
	PIC_LED_FREQ_MASK	= 0xfc,
};

enum {
	PIC_WDT_START	= 0x55,
	PIC_WDT_STOP	= 0xaa,
};

int pic_read_disk(int disk_id, int *v);
int pic_write_disk(int disk_id, int v);
int pic_init(void);
void pic_release(void);




#endif // MY_PIC__H__
