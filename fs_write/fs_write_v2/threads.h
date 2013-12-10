#ifndef __W_THREAD
#define __W_THREAD
#define FILE_MOD S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
/*pthread_t threads[THREAD_MAX];*/

/*写线程的参数*/
struct thread_args{
	long file_size;
	int block_size;
	int thread_num;
	int time_s;
	int thread_n;
	int policy;
	struct dirsname *dp;
};

/*删除线程的参数*/
struct d_args{
	struct dirsname *dp;
	int *flag;
};

/*根据参数创建写线程 thread_n 写线程个数 file_size文件大小 block_size文件block_size大小 
 * time_s为0不切换 不为0的时候写time_s秒然后切换目录*/ 
int start_w_thread(struct dirsname *, long file_size, int block_size, int thread_n, int time_s, int policy, pthread_t *pids);

/*启动删除线程  完成删除后要将flag置为1*/
pthread_t start_d_thread(char *, pthread_t *);
#endif
