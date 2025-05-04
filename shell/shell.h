/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 *
 *  modified by Faiçal FQUIHI
 *
 */

#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>



#define UART_DEVICE huart1

#define ARGC_MAX 8
#define BUFFER_SIZE 40
#define SHELL_FUNC_LIST_MAX_SIZE 64

// Fonctions d'interface pour transmission/réception
typedef uint8_t (*drv_shell_transmit_t)(char *pData, uint16_t size);
typedef uint8_t (*drv_shell_receive_t)(char *pData, uint16_t size);

typedef struct {
    drv_shell_transmit_t drv_shell_transmit;
    drv_shell_receive_t  drv_shell_receive;
} drv_shell_t;

// Type pour chaque commande du shell
typedef struct {
	char c;
	int (* func)(int argc, char ** argv);
	char * description;
} shell_func_t;

// Type regroupant toutes les variables internes
typedef struct {
	int shell_func_list_size;
	shell_func_t shell_func_list[SHELL_FUNC_LIST_MAX_SIZE];
	char print_buffer[BUFFER_SIZE];
	char cmd_buffer[BUFFER_SIZE];
	drv_shell_t io;
} h_shell_t;

/*void shell_init();
int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description);
int shell_run();*/

// prototype des fonctions avec h_shell en argument

void shell_init(h_shell_t *shell);
int shell_add(h_shell_t *shell, char c, int (* pfunc)(int, char **), char * description);
int shell_run(h_shell_t *shell);

#endif /* INC_LIB_SHELL_SHELL_H_ */
