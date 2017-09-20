#ifndef _list_H_
#define _list_H_

#include "shell.h"
#include "stdlib.h"

typedef struct listnode *ListNodeP;

typedef struct listnode {
	char command[ARGLEN];
	ListNodeP next;
} ListNode;

typedef struct history_list *HListP;

typedef struct history_list {
	ListNodeP s;
	ListNodeP e;
	int size;
} HList;

HListP create_history(void);

int clean_history(HListP);

int add_history(HListP, char *);

char *find_n_th(HListP, int);

int print_history(HListP);

#endif
