/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 16:12
 * Filename : do_test.c
 * Description : 监视文件系统挂载情况，并根据不同策略进行删除 
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
#include "parse_args.h"
#include "comm.h"
#include "list.h"
#include "moniter.h"

/*返回dirname目录挂载的文件系统可用空间*/
long get_fs_space(char *path)
{
	struct statfs fsbuf;
	long i;


	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", path);
		return -1;
	}
	return fsbuf.f_bavail * fsbuf.f_bsize;	
}

/*返回dirname挂在的文件系统剩余空间百分比*/
float get_fs_stat(char *path) 
{	
	char buf[300];
	struct statfs fsbuf;

	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", path);
		return -1;
	}

	return (float)((fsbuf.f_bavail*100)/fsbuf.f_blocks);

}
/*返回剩余空间百分比最大的dirsname指针*/
struct dirsname *get_fs_dirs(struct dirsname *dirsp)
{
	char path[300];
	struct dirsname *tmp=NULL;
	struct dirsname *result=NULL;
	struct statfs fsbuf;
	float i = 0, j;
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
	strcat(buf, tmp);
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

	sprintf(datebuf, "%s/", path);
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
		if (get_fs_stat(path) < START_RELEASE) {
			while( get_fs_stat(path) < END_RELEASE ) {
				if (_dfile(path) < 0) { 
					openlog("fs_write", LOG_CONS|LOG_PID, 0);
					syslog(LOG_USER|LOG_ERR, "_dfile %s error!\n",path);
					fprintf(stderr, "dfile %s error!\n", path);
				}
			}
		}
		tmp = tmp->next;
	}
}

/*如果文件系统剩余空间不足则删除文件*/
int moniter(struct dirsname *dirsp, long file_size, int thread_n)
{
	char path[300];
	struct dirsname *tmp;
	int n, sum;

	while (1) {
		sum = n = 0;	
		if (update_list( dirsp, file_size, thread_n) < 0){
			fprintf(stderr, "update_list error!\n");
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR, "update_list  error!\n",tmp->name);
		}
		tmp = dirsp->next;
		while (tmp != dirsp) {
			sprintf(path, "%s/%s", root_dir, tmp->name);
			/*可用空间少于20%*/
			if (get_fs_stat(path) < START_RELEASE) {
				n++;
			}
			sum++;
			printf("%s\t", tmp->name);
			tmp = tmp->next;
		}
		printf("\n");
		if ((float)n/(float)sum >= 0.5){
			printf("delete\n");
			release_percent(dirsp);
		}  else {
			printf("sleep\n");
			sleep(10);
		}
	}
	return 0;
}

