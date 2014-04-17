#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/vfs.h>
#include "comm.h"
#include "fs_list.h"
#include "parse_args.h"

extern long file_size;
extern int block_size;
extern int thread_n;
extern int time_s;
extern char policy[];

char *l_opt_arg;
char *const short_options = "d:s:b:n:i:p:h";
struct option long_options[] = {
	{"dir", 1, NULL, 'd'},
	{"file-size", 1, NULL, 's'},
	{"block-size", 1, NULL, 'b'},
	{"threadn", 1, NULL, 'n'},
	{"time", 1, NULL, 'i'},
	{"choose-policy", 1, NULL, 'p'},
	{"help", 0, NULL, 'h'},
	{0, 0, 0, 0}
};

void print_help(void)
{
	printf("Try fs_write --help or -h for more information.\n");
}

void print_usage(void)
{
	printf("fs_write, version 0.2\n");
	printf("Usage: fs_write -d <root_dir> [-s <file_size>] [-b <block_size>] [-n <thread_n>]\n");
	printf("\t\t[-i <interval>] [-p <choose-policy>]\n");
	printf("\t-d --dir	  :root_dir \tthe dir include mountpoints.\n\n");
	printf("\t-s --file-size  :file_size \tthe filesize the unit is M default is 256M.\n");
	printf("\t-b --block-size :block_size\tthe blocksize of the file to write,default is 1024.\n");
	printf("\t-n --threadn    :thread_n    \tthe number of write thread, should be lower 32 default is 4.\n");
	printf("\t-i --interval   :time_s      \tevery seconds change the mountpoints to write. \n");
	printf("\t\t\t\t\tif time_s=0 the thread will write one file system always and don't change.\n");
	printf("\t-p --choose-policy:policy    \tthe policy used by write thread to choose file system.\n");
	printf("\t\t\t\t\tit can be one of {weighting|free-size|free-percent}, default is weighting.\n");
}

/*检查目录是否挂载文件系统 返回0为挂载了*/
int dir_mounted(char *dirname)
{
	FILE *fp;
	char buf[500];
	char path[300];
	const char *delim = " ";
	
	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		fprintf(stderr, "Can't open /proc/mounts : %s\n",
			strerror(errno));
		return -1;
	}
	sprintf(path, "%s/%s", root_dir, dirname);
	while (fgets(buf, 499, fp) > 0) {
		/* 精确匹配 */
		char *s, *p = NULL;
	       
		s = strtok_r(buf, delim, &p);
		s = strtok_r(NULL, delim, &p);
		if (strcmp(path, s) == 0) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("%s not mounted a file system!\n", path);
#endif
	return -1;

}

/*检查dirname目录可用空间是否足够*/
int dir_enough_space(char *dirname, long file_size, int thread_n)
{
	struct statfs statbuf;
	char path[300];
	int ret;
	sprintf(path, "%s/%s", root_dir, dirname);

	/*判断文件系统是否有足够的空间 */
	do {
		ret = statfs(path, &statbuf);
	} while (ret < 0);
	if ((uint64_t) ((uint64_t) statbuf.f_bavail * statbuf.f_bsize) <
	    (uint64_t) (file_size * thread_n)) {
		return -1;
	}
	return 0;

}

/*删除链表中name值为dirname的*/
int del_list_node(struct dirsname *dirsp, char *dirname)
{
	if (fs_list_del(dirsp, dirname) < 0) {
		fprintf(stderr, "delete %s from list error : %s\n", dirname,
			strerror(errno));
		return -1;
	}
	return 0;
}

/*检查链表中每个都挂载了文件系统*/
int check_fs(struct dirsname *dirsp, long file_size, int thread_n)
{
	struct dirsname *tmp;
	int ret;

	tmp = dirsp->next;
	while (tmp != dirsp) {
		if (dir_mounted(tmp->name) < 0) {
			do {
				ret = del_list_node(dirsp, tmp->name);
			}
			while (ret < 0);
		}
		/*      if (dir_enough_space(tmp->name, file_size, thread_n) < 0) {
			do {ret =  del_list_node(dirsp, tmp->name); } while (ret < 0);
			} *//*不需要删除，在不同的策略中有判断 */
		tmp = tmp->next;
	}
	return 0;

}

