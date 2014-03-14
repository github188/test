#ifndef PIC_CTL_H
#define PIC_CTL_H

#include <stdint.h>
#include "pic_reg.h"

#define HZ	(16)
#define PIC_CONF_2U	(0x22)
enum {
	PERR_SUCCESS	= 0,	/* ok */
	PERR_NODEV	= -1,	/* Can't open i2c-0 */
	PERR_IOERR	= -2,	/* I2C access failed */
	PERR_INVAL	= -3,	/* Invalidate parameter */
};

enum {
	PIC_LED_NUMBER	= 16,
	PIC_LED_NUMBER_3U = 16,
	PIC_LED_NUMBER_2U = 8,


	PIC_GP1_MODE1	= 0x01,
	PIC_GP1_MODE2	= 0x03,
	PIC_GP2_MODE1	= 0x11,
	PIC_GP2_MODE2	= 0x13,

	PIC_LED_OFF_S	= 0x1,
	PIC_LED_ON_S	= 0x0,
	
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

	PIC_LED_DISK_MASK_2U	= 0x3,
};

enum {
	PIC_WDT_START	= 0x55,
	PIC_WDT_STOP	= 0xaa,
};

int pic_init(void);
void pic_release(void);
int pic_get_version(uint32_t *version);
int pic_set_led(uint8_t led, uint8_t sts, uint8_t freq);
int pic_start_watchdog(void);
int pic_stop_watchdog(void);

int pic_write_disk_2U(int disk_id, int v);
int pic_write_disk_3U(int disk_id, int v);
int pic_init_3U(void);
int pic_init_2U(void);
int sb_gpio28_set(int);

#endif
