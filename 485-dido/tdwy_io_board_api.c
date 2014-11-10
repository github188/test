#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <rpc/des_crypt.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <time.h>
#include "tdwy_io_board_api.h"




extern char gCommPort[16];
#define COMM_TIMEOUT							2

#define LIB_NULL_FUNCTION						0
#define LIB_IO_BOARD_API_DEBUG_EN				0

#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )
#undef COMM_TIMEOUT
#define COMM_TIMEOUT							10
#endif

#define UART_RAW_PKT_DATA_LENGTH			32
#define UART_R_QUEUE_LENGTH					10

#define READ_REG_VALUE							0x80
#define READ_REG_REPLY							0x81
#define WRITE_REG_VALUE						0x82
#define WRITE_REG_REPLY						0x83

/*
 * Comm Protocol
 * @ cmd used to specify a read or a write operation, READ_REG_VALUE or WRITE_REG_VALUE
 * @ reg used to specify the register offset define above
 * @ data specify the value which was read from or write into the register specified by reg
*/
typedef struct __comm_msg_header
{
	uint8_t cmd;
	uint8_t reg;
	uint8_t data;
}comm_msg_header;



int io_board_api_init ( void )
{
	int fd;
	struct termios newtio;
	struct flock comm_lock;

	fd = open ( gCommPort, O_RDWR | O_NOCTTY | O_NDELAY );
	if ( fd < 0 )
	{
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )
		printf ( "Open %s failed.\n", gCommPort );
#endif
		return -1;
	}

	/* Try to get the lock status of the comm port */
	comm_lock.l_type = F_WRLCK | F_RDLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;
	if ( fcntl ( fd, F_GETLK, &comm_lock ) == -1 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Get lock failed.\n" );
#endif
		close ( fd );
		return -1;
	} else {
		if ( comm_lock.l_type != F_UNLCK ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )			
			printf ( "PID %ld is using this comm.\n", (long)comm_lock.l_pid );
#endif
			close ( fd );
			return -1;
		}
	}

	/* Set lock on comm port */
	comm_lock.l_type = F_WRLCK | F_RDLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;

	if ( fcntl ( fd, F_SETLK, &comm_lock ) != 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Set lock on comm port failed.\n" );
#endif
		close ( fd );
		return -1;
	}

	
	bzero ( &newtio, sizeof( newtio ));

	newtio.c_cflag |= CLOCAL|CREAD;

	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;

	newtio.c_cflag &= ~PARENB;

	cfsetispeed ( &newtio, B115200 );
	cfsetospeed ( &newtio, B115200 );

	newtio.c_cflag &= ~CSTOPB;

	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	
	tcflush ( fd, TCIFLUSH );

	if ( tcsetattr ( fd, TCSANOW, &newtio ) != 0 )
	{
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )	
		perror ( "com set error" );
#endif
		return -1;
	}

	/* Unlock the comm port */
	comm_lock.l_type = F_UNLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;

	if ( fcntl ( fd, F_SETLK, &comm_lock ) != 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Set lock on comm port failed.\n" );
#endif
		close ( fd );
		return -1;
	}

	close ( fd );

	return 0;
}

#define START_BYTE								0xA0
#define STOP_BYTE								0xA5
#define HAND_SHAKE_BYTE						0xA6
#define ESC_BYTE								0xAA
#define ASCII_ESC_BYTE							0x1B
									
#define START_ENCODE_BYTE	   					0xB0
#define STOP_ENCODE_BYTE						0xB5
#define HANDSHAKE_ENCODE_BYTE				0xB6
#define ESC_ENCODE_BYTE						0xBA
#define ASCII_ESC_ENCODE_BYTE					0x3B

#define PREV_BYTE_ESC							0x01
#define PREV_BYTE_NOTESC						0x02