/*将root_dir的挂载文件系统的子目录放到链表中*/
int get_dirs(struct dirsname *dirsp)
{
	int i = 0;
	DIR *dirp;
	struct dirent *dp;
	int ret;

	if ((dirp = opendir(root_dir)) < 0) {
		fprintf(stderr, "Can't open %s : %s!\n", root_dir,
			strerror(errno));
		return -1;
	}
	while ((dp = readdir(dirp)) != NULL) {
		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				/*过滤掉.和.. */
				/*将链表中没有的目录加到链表中 */
				if (!dir_mounted(dp->d_name)) {
					do {
						ret = fs_list_add(dirsp, dp->d_name);
					} while (ret < 0);
					i++;
				}
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

int check_filesize(int file_size, int block_size)
{
	if (file_size <= 0) {
		fprintf(stderr, "filesize illegal!");
		return -1;
	}
	if (file_size < block_size) {
		fprintf(stderr, "file_size smaller than block_size!\n");
		return -1;
	}
	return 0;
}

int check_blocksize(int block_size)
{
	if (block_size <= 0) {
		fprintf(stderr, "block_size illegal!\n");
		return -1;
	}
	return 0;
}

int check_threadn(int thread_n)
{
	if (thread_n <= 0) {
		fprintf(stderr, "thread's number illegal!\n");
		return -1;
	}
	return 0;
}

#ifdef DEBUG
/*打印链表*/
void print_dirsp(struct dirsname *dirsp)
{
	struct dirsname *tmp;
	tmp = dirsp->next;
	while (tmp != dirsp) {
		printf("%s\t%d\n", tmp->name, tmp->weight);
		tmp = tmp->next;
	}
	printf("\n");
}
#endif
/*检查链表是否为空*/
int check_dirsp(struct dirsname *dirsp)
{
	if (fs_list_empty(dirsp) == 0) {
		fprintf(stderr, "check_dirsp error! list is empty!\n");
		openlog("fs_write", LOG_CONS | LOG_PID, 0);
		syslog(LOG_USER | LOG_ERR,
		       "check_dirsp error! list is empty!\n");
		return -1;
	} else
		return 0;
}

int check_time(int time_s)
{
	if (time_s < 0) {
		fprintf(stderr, "time_s must greater than zero!\n");
		openlog("fs_write", LOG_CONS | LOG_PID, 0);
		syslog(LOG_USER | LOG_ERR, "time_s must greater than zero!\n");
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
	if (!strcmp(policy, "weighting")) {
		*p = 1;
		return 0;
	} else if (!strcmp(policy, "free-size")) {
		*p = 2;
		return 0;
	} else if (!strcmp(policy, "free-percent")) {
		*p = 3;
		return 0;
	}
	fprintf(stderr,
		"policy error!policy must in {weighting|free-size|free-percent}\n");
	return -1;
}

int check_rootdir(void)
{
		
	if (root_dir[0] == '\0') {
		fprintf(stderr, "root_dir is invalid.\n");
		return -1;
	}

	if (root_dir[strlen(root_dir) - 1] == '/') {
		if ((strcmp(root_dir, "/") != 0) && (strcmp(root_dir, "./") != 0))
			root_dir[strlen(root_dir) - 1] = '\0';
	}
	
	if (root_dir[0] != '/') {
		char buf[300];
		getcwd(buf, 300);
		int i = strlen(buf);
		buf[i] = '/';
		buf[i+1] = '\0';
		strcat(buf, root_dir);
		strcpy(root_dir, buf);
	}
	printf("root_dir:%s\n", root_dir);
	return 0;

}

/*添加目录中新挂载的文件系统目录到链表， 删除链表中没有挂载文件系统的项*/
int update_list(struct dirsname *dirsp, long file_size, int thread_n)
{

	if (get_dirs(dirsp) < 0) {
		return -1;
	}

	if (check_fs(dirsp, file_size, thread_n) < 0) {
		return -1;
	}
#ifdef DEBUG
	print_dirsp(dirsp);
#endif
	return 0;
}

int my_getopt(int argc, char **argv)
{
	int c;

	while ((c = getopt_long(argc, (char *const *) argv, short_options,
			    long_options, NULL)) != -1) {
		switch (c) {
		case 'd':
			strncpy(root_dir, optarg, sizeof(root_dir
					));
			break;
		case 's':
			file_size = atol(optarg) * 1024 * 1024;
			break;
		case 'b':
			block_size = atoi(optarg);
			break;
		case 'n':
			thread_n = atoi(optarg);
			break;
		case 'i':
			time_s = atoi(optarg);
			break;
		case 'p':
			strcpy(policy, optarg);
			break;
		case 'h':
			print_usage();
			exit(0);
			break;
		}
	}

	return 0;

#ifdef DEBUG
	printf("args file_size:%ld\nblock_size:%d\nthread_n:%d\ntime_s:%d\n",
	       file_size, block_size, thread_n, time_s);
#endif

}

/*参数解析*/
int
parse_args(struct dirsname *dirsp, int argc, char *argv[], int *p)
{
	if (argc < 2) {
		return -1;
	}
	if (my_getopt(argc, argv) < 0) {
		return -1;
	}

	if (check_rootdir() < 0) {
		return -1;
	}
	
	if (update_list(dirsp, file_size, thread_n) < 0) {
		return -1;
	}

	if (check_dirsp(dirsp) < 0) {
		return -1;
	}
	if (check_filesize(file_size, block_size) < 0) {
		return -1;
	}
	if (check_blocksize(block_size) < 0) {
		return -1;
	}
	if (check_threadn(thread_n) < 0) {
		return -1;
	}
	if (check_time(time_s) < 0) {
		return -1;
	}
	if (check_policy(policy, p) < 0) {
		return -1;
	}
	return 0;
}
