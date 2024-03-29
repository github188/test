#include "parse_args.h"

/*检查子目录是否挂载了文件系统*/
int check_fs(int n)
{
	int i, flag;
	char buf[400];
	char path[300];
	FILE *fp;
	struct statfs statbuf;

	if ((fp = fopen("/proc/mounts", "r")) == NULL) {
		fprintf(stderr, "Can't open /proc/mounts!\n");
		return -1;
	}
	for (i=0; i<n; i++){

		flag = 0;
		strcpy(path,root_dir);
		strcat(path, "/");
		strcat(path, dirs_name[i]);

		fseek(fp, 0, SEEK_SET);
		while(fgets(buf, 400, fp) > 0) {
			if(strstr(buf, path) != NULL) {
				flag = 1;
				break;
			}
		}

		if (!flag) {
			fprintf(stderr, "%s is not a partion!\n", path);
			openlog("fs_write", LOG_CONS|LOG_PID, 0);
			syslog(LOG_USER|LOG_ERR,"%s is not a partion!\n", path);
			return -1;
		}

		/*判断文件系统是否有足够的空间*/
		if (statfs(path, &statbuf) < 0) {

			fprintf(stderr, "%s can't statfs!\n", path);
			return -1;
		}
		if (statbuf.f_blocks * statbuf.f_bsize < BLOCK_SIZE * thread_n) {
			fprintf(stderr, "%s don't have enough space!\n", path);
			return -1;
		}
	}
	fclose(fp);
	return 0;

}


/*将root_dir的子目录名字放到dirs_name*/
int get_dirs(void)
{
	int i=0;
	DIR *dirp;
	struct dirent *dp;

	if ((dirp = opendir(root_dir)) < 0) {
		fprintf(stderr, "Can't open the root_dir!\n");
		return -1;
	}
	while ((dp = readdir(dirp)) != NULL ) {

		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				/*过滤掉.和..*/
				strcpy(dirs_name[i], dp->d_name);
				i++;
				if (i >= MAX_DIRS) {
					/*判断是否超过MAX_DIRS*/
					closedir(dirp);
					fprintf(stderr, "Too many child dirs!\n");
					return -1;
				}
			}
		}
	}

	closedir(dirp);
	if (i == 0) {
		fprintf(stderr, "Don't have any child dir!\n");
		return -1;
	}
	return i;
}
void print_usage(void)
{
	printf( "Usage: fs_write filename  filesize  threadn  times\n" );
	printf( "\tfilename: the dir include mountpoints\n" );
	printf( "\tfilesize: the filesize the unit is M\n" );
	printf(	"\tthreadn : the number of write thread, should be lower 32\n" );
	printf(	"\ttimes   : every seconds change the mountpoints to write\n" );
	printf( "\n");
}
int check_threadn(void) {
	if (thread_n > THREAD_MAX) {
		fprintf(stderr, "thread's number lager than limits!\n");
		return -1;
	}
}
/*参数解析*/
int parse_args(void)
{

	int n; /*root_dir目录下挂载分区的目录个数*/

	if (chdir(root_dir) < 0) {
		fprintf(stderr, "Can't into the root_dir!\n");
		return -1;
	}
	if ((n = get_dirs()) < 0) {
		return -1;
	}
	/* printf("n: %d\n", n); */
	if (check_fs(n) < 0) {
		return -1;
	}
	if (check_threadn() < 0) {
		return -1;
	}
	return n;
}
