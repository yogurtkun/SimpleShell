#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "stdlib.h"
#include "shell.h"
#include "unistd.h"
#include "errno.h"
#include "sys/wait.h"

int main(int argc, char const *argv[])
{
	char command[COMMAND_BUFFER_LEN] = {};
	char short_command[COMMAND_BUFFER_LEN] = {};
	char split_command[ARGMAX][ARGLEN] = {};

	int arg_count = 0;

	while(1){
		printf("$");
		fgets(command,COMMAND_BUFFER_LEN,stdin);

		/*Check the length of input*/
		if (strlen(command) >= MAX_INPUT_LEN)
		{
			fprintf(stderr,"error: %s\n","Input exceeds the max length");
			fflush(stdin);
			continue;
		}

		memcpy(short_command,command,MAX_INPUT_LEN-1);
		short_command[MAX_INPUT_LEN] = 0;

		int i = 0;
		while(short_command[i] != '\n'){i++;};
		short_command[i] = 0;

		char * pch = NULL;
		pch = strtok(short_command,"|");
		while(pch!=NULL){
			strcpy((char *)split_command+ARGLEN*arg_count,pch);
			arg_count ++;
			pch = strtok(NULL,"|");
		}

		for (int i = 0; i < arg_count; ++i)
		{
			if (arg_count == 1){
				if (conduct_command(split_command[i],SINGLE_COM) == -1)
				{
					break;
				}
			}
			else if (i == 0){
				if (conduct_command(split_command[i],FIRST_COM) == -1)
				{
					break;
				}
			}
			else if (i == arg_count-1){
				if (conduct_command(split_command[i],LAST_COM) == -1)
				{
					break;
				}
			}
			else{
				if (conduct_command(split_command[i],MID_COM) == -1)
				{
					break;
				}
			}
		}

		arg_count = 0;
	}
	return 0;
}


int conduct_command(char *com,int status){
	char temp_command[ARGLEN] = {};
	char * command_seq[ARGMAX] = {};
	int arg_count = 0;

	strcpy(temp_command,com);
	char * pch = strtok((char*)temp_command," ");
	while(pch != NULL){
		command_seq[arg_count]=pch;
		arg_count ++;
		pch = strtok(NULL," ");
	}

	if (strcmp(command_seq[0],"exit") == 0){
		exit(EXIT_SUCCESS);
	}
	else if(strcmp(command_seq[0],"cd") == 0){
		if (check_arg_num < 0)
		{
			fprintf(stderr,"error: %s\n","Too many arguments.");
			return -1;
		}

		int ch_status = chdir(command_seq[1]);
		if (ch_status < 0)
		{
			fprintf(stderr, "error: %s\n", strerror(errno));
		}
	}
	else if (strcmp(command_seq[0],"history") == 0){
		
	}
	else{
		int exec_status = my_exec(command_seq,arg_count, status);
	}

	return 0;
}

int my_exec(char * com[], int len,int status){
	pid_t pid;
	int pid_status;
	if ((pid = fork()) == 0){
		if (len > 1)
			pid_status = execv(com[0],com+1);
		else
			pid_status = execl(com[0],com[0],NULL);
		if (pid_status < 0)
		{
			fprintf(stderr, "error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else{
		waitpid(pid,&pid_status,0);
		return 0;
	}
}

int check_arg_num(char * com,int sh){
	char temp_argument[ARGLEN] = {};
	strcpy(temp_argument,com);

	int arg_num = 0;

	char * pch = NULL;

	pch = strtok(temp_argument," ");
	while(pch != NULL){
		arg_num++;
		pch = strtok(NULL," ");
	}

	if (arg_num > sh)
	{
		return -1;
	}else{
		return 0;
	}
}