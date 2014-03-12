#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "i2c-dev.h"
#include "pic_ctl.h"

#define I2C_DEV "/dev/i2c-0"

#define PIC_CHECK_INIT()	do {	\
	int ret = pic_init();		\
	if (ret  < 0)			\
		return ret;		\
}while(0)

	 
static int pic_is_initialized;
static int pic_fd;

int pic_read_disk(int disk_id, int *v)
{
	int ret;
	uint8_t reg;

	PIC_CHECK_INIT();
	if (disk_id >= PIC_LED_NUMBER)
		return PERR_INVAL;
	reg = PIC_LED_START + (uint8_t)disk_id;
	ret = i2c_smbus_read_byte_data(pic_fd, reg);
	if (ret == -1)
		return PERR_IOERR;
	*v = ret;
	return 0;
}

int pic_write_disk(int disk_id, int mode)
{
	int ret;
	uint8_t reg;
	uint8_t v;

	PIC_CHECK_INIT();
	if (disk_id >= PIC_LED_NUMBER)
		return PERR_INVAL;
	reg = PIC_LED_START + (uint8_t)disk_id;
	v = (uint8_t)mode & PIC_LED_STS_MASK;
	ret = i2c_smbus_write_byte_data(pic_fd, reg, v);
	if (ret < 0)
		return PERR_IOERR;
	return 0;
}
int pic_read_disk_2U(int disk_id, int *v)
{
	return 0;
}
int pic_write_disk_2U(int disk_id, int mode)
{
	return 0;
}

int pic_init(void)
{
	int fd;

	if (pic_is_initialized)
		return 0;
	fd = open(I2C_DEV, O_RDWR);
	if (fd < 0)
		return PERR_NODEV;
	if (ioctl(fd, I2C_SLAVE_FORCE, PIC_ADDRESS) < 0) {
		close(fd);
		return PERR_NODEV;
	}

	pic_is_initialized = 1;
	pic_fd = fd;

	return 0;
}

void pic_release(void)
{
	if (pic_is_initialized) {
		close(pic_fd);
		pic_is_initialized = 0;
	}
}
