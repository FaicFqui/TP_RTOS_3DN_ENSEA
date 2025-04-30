/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 */

/*#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>

#define UART_DEVICE huart1

#define ARGC_MAX 8
#define BUFFER_SIZE 40
#define SHELL_FUNC_LIST_MAX_SIZE 64

void shell_init();
int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description);
int shell_run();

#endif /* INC_LIB_SHELL_SHELL_H_
*/

#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#define ARGC_MAX 8
#define BUFFER_SIZE 64
#define SHELL_FUNC_LIST_MAX_SIZE 32

typedef struct {
	char c;
	int (*func)(int argc, char **argv);
	char *description;
} shell_func_t;

void shell_init(void);
int shell_add(char c, int (*func)(int, char **), char *description);
int shell_run(void);  // Boucle shell bloquante

#endif // SHELL_H
