/* The include file for both the MD4 and MD5 routines. */
#ifndef _MDIGEST_H_
#define _MDIGEST_H_

#include <linux/types.h>
#define MD4_DIGEST_LEN 16
#define MD5_DIGEST_LEN 16
#define MAX_DIGEST_LEN MD5_DIGEST_LEN

typedef unsigned int u32;
typedef unsigned char u8;

#define CSUM_CHUNK 64

typedef struct {
	u32 A, B, C, D;
	u32 totalN;          /* bit count, lower 32 bits */
	u32 totalN2;         /* bit count, upper 32 bits */
	u8 buffer[CSUM_CHUNK];
} md_context;

void md5_begin(md_context *ctx);
void md5_update(md_context *ctx, const u8 *input, u32 length);
void md5_result(md_context *ctx, u8 digest[MD5_DIGEST_LEN]);

void get_md5(u8 digest[MD5_DIGEST_LEN], const u8 *input, int n);

#endif 

