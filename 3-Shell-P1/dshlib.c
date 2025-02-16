#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || strspn(cmd_line, " ") == strlen(cmd_line)) {
        return WARN_NO_CMDS;
    }

    char *cmd_section;
    char *exe_name;
    char *cmd_args;
    int bytes_processed;
    int cmd_counter;
    int cmd_length;

    cmd_counter = 0;
    bytes_processed = 0;
    cmd_length = strlen(cmd_line);

    cmd_section = strtok(cmd_line, PIPE_STRING);
    while (cmd_section != NULL) {
        if (cmd_counter >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        for (int i = strlen(cmd_section) - 1; cmd_section[i] == SPACE_CHAR; i--) {
            cmd_section[i + 1] = SPACE_CHAR;
            cmd_section[i] = '\0';
        }

        bytes_processed += strlen(cmd_section) + 1;

        exe_name = strtok(cmd_section, " ");
        if (strlen(exe_name) > EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strcpy(clist->commands[cmd_counter].exe, exe_name);

        cmd_args = strtok(NULL, "");
        if (cmd_args != NULL) {
            if (strlen(cmd_args) > ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(clist->commands[cmd_counter].args, cmd_args);
        } else {
            strcpy(clist->commands[cmd_counter].args, "\0");
        }

        cmd_counter++;

        if (bytes_processed < cmd_length) {
            cmd_section = strtok(cmd_line + bytes_processed, PIPE_STRING);
        } else {
            cmd_section = NULL;
        }
    }

    clist->num = cmd_counter;
    return OK;
}

