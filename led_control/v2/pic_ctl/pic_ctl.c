#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "i2c-dev.h"
#include "pic_ctl.h"

#define I2C_DEV "/dev/i2c-0"



#define PIC_CHECK_INIT_2U()	do {		\
		int ret = pic_init_2U();	\
		if (ret  < 0)			\
			return ret;		\
	} while(0)

#define PIC_CHECK_INIT_3U1()	do {		\
		int ret = pic_init_3U1();	\
		if (ret < 0)			\
			return ret;		\
	} while(0)

#define PIC_CHECK_INIT_3U2()	do {		\
		int ret = pic_init_3U2();	\
		if (ret < 0)			\
			return ret;		\
	} while(0)

static int pic_is_initialized;
static int pic_fd;

static int __pic_read_reg(uint8_t reg, uint8_t *v)
{
	int ret;

	ret = i2c_smbus_read_byte_data(pic_fd, reg);
	if (ret == -1)
		return PERR_IOERR;
	*v = ret;
	return 0;
}

static int __pic_write_reg(uint8_t reg, uint8_t v)
{
	int ret;

	ret = i2c_smbus_write_byte_data(pic_fd, reg, v);
	if (ret < 0)
		return PERR_IOERR;
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

#define PIC_CHECK_INIT()	do {		\
	int ret = pic_init();			\
	if (ret < 0)				\
		return ret;			\
} while (0)

int pic_get_version(uint32_t *version)
{
	uint8_t vl, vh;
	int ret;

	PIC_CHECK_INIT();
	ret = __pic_read_reg(PIC_VERH, &vh);
	if (ret < 0)
		return ret;
	ret = __pic_read_reg(PIC_VERL, &vl);
	if (ret < 0)
		return ret;

	*version = (vh << 8) | vl;

	return 0;
}

int pic_set_led(uint8_t led, uint8_t sts, uint8_t freq)
{
	uint8_t v;
	uint8_t reg;

	PIC_CHECK_INIT();
	if (led >= PIC_LED_NUMBER)
		return PERR_INVAL;
	reg = PIC_LED_START + led;
	v = (freq << PIC_LED_FREQ_SHIFT) | (sts & PIC_LED_STS_MASK);

	return __pic_write_reg(reg, v);
}

int pic_start_watchdog(void)
{
	PIC_CHECK_INIT();
	return __pic_write_reg(PIC_WDT, PIC_WDT_START);
}

int pic_stop_watchdog(void)
{
	PIC_CHECK_INIT();
	return __pic_write_reg(PIC_WDT, PIC_WDT_STOP);
}

int pic_reset_timer(int sec)
{
	PIC_CHECK_INIT();
	return __pic_write_reg(PIC_HDD_RESET_TIMER, sec * HZ);
}
int pic_reset_hd(int idx)
{
	PIC_CHECK_INIT();
	return __pic_write_reg(PIC_HDD_RESET_START + idx, 1);
}
int pic_clear_reset_hd(int idx)
{
	PIC_CHECK_INIT();
	return __pic_write_reg(PIC_HDD_RESET_CLR_START + idx, 1);
}

/* 简易3U */
int pic_read_disk_3U(int disk_id, int *v)
{
	//TODO
}

int pic_write_disk_3U(int disk_id, int v)
{
	//TODO
}

int pic_init_3U(void)
{
	//TODO
}

/* 2U */
int pic_read_disk_2U(int disk_id, int *v)
{
	int ret;
	uint8_t reg1, reg2;
	int value;

	PIC_CHECK_INIT_2U();


	if (disk_id >= PIC_LED_NUMBER_2U)
		return PERR_INVAL;

	/* 判断使用GP1 还是GP2 */
	if (disk_id < PIC_LED_NUMBER_2U / 2) {
		reg1 = PIC_GP1_MODE1;
		reg2 = PIC_GP1_MODE2;
	}
	else {
		reg1 = PIC_GP2_MODE1;
		reg2 = PIC_GP2_MODE2;
	}
	value = i2c_smbus_read_byte_data(pic_fd, reg2);
	if (value == -1)
		return PERR_IOERR;
	value &= 0xf0;
	ret = i2c_smbus_read_byte_data(pic_fd, reg2);
	if (ret == -1)
		return PERR_IOERR;
	
	ret = i2c_smbus_read_byte_data(pic_fd, reg1);
	if (ret == -1)
		return PERR_IOERR;
	/* 获取单个灯的状态 */
	if ( ret & (1 << (disk_id & PIC_LED_DISK_MASK_2U)) )
		*v = 1;
	else 
		*v = 0;
	return 0;
}

