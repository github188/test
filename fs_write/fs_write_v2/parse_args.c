#include "parse_args.h"
/*检查目录是否挂载文件系统*/
int dir_mounted(char *dirname)
{
	FILE *fp;
	char buf[500];
	char path[300];

	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		fprintf(stderr, "Can't open /proc/mounts!\n");
		return -1;
	}
	strcpy(path, root_dir);
	strcat(path, "/");
	strcat(path, dirname);
	while (fgets(buf, 499, fp) > 0) {
		if (strstr(buf, path) != NULL) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;

}
int dir_enough_space(char *dirname)
{
	struct statfs statbuf;
	char path[300];

	strcpy(path, root_dir);
	strcat(path, "/");
	strcat(path, dirname);

	/*判断文件系统是否有足够的空间*/
		if (statfs(path, &statbuf) < 0) {
			fprintf(stderr, "%s can't statfs!\n", path);
			return -1;
		}
		if (statbuf.f_blocks * statbuf.f_bsize < BLOCK_SIZE * thread_n) {
			if (list_del(dirsp, dirname) < 0) {
				return -1;
			}
			/*fprintf(stderr, "%s have no enough space to run all threads\n", tmp->name);
			  openlog("fs_write", LOG_CONS|LOG_PID, 0);
			  syslog(LOG_USER|LOG_ERR, "%s have no enough space to run all threads\n", tmp->name);*/
		}
	return 0;

}
int check_fs(char *root_dir, struct dirsname *dirsp)
{
	struct dirsname *tmp;
	int ret;

	tmp = dirsp->next;
	while (tmp != dirsp){


		if (dir_mounted(tmp->name) > 0) {
			if (list_del(dirsp, tmp->name) < 0){
				return -1;
			}

			/*fprintf(stderr, "%s is not mounted a file system!\n", tmp->name);
			  openlog("fs_write", LOG_CONS|LOG_PID, 0);
			  syslog(LOG_USER|LOG_ERR, "%s is not mounted a file system!\n", tmp->name); */
		}
		if (dir_enough_space(tmp->name) < 0) {
				return -1;
		}
		tmp = tmp->next;
	}
	return 0;

}
/*将root_dir的子目录名字放到链表中*/
int get_dirs(char *root_dir, struct dirsname *dirsp)
{
	int i=0;
	DIR *dirp;
	struct dirent *dp;	
	struct dirsname *tmp;


	if ((dirp = opendir(root_dir)) < 0) {
		fprintf(stderr, "Can't open the root_dir!\n");
		return -1;
	}
	while ((dp = readdir(dirp)) != NULL ) {

		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				/*过滤掉.和..*/
				/*将链表中没有的目录加到链表中*/

				if(dir_mounted(dp->d_name) == 0) {
					if (list_add(dirsp, dp->d_name) < 0) {
						return -1;
					}	
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
void print_usage(void)
{
	printf( "Usage: fs_write filename  filesize  threadn  times\n" );
	printf( "\tfilename: the dir include mountpoints\n" );
	printf( "\tfilesize: the filesize the unit is M\n" );
	printf(	"\tthreadn : the number of write thread, should be lower 32\n" );
	/*printf(	"\ttimes   : every seconds change the mountpoints to write\n" ); */
	printf( "\n");
}
int check_threadn(void) {
	if (thread_n > THREAD_MAX) {
		fprintf(stderr, "thread's number lager than limits!\n");
		return -1;
	}
}
/*返回dirname目录挂载的文件系统剩余空间*/
long get_fs_space(char *dirname)
{
	char buf[300];
	struct statfs fsbuf;
	long i;

	strcpy(buf, root_dir);
	strcat(buf, "/");
	strcat(buf, dirname);

	if (statfs(buf, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", buf);
		return -1;
	}
	return fsbuf.f_bfree;	
}

/*返回dirname挂在的文件系统剩余空间百分比*/
int get_fs_stat(char *dirname) 
{	
	char buf[300];
	struct statfs fsbuf;
	strcpy(buf, root_dir);
	strcat(buf, "/");
	strcat(buf, dirname);

	if (statfs(buf, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs!\n", buf);
		return -1;
	}

	return (int)((fsbuf.f_bfree*100)/fsbuf.f_blocks);

}

struct dirsname *get_fs_dirs(struct dirsname *dirsp)
{
	struct dirsname *tmp=NULL;
	struct dirsname *result=NULL;
	struct statfs fsbuf;
	long i = 0, j;
	tmp = dirsp->next;
	while (tmp != dirsp) {
		if ((j = get_fs_space(tmp->name)) > i) {
			i = j;
			result = tmp;
		}	
		tmp = tmp->next;		
	}
	return result;
}
void print_dirsp(struct dirsname *dirsp)
{
	struct dirsname *tmp;
	tmp = dirsp->next;
	while(tmp != dirsp){
		printf("%s\t", tmp->name);
		tmp = tmp->next;
	}
	printf("\n");
}
int update_list(struct dirsname *dirsp)
{

	if (get_dirs(root_dir, dirsp) < 0){
		return -1;
	}
	if (check_fs(root_dir, dirsp) < 0) {
		return -1;
	}
	if (check_dirsp(dirsp) < 0) {
		return -1;
	}
	print_dirsp(dirsp);
	return 0;
}
int check_dirsp(struct dirsname *dirsp)
{
	if (dirsp->next == dirsp){
		fprintf(stderr, "there is no dirs can to test!\n");
		openlog("fs_write", LOG_CONS|LOG_PID, 0);
		syslog(LOG_USER|LOG_ERR, "check_dirsp error! list is empty\n");
		return -1;
	}
	else
		return 0;
}
/*参数解析*/
int parse_args(char *root_dir, struct dirsname *dirsp)
{
	if (update_list( dirsp) < 0) {
		return -1;
	}
	if (check_threadn() < 0) {
		return -1;
	}
	return 0;
}
