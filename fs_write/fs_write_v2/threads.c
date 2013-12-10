/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-12-06 09:12
 * Filename : w_thread.c
 * Description : 创建写线程
 * *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "comm.h"
#include "mdigest.h"
#include "list.h"
#include "moniter.h"
#include "parse_args.h"
#include "threads.h"


pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;	

int get_random(void)
{
	srand((int)time(0));

	return rand();
}
int get_date(char *buf)
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo=localtime(&rawtime);
	strftime(buf,10, "%Y%m%d", timeinfo);

	return 0;
}
int get_hour(char  *buf)
{
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo=localtime(&rawtime);
	if (timeinfo->tm_min < 30) {
		strftime(buf, 10, "%H_0", timeinfo);
	} else {
		strftime(buf, 10, "%H_1", timeinfo);
	}
	return 0;
}
int get_newpath(char *newpath, int tnum)
{
	char namebuf[200];
	/*得到要存放的路径名*/
	if (get_date(namebuf)) {
		return -1;
	}
	sprintf(newpath, "%s/%s", newpath, namebuf);
	if (access(newpath, F_OK) < 0) {
		if (mkdir(newpath ,FILE_MOD) < 0) {
			return -1;
		}
	}
	if (get_hour(namebuf)) {
		return -1;
	}
	sprintf(newpath, "%s/%s/", newpath, namebuf);
	if (access(newpath, F_OK) < 0) {
		if (mkdir(newpath, FILE_MOD) < 0) {
			return -1;
		}
	}
	return 0;
}
int write_file(int tnum, int block_size, int count, struct dirsname *tmp)
{  
	int buf[block_size/4];
	int i, j;
	char tmppath[300]; /*线程正在写的文件的 完整路径*/
	FILE *fp;
	int begin, ret;
	char newpath[400];  /*写好的文件要rename的完整路径*/  
	char namebuf[300];	
	unsigned char md5[16]; /*要改成的名字*/
	unsigned char newname[33], md5str[33];
	md_context context; 

	if (tmp == NULL) {
		fprintf(stderr, "Can't get_fs_dirs!\n");
		return -1;
	}
	sprintf(tmppath, "%s/%s", root_dir, tmp->name);
	if (chdir(tmppath) < 0) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "thread %d can't change dir to %s!\n",tnum, tmppath);
		fprintf(stderr, "thread %d can't into dir: %s\n", tnum, tmppath);
		return -1;
	}
	sprintf(tmppath, "%s/tmp.%d", tmppath, tnum);
	if ((fp=fopen(tmppath,"w")) == NULL) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "thread %d can't open %s to write!\n",tnum,tmppath);
		fprintf(stderr, "_w_thread can't open %s!\n", tmppath);
		return -1; 
	}
	md5_begin(&context);
	/*count为写够FILE_SIZE所需要的次数*/
	for(i=0; i < count; i++) {
		begin = get_random();
		for (j=0; j<block_size/4; j++) {
			buf[j] = begin+j;
		}
		fwrite(buf, sizeof(buf), 1,  fp);
		md5_update(&context, (char *)buf, block_size);
	}
	fflush(fp);
	fsync(fileno(fp));
	fclose(fp);
	md5_result(&context, md5);

	/*将md5转换成文件名*/	
	sprintf(newname, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\0",
			md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],
			md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],
			md5[14],md5[15]);



	do{ 
		sprintf(newpath, "%s/%s", root_dir, tmp->name);
		ret = get_newpath(newpath, tnum);
	}while(ret < 0);

	strcat(newpath, newname);
	/*rename成新文件*/
	if (rename(tmppath, newpath) < 0) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "thread %d rename %s error!\n",tnum, tmppath);
		fprintf(stderr, "rename %s error!\n", tmppath);
		return -1;
	}

	/*重新读取文件并计算md5*/
	md5_begin(&context);
	if ((fp = fopen(newpath, "r")) == NULL) {
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "thread %d can't read %s!\n",tnum, newpath);
		fprintf(stderr, "Can't open %s!\n", newpath);
		return -1;
	}	
	while ((fread(buf, sizeof(buf),(size_t)1, fp))) {
		md5_update(&context, (char *)buf, block_size);
	}
	fclose(fp);
	md5_result(&context, md5);
	/*将md5转换成字符串*/
	sprintf(md5str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\0",
			md5[0],md5[1],md5[2],md5[3],md5[4],md5[5],md5[6],
			md5[7],md5[8],md5[9],md5[10],md5[11],md5[12],md5[13],
			md5[14],md5[15]);

	/*与文件名做比较*/
	if (strncmp(md5str ,newname, 32) != 0){
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "thread %d's md5 check error!\n",tnum);
		fprintf(stderr, "thread %d's md5 check error!\n", tnum);
		return -1;
	}  

	return 0;

}
/*根据不同的策略 返回不同的dirsname指针*/
struct dirsname *choose_policy(struct dirsname *dirsp,struct dirsname *bef, int thread_n, long file_size,int policy)
{
	struct dirsname *tmp = NULL;	
	switch (policy) {
		case 0:
			tmp = get_fs_dirs_default(dirsp, bef, thread_n, file_size);
			break;
		case 1:
			tmp = get_fs_dirs_by_weight(dirsp, thread_n, file_size);
			break;
		case 2:
			tmp = get_fs_dirs_by_size(dirsp, thread_n, file_size);
			break;
		case 3:
			tmp = get_fs_dirs_by_percent(dirsp, thread_n, file_size);
			break;
		default:
			break;
	}
	return tmp;
}
void *w_thread(void *arg)
{	
	int block_size = ((struct thread_args *)arg)->block_size;
	int thread_n, count, tnum, time_s, start, end, time;
	long file_size;
	int policy;
	struct dirsname *tmp=NULL;
	struct dirsname *tmp_bef;
	struct dirsname *t_dirsp;

	t_dirsp = (struct dirsname *)((struct thread_args *)arg)->dp;
	tnum =(int) ((struct thread_args *)arg)->thread_num;
	count =((struct thread_args *)arg)->file_size / block_size;
	time_s =(int)((struct thread_args *)arg)->time_s;
	file_size =(long)((struct thread_args *)arg)->file_size;
	thread_n = (int)((struct thread_args *)arg)->thread_n;
	policy = (int)((struct thread_args *)arg)->policy;
	time = 0;
	tmp_bef=t_dirsp;
	if (time_s) {
		tmp = choose_policy(t_dirsp,tmp_bef, thread_n, file_size,policy);
	} else {
		tmp = t_dirsp->next;
	}
	while (1) {
		pthread_mutex_lock(&mutex);
		if (update_list(t_dirsp, file_size, thread_n) < 0){
			fprintf(stderr, "update_list error!\n");
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "update_list %s error!\n",tmp->name);
		}
		pthread_mutex_unlock(&mutex);
		if (time_s){
			if (time >= time_s){
				time = 0;
				tmp = choose_policy(t_dirsp,tmp_bef, thread_n, file_size,policy);
			}
			start = clock();
		} else {

			tmp = choose_policy(t_dirsp,tmp_bef, thread_n, file_size, policy);
		}
		if ( write_file( tnum,block_size, count, tmp) < 0){
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d's error and exitting!\n",tnum);
			fprintf(stderr, "thread %d's error and exitting!\n", tnum);
			pthread_exit((void *)-1);
		}
		tmp_bef = tmp;
		if (time_s != 0) {
			end = clock();
			time += (end-start)/CLOCKS_PER_SEC;
		}
		if (policy == 1) {
			pthread_mutex_lock(&mutex);
			tmp->weight++;
			pthread_mutex_unlock(&mutex);
		}
	}
}

