/* *****************************************************************************
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
#include <stdint.h>
#include <dirent.h>
#include <limits.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <string.h>
#include <time.h>
#include "parse_args.h"
#include "comm.h"
#include "fs_list.h"
#include "moniter.h"
#include "threads.h"

extern int exitcode;

/*返回dirname目录挂载的文件系统可用空间*/
uint64_t get_fs_space(char *path)
{
	struct statfs fsbuf;

	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs : %s\n", path,
			strerror(errno));
		return -1;
	}
	return (uint64_t) fsbuf.f_bavail * fsbuf.f_bsize;
}

/*返回dirname挂在的文件系统剩余空间百分比*/
float get_fs_stat(char *path)
{
	struct statfs fsbuf;

	if (statfs(path, &fsbuf) < 0) {
		fprintf(stderr, "Can't get %s's statfs: %s\n", path,
			strerror(errno));
		return -1;
	}

	return ((float) fsbuf.f_bavail / fsbuf.f_blocks);

}

/*返回剩余空间百分比最大的dirsname指针*/
struct dirsname * get_fs_dirs_by_percent(struct dirsname *dirsp, int thread_n, long file_size)
{
	char path[300];
	struct dirsname *tmp = NULL;
	struct dirsname *result = NULL;
	float i = 0.0, j;
	tmp = dirsp->next;
	while (tmp != dirsp) {
		sprintf(path, "%s/%s", root_dir, tmp->name);
		if ((j = get_fs_stat(path)) > i &&
		    get_fs_space(path) > (uint64_t) thread_n * file_size) {
			i = j;
			result = tmp;
		}
		tmp = tmp->next;
	}
	return result;
}

/*返回剩余空间最大的dirsname指针*/
struct dirsname * get_fs_dirs_by_size(struct dirsname *dirsp, int thread_n, long file_size)
{
	char path[300];
	struct dirsname *tmp = NULL;
	struct dirsname *result = NULL;
	uint64_t i = 0, j;
	tmp = dirsp->next;
	while (tmp != dirsp) {
		sprintf(path, "%s/%s", root_dir, tmp->name);
		if ((j = get_fs_space(path)) > i
		    && get_fs_space(path) > (uint64_t) thread_n * file_size) {
			i = j;
			result = tmp;
		}
		tmp = tmp->next;
	}
	return result;
}

/*返回weight最小的dirsname指针*/
struct dirsname * get_fs_dirs_by_weight(struct dirsname *dirsp, int thread_n, long file_size)
{
	char path[300];
	struct dirsname *tmp = NULL;
	struct dirsname *result = NULL;
	unsigned int i = UINT_MAX;

	tmp = dirsp->next;
	while (tmp != dirsp) {
		sprintf(path, "%s/%s", root_dir, tmp->name);
		if (tmp->weight < i
		    && get_fs_space(path) > (uint64_t) thread_n * file_size) {
			i = tmp->weight;
			result = tmp;
		}
		tmp = tmp->next;
	}
	return result;
}

/*默认的写策略，如果文件系统的可用空间大于80%且足够多线程同时写则返回，否则返回下一个*/
struct dirsname * get_fs_dirs_default(struct dirsname *dirsp, struct dirsname *tmp_bef,
				      int thread_n, long file_size)
{
	char path[300];
	struct dirsname *tmp;
	struct dirsname *result;

	tmp = result = list_next(dirsp, tmp_bef);
	do {
		sprintf(path, "%s/%s", root_dir, result->name);
		//printf("result->name:%s\t get_fs_space:%ld\t file_size*n:%ld\n",tmp->name, get_fs_space(path), thread_n*file_size);
		if (get_fs_space(path) > (uint64_t) thread_n * file_size) {
			return result;
		}
		result = result->next;
	} while (result != tmp);
	fprintf(stderr, "no file system have enough space!\n");
	return NULL;
}


int _dfile(char *path)
{

	int n = 0;
	DIR *dirp;
	struct dirent *dp;
	struct stat statbuf;
	char datebuf[300];
	char hourbuf[200];
	char thefile[300];
	time_t nowtime;
	char cmd[400];
	char tmp[100] = "9999999999";

	/*找到最早的date目录 */
	sprintf(datebuf, "%s/", path);
	//strcpy(tmp,"999999999\0");
	if (!(dirp = opendir(path))) {
		fprintf(stderr, "Can't open the dir %s: %s\n", path,
			strerror(errno));
		closedir(dirp);
		return -1;
	}
	while ((dp = readdir(dirp))) {
		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				if (strcmp(dp->d_name, tmp) < 0) {
					strcpy(tmp, dp->d_name);
				}
			}
		}
	}
	strcat(datebuf, tmp);
