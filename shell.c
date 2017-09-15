#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "stdlib.h"
#include "shell.h"
#include "unistd.h"

int conduct_command(char * com,int flag){
	printf("%s\n", com);
	if (strcmp(com,"exit") == 0)
	{
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char const *argv[])
{
	char command[COMMAND_BUFFER_LEN] = {};
	char short_command[COMMAND_BUFFER_LEN] = {};
	char split_command[ARGMAX][ARGLEN] = {};

	int arg_count = 0;
	int current_arg_pos = 0;

	while(1){
		printf("$");
		fgets(command,COMMAND_BUFFER_LEN,stdin);

		/*Check the length of input*/
		if (strlen(command) >= MAX_INPUT_LEN)
		{
			fprintf(stderr,"error: %s\n","Input exceeds the max length");
			continue;
		}

		memcpy(short_command,command,MAX_INPUT_LEN-1);
		short_command[MAX_INPUT_LEN] = 0;

		int i = 0;
		while(short_command[i] != '\n'){i++;};
		short_command[i] = 0;

		char * pipe_pos = get_pipe_pos(short_command);

		char * pch = NULL;
		pch = strtok(short_command," ");
		
		while(pch != NULL){
			if (pch -1 == pipe_pos)
			{
				strcpy((char*)split_command+arg_count*ARGLEN,"|");
				arg_count ++;
			}
			strcpy((char *)split_command+arg_count*ARGLEN,pch);
			arg_count += 1;
			pch = strtok(NULL," ");
		}
		
		char single_command[40] = {};
		int p_pipe = 0;
		pid_t pid;
		int status;

		for (i = 0; i < arg_count; ++i)
		{
			if(strcmp(split_command[i], "|") != 0){
				strcat(single_command,split_command[i]);
				strcat(single_command," ");
			}else{

				if (!p_pipe)
					status = conduct_command(single_command,FIRST_COM);
				else
					status = conduct_command(single_command,MID_COM);
				
				memset(single_command,0,40*sizeof(char));
				p_pipe = 1;
			}
		}

		if (!p_pipe)
			conduct_command(single_command,SINGLE_COM);
		else
			conduct_command(single_command,LAST_COM);
		memset(single_command,0,40*sizeof(char));

	}
	return 0;
}