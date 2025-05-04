/*
 * shell.c
 */

#include "shell.h"
#include <stdio.h>
#include <string.h>
#include "drv_uart.h"

static h_shell_t *shell_global = NULL;  // Pointeur global accessible dans sh_help

static char backspace[] = "\b \b";
static char prompt[] = "> ";


int sh_help(int argc, char **argv) {
    for (int i = 0; i < shell_global->shell_func_list_size; i++) {
        int size = snprintf(shell_global->print_buffer, BUFFER_SIZE, "%c: %s\r\n",
                            shell_global->shell_func_list[i].c,
                            shell_global->shell_func_list[i].description);
        shell_global->io.drv_shell_transmit(shell_global->print_buffer, size);
    }
    return 0;
}

void shell_init(h_shell_t *shell) {
    shell_global = shell;  // Initialise le pointeur global

    shell->shell_func_list_size = 0;

    int size = snprintf(shell->print_buffer, BUFFER_SIZE,
                        "\r\n\r\n===== Monsieur Shell v0.2 =====\r\n");
    shell->io.drv_shell_transmit(shell->print_buffer, size);

    shell_add(shell, 'h', sh_help, "Help");
}

int shell_add(h_shell_t *shell, char c,
              int (*pfunc)(int, char **),
              char *description) {
    if (shell->shell_func_list_size < SHELL_FUNC_LIST_MAX_SIZE) {
        shell->shell_func_list[shell->shell_func_list_size].c = c;
        shell->shell_func_list[shell->shell_func_list_size].func = pfunc;
        shell->shell_func_list[shell->shell_func_list_size].description = description;
        shell->shell_func_list_size++;
        return 0;
    }
    return -1;
}

static int shell_exec(h_shell_t *shell, char *buf) {
    char c = buf[0];
    int argc = 1;
    char *argv[ARGC_MAX];
    argv[0] = buf;

    for (char *p = buf; *p != '\0' && argc < ARGC_MAX; p++) {
        if (*p == ' ') {
            *p = '\0';
            argv[argc++] = p + 1;
        }
    }

    for (int i = 0; i < shell->shell_func_list_size; i++) {
        if (shell->shell_func_list[i].c == c) {
            return shell->shell_func_list[i].func(argc, argv);
        }
    }

    int size = snprintf(shell->print_buffer, BUFFER_SIZE,
                        "%c: no such command\r\n", c);
    shell->io.drv_shell_transmit(shell->print_buffer, size);
    return -1;
}

int shell_run(h_shell_t *shell) {
    int reading = 0;
    int pos = 0;

    while (1) {
        shell->io.drv_shell_transmit(prompt, 2);
        reading = 1;

        while (reading) {
            char c;
            if (shell->io.drv_shell_receive(&c, 1) != 0)
                continue;

            int size;
            switch (c) {
            case '\r':
                size = snprintf(shell->print_buffer, BUFFER_SIZE, "\r\n");
                shell->io.drv_shell_transmit(shell->print_buffer, size);
                shell->cmd_buffer[pos++] = 0;
                shell_exec(shell, shell->cmd_buffer);
                reading = 0;
                pos = 0;
                break;

            case '\b':
                if (pos > 0) {
                    pos--;
                    shell->io.drv_shell_transmit(backspace, 3);
                }
                break;

            default:
                if (pos < BUFFER_SIZE - 1) {
                    shell->io.drv_shell_transmit(&c, 1);
                    shell->cmd_buffer[pos++] = c;
                }
                break;
            }
        }
    }

    return 0;
}