#ifdef DEBUG
	printf("find date file : %s\n", datebuf);
#endif	
	
	/*找到最早的hour目录 */
	time(&nowtime);
	if ((dirp = opendir(datebuf)) < 0) {
		fprintf(stderr, "Can't open dir %s: %s\n", datebuf,
			strerror(errno));
		return -1;
	}
	while ((dp = readdir(dirp)) != NULL) {
		/*找到date目录下的hour目录 */
		if (dp->d_type == DT_DIR) {
			if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
				n++;
				sprintf(hourbuf, "%s/%s", datebuf, dp->d_name);
				/*得到目录的创建时间 */
				if (stat(hourbuf, &statbuf) < 0) {
					fprintf(stderr,
						"Can't stat the dir %s: %s\n",
						hourbuf, strerror(errno));
					return -1;
				}
				/*选择最早的目录 */
				if (statbuf.st_ctime < nowtime) {
					nowtime = statbuf.st_ctime;
					strcpy(thefile, hourbuf);
				}

			}
		}
	}
	/* 删除最早的hour目录*/
	if (n > 0) {
		sprintf(cmd, "rm -rf %s", thefile);

		if (system(cmd) < 0) {
			fprintf(stderr, "Can't delete the file %s: %s\n",
				thefile, strerror(errno));
			return -1;
		} else {
#ifdef DEBUG
			printf("deleted hour dir: %s\n", thefile);
#endif
			n--;
		}
	}
/*删除空date目录 */
	if (n == 0) {
		sprintf(cmd, "rm -rf %s", datebuf);
		if (system(cmd) < 0) {
			fprintf(stderr, "_dfile can't delete %s: %s\n", datebuf,
				strerror(errno));
			return -1;
		}
#ifdef DEBUG
		printf("deleted date dir: %s\n", datebuf);
#endif
	}
	sync();
	return 0;

}

int release_percent(char *path)
{
	if (get_fs_stat(path) <= START_RELEASE) {
		while (get_fs_stat(path) <= END_RELEASE) {
			if (_dfile(path) < 0) {
				openlog("fs_write", LOG_CONS | LOG_PID, 0);
				syslog(LOG_USER | LOG_ERR, "_dfile %s error!\n",
				       path);
				fprintf(stderr, "dfile %s error!\n", path);
				return -1;
			}
			if (exitcode) {
				return 0;
			}

		}
	}
	sync();
	return 0;
}

/*如果文件系统剩余空间不足则删除文件*/
int moniter(struct dirsname *dirsp, long file_size, int thread_n, pthread_t * pids)
{
	char path[300];
	struct dirsname *tmp;
	int n, sum, i, j, x;
	pthread_t *flag;
	char **paths;

	while (1) {
		sum = n = 0;
		tmp = dirsp->next;
		while (tmp != dirsp) {
			sprintf(path, "%s/%s", root_dir, tmp->name);
			/*可用空间少于20% */
			if (get_fs_stat(path) <= START_RELEASE) {
				n++;
			}
			sum++;
			tmp = tmp->next;
		}
		if ((float) n / sum >= 0.5) {
#ifdef DEBUG
			printf("starting delete thread!\n");
#endif
			i = 0;
			flag = (pthread_t *) malloc(sizeof (pthread_t) * n);
			/*存放path的值，供线程使用不然会有竞争 */
			paths = (char **) malloc(sizeof (char *) * n);
			for (x = 0; x < n; x++) {
				paths[x] = (char *) malloc(sizeof (char) * 300);
			}
			tmp = dirsp->next;
			while (tmp != dirsp) {
				sprintf(path, "%s/%s", root_dir, tmp->name);
				if (get_fs_stat(path) <= START_RELEASE) {
					strcpy(paths[i], path);
					if (start_d_thread(paths[i], flag + i) <
					    0) {
						fprintf(stderr,
							"create delete thread error!\n");
						return -1;
					}
#ifdef DEBUG
					printf
						("delete thread %d  into %s to delete tid:%u!\n",
						 i, path, (unsigned) *(flag + i));
#endif
					i++;
				}
				tmp = tmp->next;
			}
			for (j = 0; j < i; j++) {
				pthread_join(*(flag + j), NULL);
#ifdef DEBUG
				printf("delete thread %d returned tid:%u!\n", j,
				       (unsigned) *(flag + j));
#endif
			}
#ifdef DEBUG
			printf("delete successed!\n");
#endif
			free(flag);
			for (x = 0; x < n; x++) {
				free(paths[x]);
			}
			free(paths);
		} else {
			sleep(10);
		}
		if (exitcode) {
			for (i = 0; i < thread_n; i++) {
				pthread_join(*(pids + i), NULL);
			}
			free(pids);
			exit(-1);
		}
	}
	return 0;
}
