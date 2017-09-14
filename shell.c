#include "stdio.h"
#include "string.h"
#include "shell.h"

int main(int argc, char const *argv[])
{
	char command[COMMAND_BUFFER_LEN] = {};
	char short_command[COMMAND_BUFFER_LEN] = {};
	char split_command[ARGMAX][ARGLEN] = {};
	while(1){
		printf("$");
		fgets(command,COMMAND_BUFFER_LEN,stdin);

		/*Check the length of input*/
		if (strlen(command) >= MAX_INPUT_LEN)
		{
			printf("Input exceeds the max length\n");
			continue;
		}

		memcpy(short_command,command,MAX_INPUT_LEN-1);
		short_command[MAX_INPUT_LEN] = 0;
		int i = 0;
		while(short_command[i] != '\n'){i++;};
		short_command[i] = 0;

		char * pch = NULL;
		pch = strtok(short_command," ");

		int arg_count = 0;
		while(pch != NULL){
			strcpy((char *)split_command+arg_count*ARGLEN,pch);
			arg_count += 1;
			pch = strtok(NULL," ");
		}

		i = 0;
		for ( i = 0; i < arg_count; ++i)
		{
			printf("%d.%s\n", arg_count,split_command[i]);
		}
		
		break;
	}
	return 0;
}