/*
 * Encode the data which will be sent to the mcu
 * @data_in points to the buffer store he data which will be sent to the mcut 
 * @data_in_len specify the length of the data
 * @data_out points to the buffer which will store the data after encode
 *
 * return the length of the data after encode, 0 means error.
*/
static int32_t uart_data_encode ( uint8_t *data_in, uint8_t data_in_len, uint8_t *data_out )
{
	int32_t send_len = 0;
	int i;
	
	if (( data_in == NULL ) || ( data_out == NULL ) || ( data_in_len == 0 )) {
		return -1;
	}

	data_out[send_len++] = START_BYTE;
	for ( i = 0; i < data_in_len; i++ )
	{
		switch ( *( data_in + i ))
		{
			case START_BYTE:
			case STOP_BYTE:
			case HAND_SHAKE_BYTE:
			case ESC_BYTE:
				data_out[send_len++]  = ESC_BYTE;
				data_out[send_len++]  = *( data_in + i ) + 0x10;
				break;
			case ASCII_ESC_BYTE:
				data_out[send_len++] = ESC_BYTE;
				data_out[send_len++] = *( data_in + i ) + 0x20;
				break;
			default:
				data_out[send_len++] = *( data_in + i );
				break;
		}
	}
	data_out[send_len++]  = STOP_BYTE;

	return send_len;	
}

/*
 * Decode the data which recv from host
 * @data_in points to the buffer store he data which recv from the host 
 * @data_in_len specify the length of the data
 * @data_out points to the buffer which will store the data after decode
 *
 * return the length of the data after decode, 0 means error.
*/
static int32_t uart_data_decode ( uint8_t * data_in, uint8_t data_in_len, uint8_t *data_out )
{
	int i;
	int32_t uart_recv_pos = 0;
	uint8_t prev_byte_status = PREV_BYTE_NOTESC;
	
	if (( data_in == NULL ) || ( data_out == NULL ) || ( data_in_len == 0 )) {
		return -1;
	}

	for  ( i = 0; i < UART_RAW_PKT_DATA_LENGTH; i++ ) {
		switch ( data_in[i] ) {
			case START_BYTE:
				uart_recv_pos = 0;
				prev_byte_status = PREV_BYTE_NOTESC;
				break;
			case STOP_BYTE:
				/* All data has been processed. */
				return uart_recv_pos;
			case ESC_BYTE:
				prev_byte_status = PREV_BYTE_ESC;
				break;
			case START_ENCODE_BYTE:
			case STOP_ENCODE_BYTE:
			case HANDSHAKE_ENCODE_BYTE:
			case ESC_ENCODE_BYTE:
				if ( prev_byte_status == PREV_BYTE_ESC )
				{
					data_out[uart_recv_pos] = data_in[i] - 0x10;
					prev_byte_status = PREV_BYTE_NOTESC;
				}
				else
				{
					data_out[uart_recv_pos] = data_in[i];
				}
				uart_recv_pos++;
				break;
			case ASCII_ESC_ENCODE_BYTE:
				if ( prev_byte_status == PREV_BYTE_ESC )
				{
					data_out[uart_recv_pos] = data_in[i] - 0x20;
					prev_byte_status = PREV_BYTE_NOTESC;
				}
				else
				{
					data_out[uart_recv_pos] = data_in[i];
				}
				uart_recv_pos++;
				break;
			case ASCII_ESC_BYTE:
				uart_recv_pos = 0;
				return 0;
			default:
				data_out[uart_recv_pos] = data_in[i];
				uart_recv_pos++;
				break;	
		}
	}	

	return uart_recv_pos;
}

static int uart_msg_check ( uint8_t *data_buffer, uint8_t data_len, uint8_t *check_buffer )
{
	uint8_t i;
	uint8_t start_pos = 0, end_pos = 0;

	if ( data_buffer == NULL ) {
		return -1;
	}

	for ( i = 0; i < data_len; i++ ) {
		if ( data_buffer[i] == START_BYTE ) {
			start_pos = i;
		} else if ( data_buffer[i] == STOP_BYTE ) {
			end_pos = i;
		}
	}

	if (( end_pos == 0 ) || (( end_pos - start_pos ) != 4 )) {
		return -1;
	} 

	for ( i = start_pos; i <= end_pos; i++ ) {
		check_buffer[i] = data_buffer[i+start_pos];
	}

	return ( end_pos - start_pos + 1 );
}


