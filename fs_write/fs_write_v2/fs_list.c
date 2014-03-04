/*******************************************************************************
 * Author : liyunteng
 * Email : li_yunteng@163.com
 * Created Time : 2013-12-04 10:06
 * Filename : fs_list.c
 * Description : 
 * *****************************************************************************/
#include "fs_list.h"
int fs_list_empty(struct dirsname *dp)
{
	if (dp->next == dp)
		return 0;
	else
		return -1;
}

struct dirsname * fs_list_end(struct dirsname *dp)
{
	struct dirsname *tmp = dp;
	while (tmp->next != dp) {
		tmp = tmp->next;
	}
	if (tmp->next == dp)
		return tmp;
	return NULL;
}

int fs_list_have(struct dirsname *dp, char *dirname)
{
	struct dirsname *tmp = dp->next;
	while (tmp != dp) {
		if (strcmp(tmp->name, dirname) == 0) {
			return 0;
		} else {

			tmp = tmp->next;
		}
	}
	return -1;
}

int fs_list_add(struct dirsname *dp, char *dirname)
{
    	struct dirsname *node = NULL;
	struct dirsname *tmp = dp;

	if (fs_list_have(dp, dirname) == 0)
		return 0;
	
	tmp = fs_list_end(dp);
	node = (struct dirsname *) malloc(sizeof (struct dirsname));
	if (node == NULL) {
		fprintf(stderr, "Can't malloc fs_list node!\n");
		return -1;
	}
	strcpy(node->name, dirname);
	node->weight = 0;
	node->next = tmp->next;
	tmp->next = node;
	return 0;
}

int fs_list_del(struct dirsname *dp, char *dirname)
{
	struct dirsname *tmp_before = dp;
	struct dirsname *tmp = dp->next;
	do {
		if (!strcmp(tmp->name, dirname)) {
			tmp_before->next = tmp->next;
			free(tmp);
			break;
		} else {
			tmp_before = tmp;
			tmp = tmp->next;
		}
	} while (tmp_before != dp);
	return 0;
}

struct dirsname * fs_list_next(struct dirsname *dp, struct dirsname *now)
{
	struct dirsname *tmp = dp;
	if (!fs_list_empty(dp)) {
		return NULL;
	}
	do {
		if (tmp == now) {
			if (tmp->next == dp)
				return dp->next;
			else
				return tmp->next;
		}
		tmp = tmp->next;
	} while (tmp != dp);
	return NULL;
}
