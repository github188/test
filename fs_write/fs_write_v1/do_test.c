/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 16:12
 * Filename : do_test.c
 * Description : 
 * *****************************************************************************/
#include <stdio.h>
#include "do_test.h"

int get_random(void)
{
	srand((int)time(0));

	return rand();
}
void * _w_thread(void *arg)
{ 
	int buf[BLOCK_SIZE/4];
	int i, j, count, tnum;
	char filename[300]; /*线程正在写的文件的 完整路径*/
	FILE *fp;
	int begin;
	char dirbuf[400];  /*写好的文件要rename的完整路径*/  
	char namebuf[200];	
	unsigned char md5[16]; /*要改成的名字*/
	unsigned char newname[33], md5str[33];
	md_context context; 

	tnum =(int) ((struct thread_args *)arg)->thread_num;
	count = file_size/BLOCK_SIZE;
	while(1) {

		strcpy(dirbuf, _dirname);

		strcpy(filename, dirbuf);
		strcat(filename, "/");
		sprintf(namebuf, "tmp.%d", tnum);
		strcat(filename, namebuf);

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
			for (j=0; j<BLOCK_SIZE/4; j++) {
				buf[j] = begin+j;
			}
			fwrite(buf, sizeof(buf), 1,  fp);
			md5_update(&context, (char *)buf, BLOCK_SIZE);
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
		/*得到新的文件名字*/
		/*get_time(namebuf);
		  strcpy(newname, namebuf);
		  sprintf(namebuf, ".%d", tnum);
		  strcat(newname, namebuf);
		  printf("newname : %s\n", newname);
		  */

		/*得到要存放的路径名*/
		if (get_date(namebuf)) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d can't get the time!\n",tnum);
			fprintf(stderr, "Can't get the date!\n");
			pthread_exit((void *)-1);
		}

		strcat(dirbuf, "/");
		strcat(dirbuf, namebuf);
		strcat(dirbuf, "/");
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
		strcat(dirbuf, namebuf);
		strcat(dirbuf, "/");
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
			md5_update(&context, (char *)buf, BLOCK_SIZE);
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
			fprintf(stderr, "MD5 error!\n");
			return (void *)-1;
		}  
	}
}
/*创建写线程*/
int w_thread (void)
{
	int tid;
	int err;
	struct thread_args *t_args[thread_n];  /*线程的参数结构指针*/
	printf("thread_n: %d\nfile_size:%ldM\ntime_s:%ds\n", thread_n, file_size/1024/1024, time_s);

	for (tid=0; tid<thread_n; tid++) {
		t_args[tid] = (struct thread_args *)malloc(sizeof(struct thread_args));
		if (t_args == NULL) {
			fprintf(stderr, "thread %d malloc error!\n", tid);
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "thread %d malloc error!\n", tid);
			return -1;
		}
		t_args[tid]->thread_num  = tid;
		//strcpy(t_args[i].dir, _dirname);
		if (pthread_create(&threads[tid], NULL, &_w_thread, (void *)t_args[tid]) != 0) {
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "create thread %d error!\n", tid);
			fprintf(stderr, "threads create error!\n");
			return -1;
		}
	}
	return 0;

}
/*
   int get_time(char *buf){
   time_t rawtime;
   struct tm *timeinfo;

   time(&rawtime);
   timeinfo=localtime(&rawtime);
   strftime(buf, 20, "%H%M%S", timeinfo);

   return 0;

   }
   */
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
int find_delete(char *dir, char *buf)
{
	DIR *dirp;
	struct dirent *dp;
	char tmp[100]="9999999999";

	//strcpy(tmp,"999999999\0");
	if ((dirp = opendir(dir)) == NULL) {
		fprintf(stderr, "Can't open the dir %s\n", dir);
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
int _dfile(char *dir)
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
	if (find_delete(dir, datebuf) < 0) {
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
				strcpy(hourbuf, datebuf);
				strcat(hourbuf, "/");
				strcat(hourbuf, dp->d_name);
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
		} else {
			n--;
		}
	}

	/*删除空date目录*/
	if (n == 0) {
		sprintf(cmd, "rm -rf %s", datebuf);
		if (system(cmd) < 0) {
			fprintf(stderr, "_dfile can't delete %s\n", datebuf);
		}
	}
	sync();
	return 0;

}
/*如果文件系统剩余空间不足则删除文件*/
int dfile(void)
{
	char buf[10];
	struct statfs fsbuf;
	char dirname[400];


	while (1) {

		strcpy(dirname, _dirname);
		/*进入指定目录*/
		if (chdir(dirname) < 0) {
			fprintf(stderr, "dfile Can't into %s", _dirname);
			return -1;
		}
		if (statfs(dirname, &fsbuf) < 0) {
			fprintf(stderr, "Can't get %s's statfs!\n", _dirname);
			return -1;
		}
		/*可用空间少于20%*/
		if ((fsbuf.f_bfree*10)/fsbuf.f_blocks <= 2) {
			/*直到可用空间大于40%*/
			while((fsbuf.f_bfree*10)/fsbuf.f_blocks <= 4) {
				if (_dfile(dirname) < 0) { 
					openlog("fs_write", LOG_CONS|LOG_PID, 0);
					syslog(LOG_USER|LOG_ERR, "_dfile %s error!\n",dirname);
					fprintf(stderr, "Release space error!\n");
					return -1;
				}

				if (statfs(dirname, &fsbuf) < 0) {
					openlog("fs_write", LOG_CONS|LOG_PID, 0);
					syslog(LOG_USER|LOG_ERR, "get %s's statfs error!\n",dirname);
					fprintf(stderr, "Can't get %s's statfs!\n", _dirname);
					return -1;
				}
			}
		}
		else {
			sleep(10);
		}
	}
	return 0;

}
