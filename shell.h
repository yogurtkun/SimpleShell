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

#define SINGLE_COM 0
#define FIRST_COM 1
#define MID_COM 2
#define LAST_COM 3

#define TESTSENTENCES printf("*********\n")

#include "string.h"

int parse_command(char *);

int conduct_command(char *,int status);

int check_arg_num(char *,int sh);

int my_exec(char *a[],int, int );

int change_file(int status);

#endif