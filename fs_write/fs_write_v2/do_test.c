/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 16:12
 * Filename : do_test.c
 * Description : 
 * *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/statfs.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "main.h"
#include "parse_args.h"
#include "comm.h"
#include "mdigest.h"
#include "list.h"
#include "do_test.h"

/*返回dirname目录挂载的文件系统可用块数*/
long get_fs_space(char *path)
{
	struct statfs fsbuf;
	long i;


	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", path);
		return -1;
	}
	return fsbuf.f_bavail;	
}

/*返回dirname挂在的文件系统剩余空间百分比*/
int get_fs_stat(char *path) 
{	
	char buf[300];
	struct statfs fsbuf;

	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", path);
		return -1;
	}

	return (int)((fsbuf.f_bavail*100)/fsbuf.f_blocks);

}
/*返回剩余空间百分比最大的dirsname指针*/
struct dirsname *get_fs_dirs(struct dirsname *dirsp)
{
	char path[300];
	struct dirsname *tmp=NULL;
	struct dirsname *result=NULL;
	struct statfs fsbuf;
	long i = 0, j;
	tmp = dirsp->next;
	while (tmp != dirsp) {
		sprintf(path, "%s/%s", root_dir, tmp->name);
		if ((j = get_fs_stat(path)) > i) {
			i = j;
			result = tmp;
		}	
		tmp = tmp->next;		
	}
	return result;
}

int get_random(void)
{
	srand((int)time(0));

	return rand();
}

