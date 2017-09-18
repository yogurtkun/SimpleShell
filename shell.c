#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "stdlib.h"
#include "shell.h"
#include "unistd.h"
#include "errno.h"
#include "sys/wait.h"
#include "list.h"
#include "limits.h"

HListP history_record;
char split_command[ARGMAX][ARGLEN] = {};
int in_std,out_std;
int p[2];
int previous;

int history_loop;

int main(int argc, char const *argv[])
{
    history_record = create_history();
    in_std = dup(0);
    out_std = dup(1);
    
    while(1){
        char command[COMMAND_BUFFER_LEN] = {};
        char short_command[COMMAND_BUFFER_LEN] = {};
        history_loop = INT_MAX;
        
        printf("$");
        fgets(command,COMMAND_BUFFER_LEN,stdin);
        if (command[0] == 0){
        	exit(EXIT_SUCCESS);
        }
        
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
        
        add_history(history_record, short_command);
        previous = 0;
        parse_command(short_command, NOT_EXIST_NEXT);
        
    }
    return 0;
}

int parse_command(char * o_short_command, int next){
	if (check_pipe(o_short_command) == -1)
    {
    	fprintf(stderr,"error: %s\n","Wrong pipe.");
    	return -1;
    }

    int arg_count = 0;
    char short_command[ARGLEN];
    strcpy(short_command,o_short_command);
    char * pch = NULL;

    pch = strtok(short_command,"|");
    while(pch!=NULL){
        strcpy((char *)split_command+ARGLEN*arg_count,pch);
        arg_count ++;
        pch = strtok(NULL,"|");
    }
    
    for (int i = 0; i < arg_count; ++i)
    {
        if (arg_count == 1 && next == NOT_EXIST_NEXT){
            if (conduct_command(split_command[i],SINGLE_COM, NOT_EXIST_NEXT) == -1)
            {
                break;
            }
        }
        else if (i == 0){
            if (conduct_command(split_command[i],FIRST_COM, EXIST_NEXT) == -1)
            {
                break;
            }
        }
        else if (i == arg_count-1 && next == NOT_EXIST_NEXT){
            if (conduct_command(split_command[i],LAST_COM, NOT_EXIST_NEXT) == -1)
            {
                break;
            }
        }
        else{
            if (conduct_command(split_command[i],MID_COM,EXIST_NEXT) == -1)
            {
                break;
            }
        }
    }
    
    return PARSE_SUCCESS;
}

int conduct_command(char *com,int status,int next){
    char temp_command[ARGLEN] = {};
    char * command_seq[ARGMAX] = {};
    int arg_count = 0;
    
    pipe(p);
    
    strcpy(temp_command,com);
    char * pch = strtok((char*)temp_command," ");
    if(pch ==NULL){
    	fprintf(stderr,"error: %s\n","Wrong pipe.");
    	return -1;
    }
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
        pid_t h_pid;
        if (arg_count == 1){
        	h_pid = fork();
        	if (h_pid < 0){
        		fprintf(stderr, "error: %s\n", strerror(errno));
        		return -1;	
        	}
        	if(h_pid == 0){
        		change_file(status);
        		print_history(history_record);
        		exit(EXIT_SUCCESS);
        	}else{
        		waitpid(h_pid,NULL,0);
        		close(p[1]);
        	}
        }else if(arg_count == 2 && strcmp(command_seq[1], "-c") == 0){
        	h_pid = fork();
        	if (h_pid < 0){
        		fprintf(stderr, "error: %s\n", strerror(errno));
        		return -1;	
        	}
        	if(h_pid == 0){
        		change_file(status);
        		clean_history(history_record);
        		exit(EXIT_SUCCESS);
        	}else{
        		waitpid(h_pid,NULL,0);
        		close(p[1]);
        	}
        }else if(arg_count == 2 ){
			int offest = -1;
			if(sscanf(command_seq[1], "%d",&offest) != 1 || (offest < 0 
				|| offest>=history_record->size || offest >= history_loop)){
			fprintf(stderr, "error: %s\n","Invalid offest");
			return(-1);
			}
                
			history_loop = offest;
			parse_command(find_n_th(history_record, offest),next);

        }else{
        	fprintf(stderr,"error: %s\n","Wrong arguments.");
        	return -1;
        }
    }
    else{
        if(my_exec(command_seq,arg_count, status) == -1){
            return -1;
        }
    }
    
    previous = p[0];
    return 0;
}

int my_exec(char * com[], int len,int status){
    pid_t pid;
    int pid_status;
    //fprintf(stderr, "read:%d write:%d previous_read:%d\n",p[0],p[1],previous );
    if ((pid = fork()) == 0){
        change_file(status);

        if (len > 1)
            pid_status = execv(com[0],com);
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
        close(p[1]);
        if (WEXITSTATUS(pid_status) == 1)
        {
        	return -1;
        }        
    }
    return 0;
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

int change_file(int status){
    if(status == SINGLE_COM){
        return 0;
    }else if (status == FIRST_COM){
    	dup2(previous,0);
        close(p[0]);
        dup2(p[1], 1);
    }else if(status == MID_COM){
        dup2(previous, 0);
        close(p[0]);
        dup2(p[1], 1);
    }else if(status == LAST_COM){
        dup2(previous, 0);
        close(p[0]);
    }
    return 0;
}

int check_pipe(char * command){
	int start = 0;
	int last_pipe =0;
	char last_c = 0;
	for(int i = 0 ; i < strlen(command) ; i++){
		if(!start && command[i] == '|')
			return -1;
		if(!start && command[i] != '|' && command[i] != ' ')
			start =1;
		if(last_pipe && command[i] == '|')
			return -1;
		if (command[i] == '|'){
			last_pipe = 1;
		}else{
			last_pipe = 0;
		}
		last_c = (command[i] == ' '?last_c:command[i]);
	}
	if(last_c=='|')
		return -1;
	return 0;
}