static int io_board_xfer_msg_to_mcu ( comm_msg_header *pXferMsg )
{
	int fd;
	int ret = 0, io_len;
	struct flock comm_lock;
	uint8_t io_buf[32];
    uint8_t check_buf[32];
	fd = open ( gCommPort, O_RDWR | O_NOCTTY | O_NDELAY );
    if ( fd < 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Open %s failed.\n", gCommPort );
#endif
		return -1;
	}

	/* Try to get the lock status of the comm port */
	comm_lock.l_type = F_WRLCK | F_RDLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;
	if ( fcntl ( fd, F_GETLK, &comm_lock ) == -1 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Get lock failed.\n" );
#endif
		close ( fd );
		return -1;
	} else {
		if ( comm_lock.l_type != F_UNLCK ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )			
			printf ( "PID %ld is using this comm.\n", (long)comm_lock.l_pid );
#endif
			close ( fd );
			return -1;
		}
	}

	/* Set lock on comm port */
	comm_lock.l_type = F_WRLCK | F_RDLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;

	if ( fcntl ( fd, F_SETLK, &comm_lock ) != 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Set lock on comm port failed.\n" );
#endif
		close ( fd );
		return -1;
	}

	/* Send command to mcu via uart */
	io_len = uart_data_encode (( uint8_t * )pXferMsg, sizeof( comm_msg_header ), io_buf );

	if ( io_len <= 0 ) {
		ret = -1;
		goto unlock_comm;
	}

	ret = write ( fd, io_buf, io_len );
	if ( ret < io_len ) {
		ret = -1;
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Send msg to mcu failed.\n" );
#endif
		goto unlock_comm;
	}

	memset ( io_buf, 0, 32 );
	if ( pXferMsg->cmd == READ_REG_VALUE ) {
		fd_set rd;
		struct timeval timeout;
		FD_ZERO ( &rd );

		timeout.tv_sec = COMM_TIMEOUT;
		timeout.tv_usec = 0;

		FD_SET ( fd, &rd );
		ret = select ( fd + 1, &rd, NULL, NULL, &timeout );
		if ( ret > 0 ) {
			if ( FD_ISSET ( fd, &rd )) {
				ret = read ( fd, io_buf, 32 );
				if ( ret < 5 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )					
					printf ( "Recv data has error, too few bytes.\n" );
#endif
					goto unlock_comm;
				} 

				/* Check data */
				io_len = uart_msg_check ( io_buf, ret, check_buf );
				if ( 0 > io_len) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )					
					printf ( "Recv data has error.\n" );
#endif
					ret = -1;
					goto unlock_comm;
				}

				ret = uart_data_decode ( check_buf, io_len, ( uint8_t * )pXferMsg );

				if ( ret < 0  ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )					
					printf ( "decode uart msg error.\n" );
#endif
					goto unlock_comm;
				}
				
			}
		} else if ( ret == 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
			printf ( "Timeout.\n" );
#endif
			ret = -1;
			goto unlock_comm;
		} else {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
			printf ( "Io error.\n" );
#endif
			ret = -1;
		}
	}


unlock_comm:
	
	/* Unlock the comm port */
	comm_lock.l_type = F_UNLCK;
	comm_lock.l_whence = SEEK_SET;
	comm_lock.l_start = 0;
	comm_lock.l_len = 0;

	if ( fcntl ( fd, F_SETLK, &comm_lock ) != 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Set lock on comm port failed.\n" );
#endif
		close ( fd );
		return -1;
	}
	
	close ( fd );	

	return ret;
}

int io_board_write_byte_data ( uint8_t reg, uint8_t value )
{
	int ret = 0;
#if ( LIB_NULL_FUNCTION == 0 )
	comm_msg_header send_msg;
	send_msg.cmd = WRITE_REG_VALUE;
	send_msg.reg = reg;
	send_msg.data = value;

	ret = io_board_xfer_msg_to_mcu ( &send_msg );

	return (( ret < 0 ) ? ret : 0); 
#else
	reg = reg;
	value = value;
	return ret;
#endif
}

int io_board_read_byte_data ( uint8_t reg, uint8_t *value )
{
	int ret = 0;
#if ( LIB_NULL_FUNCTION == 0 )
	
	comm_msg_header io_msg;
	io_msg.cmd = READ_REG_VALUE;
	io_msg.reg = reg;
	io_msg.data = *value;

	ret = io_board_xfer_msg_to_mcu ( &io_msg );
	if ( ret < 0 ) {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )		
		printf ( "Recv data from mcu failed.\n" );
#endif
		return ret;
	} else {
#if ( LIB_IO_BOARD_API_DEBUG_EN == 1 )
		printf ( "Cmd:%02x; Reg:%02x;Value:%02x.\n", io_msg.cmd, io_msg.reg, io_msg.data );
#endif
		*value = io_msg.data;
		return 0;
	}
#else
	reg = reg;
	*value = 0x00;
	return ret;
#endif
}