void *_w_thread(void *arg)
{	
	int block_size = ((struct thread_args *)arg)->block_size;
	int buf[block_size/4+1];
	int i, j, count, tnum;
	char filename[300]; /*线程正在写的文件的 完整路径*/
	FILE *fp;
	int begin;
	char dirbuf[400];  /*写好的文件要rename的完整路径*/  
	char namebuf[200];	
	unsigned char md5[16]; /*要改成的名字*/
	unsigned char newname[33], md5str[33];
	md_context context; 
	struct dirsname *tmp=NULL;
	struct dirsname *t_dirsp;

	t_dirsp = (struct dirsname *)((struct thread_args *)arg)->dp;
	tnum =(int) ((struct thread_args *)arg)->thread_num;
	count =((struct thread_args *)arg)->file_size / block_size + 1;
	while (1) {
		tmp = get_fs_dirs(t_dirsp);
		if (tmp == NULL) {
			fprintf(stderr, "Can't get_fs_dirs!\n");
			return (void *)-1;
		}
		sprintf(dirbuf, "%s/%s", root_dir, tmp->name);
		sprintf(filename, "%s/tmp.%d", dirbuf, tnum);
		if (chdir(dirbuf) < 0) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't change dir to %s!\n",tnum, dirbuf);
			fprintf(stderr, "thread %d can't into dir: %s\n", tnum, dirbuf);
			pthread_exit((void *)-1);
		}
		if ((fp=fopen(filename,"w")) == NULL) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't open %s to write!\n",tnum,filename);
			fprintf(stderr, "_w_thread can't open %s!\n", filename);
			pthread_exit((void *)-1);
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

		/*得到要存放的路径名*/
		if (get_date(namebuf)) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't get the time!\n",tnum);
			fprintf(stderr, "Can't get the date!\n");
			pthread_exit((void *)-1);
		}
		sprintf(dirbuf, "%s/%s", dirbuf, namebuf);
		if (access(dirbuf, F_OK) < 0) {
			if (mkdir(dirbuf ,FILE_MOD) < 0) {
				/*	fprintf(stderr, "make date dir %s error!\n", namebuf);
					pthread_exit((void *)-1); */
			}
		}
		if (get_hour(namebuf)) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't get the time!\n",tnum);
			fprintf(stderr, "Can't get the hour!\n");
			pthread_exit((void *)-1);
		}
		sprintf(dirbuf, "%s/%s/", dirbuf, namebuf);
		if (access(dirbuf, F_OK) < 0) {
			if (mkdir(dirbuf, FILE_MOD) < 0) {
				/*	fprintf(stderr, "Can't make hour dir!\n");
					pthread_exit((void *)-1); */
			}
		}
		strcat(dirbuf, newname);

		/*rename成新文件*/
		if (rename(filename, dirbuf) < 0) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d rename %s error!\n",tnum, dirbuf);
			fprintf(stderr, "rename %s error!\n", filename);
			pthread_exit((void *)-1);
		}

		/*重新读取文件并计算md5*/
		md5_begin(&context);
		if ((fp = fopen(dirbuf, "r")) == NULL) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't read %s!\n",tnum, dirbuf);
			fprintf(stderr, "Can't open %s!\n", dirbuf);
			return (void *)-1;
		}	
		while (i=(fread(buf, sizeof(buf), 1, fp))) {
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
			return (void *)-1;
		}  
	}
}
/*创建写线程*/
int w_thread (struct dirsname *dirsp, long file_size, int block_size, int thread_n, int time_s)
{
	int tid;
	int err;
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
		if (pthread_create(&threads[tid], NULL, &_w_thread, (void *)t_args[tid]) != 0) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "create thread %d error!\n", tid);
			fprintf(stderr, "threads create error!\n");
			return -1;
		}
	}
	return 0;

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
int find_delete(char *path, char *buf)
{
	DIR *dirp;
	struct dirent *dp;
	char tmp[100]="9999999999";

	//strcpy(tmp,"999999999\0");
	if ((dirp = opendir(path)) == NULL) {
		fprintf(stderr, "Can't open the dir %s\n", path);
		closedir(dirp);
		return -1;
	}
	while (dp = readdir(dirp)) {
		if (dp->d_type == DT_DIR){
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				if (strcmp(dp->d_name, tmp) < 0)
					strcpy(tmp, dp->d_name);
			}
		}
	}
	closedir(dirp);
	strcpy(buf, tmp);
	return 0;
}
int _dfile(char *path)
{

	int n=0;
	DIR *dirp;
	struct dirent *dp;
	struct stat statbuf;
	char datebuf[200];
	char hourbuf[200];
	char thefile[300];
	time_t nowtime;
	char cmd[200];
	/*找到最早的date目录*/
	if (find_delete(path, datebuf) < 0) {
		fprintf(stderr, "Can't find date file to delete!\n");
		return -1;
	}
	//	printf("the date file  is %s\n", datebuf);
	/*找到最早的hour目录*/
	time(&nowtime);
	if ((dirp = opendir(datebuf)) == NULL) {
		fprintf(stderr, "Can't open dir %s\n", datebuf);
		return -1;
	}
	while (dp = readdir(dirp)) {
		/*找到date目录下的hour目录*/
		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				n++;
				sprintf(hourbuf, "%s/%s", datebuf, dp->d_name);
				/*得到目录的创建时间*/
				if (stat(hourbuf, &statbuf) < 0) {
					fprintf(stderr, "Can't stat the dir %s\n", hourbuf);
					return -1;
				}
				/*选择最早的目录*/
				if (statbuf.st_atime < nowtime) {
					nowtime=statbuf.st_atime;
					strcpy(thefile, hourbuf);
				}

			}
		}
	}

	/*删除最早的hour目录*/
	if (n > 0) {                                                                                                             
		sprintf(cmd, "rm -rf %s", thefile);
		if (system(cmd) < 0) {
			fprintf(stderr, "Can't delete the file!/n", thefile);
			return -1;
		} else {
			n--;
		}
	}

	/*删除空date目录*/
	if (n == 0) {
		sprintf(cmd, "rm -rf %s", datebuf);
		if (system(cmd) < 0) {
			fprintf(stderr, "_dfile can't delete %s\n", datebuf);
			return -1;
		}
	}
	sync();
	return 0;

}
void release_percent(struct dirsname *dirsp)
{
	struct dirsname *tmp;
	char path[300];
	tmp = dirsp->next;
	while ( tmp != dirsp ) {
		sprintf(path, "%s/%s", root_dir, tmp->name);
		while( get_fs_stat(path) < END_RELEASE ) {
			if (_dfile(path) < 0) { 
				openlog("fs_write", LOG_CONS|LOG_PID, 0);
				syslog(LOG_USER|LOG_ERR, "_dfile %s error!\n",path);
				fprintf(stderr, "dfile %s error!\n", path);
			}
		}
	}
}
/*如果文件系统剩余空间不足则删除文件*/
int dfile(struct dirsname *dirsp, long file_size, int thread_n)
{
	char path[300];
	char buf[10];
	struct dirsname *tmp;


	while (1) {

		if (update_list( dirsp, file_size, thread_n) < 0){
			fprintf(stderr, "update_list error!\n");
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "update_list  error!\n",tmp->name);
		}
		tmp = dirsp->next;
		while (tmp != dirsp) {
			sprintf(path, "%s/%s", root_dir, tmp->name);
			/*进入指定目录*/
			if (chdir(path) < 0) {
				fprintf(stderr, "dfile Can't into %s", tmp->name);
				return -1;
			}
			/*可用空间少于20%*/
			if (get_fs_stat(path) < START_RELEASE) {
				release_percent(dirsp);
			}
			tmp = tmp->next;
		}
		sleep(10);
	}
	return 0;
}
