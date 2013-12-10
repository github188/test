#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/vfs.h>
#include "comm.h"
#include "list.h"
#include "parse_args.h"

void print_help(void) 
{
	printf("Try fs_write --help or -h for more information.\n");
}
void print_usage(void)
{
	printf("fs_write, version 0.2\n");
	printf( "Usage: fs_write root_dir [[-s file_size] [-b block_size] [-n thread_n]\n");
	printf( "\t\t[-t time] [-p choose-policy]]\n" );
	printf( "\troot_dir        :\tthe dir include mountpoints.\n\n" );
	printf( "\t-s --file-size  :file_size \tthe filesize the unit is M default is 256M.\n" );
	printf( "\t-b --block-size :block_size\tthe blocksize of the file to write,default is 1024.\n");
	printf(	"\t-n --threadn    :thread_n    \tthe number of write thread, should be lower 32 default is 4.\n" );
	printf(	"\t-t --time       :time_s      \tevery seconds change the mountpoints to write. \n");
	printf( "\t\t\t\t\tif time_s=0 the thread will choose fs to write after write one file.\n" );
	printf( "\t-p --choose-policy:policy    \tthe policy used by write thread to choose file system.\n");
	printf( "\t\t\t\t\tit can be one of {weighting|free-size|free-percent}.\n");
	printf( "\n\n");
}

/*检查目录是否挂载文件系统 返回0为挂载了*/
int dir_mounted(char *dirname)
{
	FILE *fp;
	char buf[500];
	char path[300];

	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		fprintf(stderr, "Can't open /proc/mounts!\n");
		return -1;
	}
	sprintf(path, "%s/%s", root_dir, dirname);
	while (fgets(buf, 499, fp) > 0) {
		if (strstr(buf, path) != NULL) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return -1;

}
/*检查dirname目录可用空间是否足够*/
int dir_enough_space(char *dirname, long file_size, int thread_n)
{
	struct statfs statbuf;
	char path[300];
	int ret;
	sprintf(path, "%s/%s", root_dir, dirname);

	/*判断文件系统是否有足够的空间*/
	do  {ret = statfs(path, &statbuf);} while(ret < 0);
	if (statbuf.f_bavail * statbuf.f_bsize < file_size * thread_n) {
		return -1;
	}
	return 0;

}
/*删除链表中name值为dirname的*/
int del_list_node(struct dirsname *dirsp, char *dirname) {
	if (list_del(dirsp, dirname) < 0){
		fprintf(stderr, "delete %s from list error!\n", dirname);
		return -1;
	}
	return 0;
}
int check_fs( struct dirsname *dirsp, long file_size, int thread_n)
{
	struct dirsname *tmp;
	int ret;

	tmp = dirsp->next;
	while (tmp != dirsp){
		if (dir_mounted(tmp->name) < 0) {
			do {ret =  del_list_node(dirsp, tmp->name); } while (ret < 0);
		}
		if (dir_enough_space(tmp->name, file_size, thread_n) < 0) {
			do {ret =  del_list_node(dirsp, tmp->name); } while (ret < 0);
		} 
		tmp = tmp->next;
	}
	return 0;

}
/*将root_dir的子目录放到链表中*/
int get_dirs(struct dirsname *dirsp)
{
	int i=0;
	DIR *dirp;
	struct dirent *dp;	
	int ret;

	if ((dirp = opendir(root_dir)) < 0) {
		fprintf(stderr, "Can't open the root_dir!\n");
		return -1;
	}
	while ((dp = readdir(dirp)) != NULL ) {
		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				/*过滤掉.和..*/
				/*将链表中没有的目录加到链表中*/

				do { ret = list_add(dirsp, dp->d_name); } while (ret < 0);
				i++;
			}
		}
	}

	closedir(dirp);
	if (i == 0) {
		fprintf(stderr, "Don't have any child dir mounted file system!\n");
		return -1;
	}
	return 0;
}

int check_threadn(int thread_n) {
	if (thread_n > THREAD_MAX) {
		fprintf(stderr, "thread's number lager than limits!\n");
		return -1;
	}
	return 0;
}
/*打印链表*/
void print_dirsp(struct dirsname *dirsp)
{
	struct dirsname *tmp;
	tmp = dirsp->next;
	while(tmp != dirsp){
		printf("%s\t %d\n", tmp->name, tmp->weight);
		tmp = tmp->next;
	}
	printf("\n");
}
/*检查链表是否为空*/
int check_dirsp(struct dirsname *dirsp)
{
	if (list_empty(dirsp) == 0){
		fprintf(stderr, "check_dirsp error! list is empty!\n");
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "check_dirsp error! list is empty!\n");
		return -1;
	}
	else
		return 0;
}
int check_time(int time_s)
{
	if (time_s < 0) {
		fprintf(stderr, "time_s must greater than zero!\n");
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "time_s must greater than zero!\n");
		return -1;
	}

	return 0;
}
int check_policy(char *policy, int *p)
{
	if (!strcmp(policy, "")) {
		*p = 0;
		return 0;
	}
	if (!strcmp(policy, "weighting")){
		*p = 1;
		return 0;
	} else if (!strcmp(policy, "free-size")){
		*p = 2;
		return 0;
	} else if (!strcmp(policy, "free-percent")){
		*p = 3;
		return 0;
	}
	fprintf(stderr, "policy error!policy must in {weighting|free-size|free-percent}\n");
	return -1;
}
int update_list( struct dirsname *dirsp, long file_size, int thread_n)
{


	if (get_dirs(dirsp) < 0){
		return -1;
	}

	if (check_fs(dirsp, file_size, thread_n) < 0) {
		return -1;
	}
	//print_dirsp(dirsp);
	return 0;
}

/*参数解析*/
int parse_args(struct dirsname *dirsp, long file_size, int thread_n, int time_s, char *policy, int *p)
{
	if (update_list( dirsp, file_size, thread_n) < 0) {
		return -1;
	}
	if (check_dirsp(dirsp) < 0) {
		return -1;
	}
	/*if (check_threadn(thread_n) < 0) {
	  return -1;
	  }*/
	if (check_time(time_s) < 0) {
		return -1;
	}
	if (check_policy(policy, p) < 0) {
		return -1;
	}
	return 0;
}
