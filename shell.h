//
//  shell.h
//  test
//
//  Created by 杨嘉辰 on 2017/9/15.
//  Copyright © 2017年 杨嘉辰. All rights reserved.
//

#ifndef _shell_H_
#define _shell_H_

#define MAX_INPUT_LEN 80
#define COMMAND_BUFFER_LEN 200
#define ARGMAX 40
#define ARGLEN 80

#define PARSE_SUCCESS 200

#define EXIST_NEXT 0
#define NOT_EXIST_NEXT 1

#define SINGLE_COM 0
#define FIRST_COM 1
#define MID_COM 2
#define LAST_COM 3

#define TESTSENTENCES printf("*********\n")

#include "string.h"

int check_pipe(char *str);

int parse_command(char *str, int status);

int conduct_command(char *str, int status, int next);

int check_arg_num(char *str, int sh);

int my_exec(char *a[], int len, int status);

int change_file(int status);

#endif