/*创建写线程*/
int start_w_thread (struct dirsname *dirsp, long file_size, int block_size, int thread_n, int time_s, int policy, pthread_t *pids)
{
	int tid;
	struct thread_args *t_args[thread_n];  /*线程的参数结构指针*/
	printf("thread_n: %d\nfile_size:%ldM\nblock_size:%d\ntime_s:%d\n",
			thread_n, file_size/1024/1024, block_size, time_s);

	for (tid=0; tid<thread_n; tid++) {
		t_args[tid] = (struct thread_args *)malloc(sizeof(struct thread_args));
		if (t_args[tid] == NULL) {
			fprintf(stderr, "thread %d malloc error!\n", tid);
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d malloc error!\n", tid);
			return -1;
		}
		t_args[tid]->file_size = file_size;
		t_args[tid]->block_size = block_size;
		t_args[tid]->thread_num  = tid;
		t_args[tid]->time_s = time_s;
		t_args[tid]->dp = dirsp;
		t_args[tid]->thread_n = thread_n;
		t_args[tid]->policy = policy;
		if (pthread_create(pids+tid, NULL, &w_thread, (void *)t_args[tid]) != 0) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "create write thread %d error!\n", tid);
			fprintf(stderr, "create write thread %d error!\n", tid);
			return -1;
		}
	}
	return 0;
}

void *d_thread(void *arg)
{
	char path[300];
	strcpy(path, (char *)arg);
	if (release_percent(path) < 0) {
		fprintf(stderr, "delete thread error!\n");
		pthread_exit((void *)-1);
	}
	pthread_exit((void *)0);

}
pthread_t start_d_thread(char *path, pthread_t *flag)
{

	if (pthread_create(flag, NULL, &d_thread, (void *)path) != 0){
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "create delete thread error!\n ");
		fprintf(stderr, "delete thread create error!\n");
		return -1;
	}
	return 0;
}

