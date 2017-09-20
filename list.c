#include "list.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

HListP create_history(void)
{
	HListP new_list = (HListP) malloc(sizeof(HList));
	new_list->size = 0;
	new_list->s = (ListNodeP) malloc(sizeof(ListNode));
	new_list->s->next = NULL;
	new_list->e = new_list->s;
	return new_list;
}

int clean_history(HListP h)
{
	ListNodeP p, n;
	h->e = h->s;
	p = h->s->next;
	h->s->next = NULL;
	h->size = 0;
	while (p != NULL) {
		n = p->next;
		free(p);
		p = n;
	}
	return 0;
}

int add_history(HListP h, char *c)
{

	h->e->next = (ListNodeP) malloc(sizeof(ListNode));
	strcpy(h->e->next->command, c);
	h->e = h->e->next;
	h->e->next = NULL;

	if (h->size == 100) {
		ListNodeP temp = h->s->next;
		h->s->next = temp->next;
		free(temp);
	} else {
		h->size++;
	}

	return 0;
}

char *find_n_th(HListP h, int n)
{
	ListNodeP p = h->s->next;
	if (n + 1 > h->size) {
		return NULL;
	}

	int i = 0;
	for (; i < n; i++) {
		p = p->next;
	}
	return p->command;
}

int print_history(HListP h)
{
	int n = h->size;
	int i = 0;
	ListNodeP w_p = h->s->next;
	for (; i < n; i++) {
		fprintf(stdout, "%d %s\n", i, w_p->command);
		w_p = w_p->next;
	}
	return 0;
}
