/***************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-11-26 15:41
 * Filename : main.c
 * Description : 
 * *************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include "comm.h"
#include "parse_args.h"
#include "moniter.h"
#include "threads.h"
#include "main.h"
#include "list.h"

long file_size = 268435456;
int block_size = 1024;
int thread_n = 4;
int time_s = 0;
char root_dir[300];
struct dirsname *dirsp = NULL;
char policy[20];
pthread_t *pids = NULL;
volatile int exitcode = 0;

static void sig_int (int signo)
{
	if (signo == SIGINT) {
		exitcode = 1;
#ifdef DEBUG
		printf("catch SIGINT\n");
#endif
		/*for (i=0; i < thread_n; i++){
		  pthread_cancel((*(pids+i)));
		  }
		  if ((dirp = opendir(root_dir)) < 0) {
		  fprintf(stderr, "Can't open the root_dir!\n");
		  }
		  while ((dp = readdir(dirp))) {
		  if (dp->d_type == DT_DIR) {
		  if (strcmp(dp->d_name, ".") && strcmp(dp->d_
		  sprintf(buf, "%s/%s", root_dir, dp->d_name);
		  chdir(buf);
		  system("rm -rf tmp.*");      
		  }
		  }
		  exit (0); */
	}
}

int main(int argc, char *argv[])
{
	int p;

	if ((argc < 2)) {fprintf(stderr, "No root_dir!\n"); print_help();
		return -1;
	}
	dirsp = (struct dirsname *) malloc(sizeof (struct dirsname));
	if (dirsp == NULL) {
		fprintf(stderr, "malloc error : %s", strerror(errno));
		return -1;
	}
	dirsp->name[0] = '\0';
	dirsp->next = dirsp;
	dirsp->weight = 0;

	/*    if (chdir(root_dir) < 0) {
	      fprintf(stderr, "chdir %s : %s", root_dir, strerror(errno));
	      print_help();
	      return -1;
	      } */
	if (parse_args(dirsp, argc, argv, &p) < 0) {
		print_help();
		return -1;
	}
	signal(SIGINT, sig_int);
	pids = (pthread_t *) malloc(sizeof (pthread_t) * thread_n);
	if (start_w_thread(dirsp, file_size, block_size,
			   thread_n, time_s, p, pids) < 0) {
		return -1;
	}
	if (moniter(dirsp, file_size, thread_n, pids) < 0) {
		return -1;
	}
	return 0;
				
}
