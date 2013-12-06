#ifndef __W_THREAD
#define __W_THREAD
#define FILE_MOD S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
/*根据参数创建写线程 thread_n 写线程个数 file_size文件大小 block_size文件block_size大小 
 * time_s为0不切换 不为0的时候写time_s秒然后切换目录*/ 
int start_w_thread(struct dirsname *, long file_size, int block_size, int thread_n, int time_s);
#endif