int pic_write_disk_2U(int disk_id, int mode)
{
	int ret;
	uint8_t reg1, reg2;
	int value;

	PIC_CHECK_INIT_2U();
	
	
	if (disk_id >= PIC_LED_NUMBER_2U)
		return PERR_INVAL;
	if (disk_id < PIC_LED_NUMBER_2U / 2) {
		reg1 = PIC_GP1_MODE1;
		reg2 = PIC_GP1_MODE2;
	}
	else {
		reg1 = PIC_GP2_MODE1;
		reg2 = PIC_GP2_MODE2;
	}
	
	value = i2c_smbus_read_byte_data(pic_fd, reg2);
	if (value == -1)
		return PERR_IOERR;
	value &= 0xf0;
	ret = i2c_smbus_write_byte_data(pic_fd, reg2, value);
	if (ret == -1)
		return PERR_IOERR;
	
	value = i2c_smbus_read_byte_data(pic_fd, reg1-1);
	if (value == -1)
		return PERR_IOERR;
	if (mode)
		value = value | (1 << (disk_id & PIC_LED_DISK_MASK_2U));
	else 
		value = value & ~(1 << (disk_id & PIC_LED_DISK_MASK_2U));

	ret = i2c_smbus_write_byte_data(pic_fd ,reg1, value);
	if (ret == -1)
		return PERR_IOERR;
	return 0;
}

int pic_init_2U(void)
{
	int fd, ret;
	uint8_t g_config_reg_value;
	int value;
	
	if (pic_is_initialized)
		return 0;
	fd = open(I2C_DEV, O_RDWR);
	if (fd < 0)
		return PERR_NODEV;

	if (ioctl(fd, I2C_SLAVE_FORCE, PIC_ADDRESS_2U) < 0) {
		close(fd);
		return PERR_NODEV;
	}

	g_config_reg_value = i2c_smbus_read_byte_data(fd, PIC_CONF_2U);
	if (g_config_reg_value == 255) {
		return PERR_IOERR;
	}
	g_config_reg_value |= 0x80;
	ret = i2c_smbus_write_byte_data(fd, PIC_CONF_2U, g_config_reg_value);
	if (ret == -1)
		return PERR_IOERR;
	
	value = i2c_smbus_read_byte_data(fd, PIC_GP1_MODE2);
	if (value == -1)
		return PERR_IOERR;
	value &= 0xf0;
	ret = i2c_smbus_write_byte_data(fd, PIC_GP1_MODE2, value);
	if (ret == -1)
		return PERR_IOERR;
	value = i2c_smbus_read_byte_data(fd, PIC_GP2_MODE2);
	if (value == -1)
		return PERR_IOERR;
	value &= 0xf0;
	ret = i2c_smbus_write_byte_data(fd, PIC_GP2_MODE2, value);
	if (ret == -1)
		return PERR_IOERR;

	pic_is_initialized = 1;
	pic_fd = fd;
	return 0;
}

/* 系统灯 */
#define GPIO_BASE_ADDR (0x500)

#define GPIO_VALUE(fd, base, op, data)		\
	{					\
		lseek(fd, (base), SEEK_SET);	\
		op(fd, &data, sizeof(data));	\
	}

int sb_gpio28_set(int sw)
{
	uint32_t bit28, tmpval;
	int fd;

	fd = open("/dev/port", O_RDWR);
	if (fd)
	{
		bit28 = (1<<28);

		// Set GPIO28 to an output
		GPIO_VALUE(fd, GPIO_BASE_ADDR+4, read, tmpval);
		tmpval &= (~bit28);
		GPIO_VALUE(fd, GPIO_BASE_ADDR+4, write, tmpval);

		GPIO_VALUE(fd, GPIO_BASE_ADDR+0xc, read, tmpval);
		if (sw)
			tmpval |= bit28;
		else
			tmpval &= (~bit28);
		GPIO_VALUE(fd, GPIO_BASE_ADDR+0xc, write, tmpval);

		close(fd);
		return 0;
	}

	return -1;
}
