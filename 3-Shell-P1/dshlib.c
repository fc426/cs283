#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	if (cmd_line == NULL) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    char *cmd_copy = strdup(cmd_line);
    if (cmd_copy == NULL) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    char *trim_cmd = cmd_copy;

    while (*trim_cmd == ' ') {
        trim_cmd++;
    }

    size_t cmd_len = strlen(trim_cmd);
    while (cmd_len > 0 && trim_cmd[cmd_len - 1] == ' ') {
        trim_cmd[--cmd_len] = '\0';
    }

    int cmd_count = 0;
    char *outer_ctx, *inner_ctx; 
    char *cmd_token = strtok_r(trim_cmd, PIPE_STRING, &outer_ctx); 

    while (cmd_token != NULL && cmd_count < CMD_MAX) { 
        memset(&clist->commands[cmd_count], 0, sizeof(command_t));

        char *cmd_exec = strtok_r(cmd_token, " ", &inner_ctx);
        if (cmd_exec != NULL) {
            if (strlen(cmd_exec) >= EXE_MAX) {
                free(cmd_copy);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            strcpy(clist->commands[cmd_count].exe, cmd_exec);

            char *arg_token = strtok_r(NULL, " ", &inner_ctx); 
            while (arg_token != NULL) {
                if (strlen(clist->commands[cmd_count].args) + strlen(arg_token) + 1 >= ARG_MAX) {
                    free(cmd_copy);
                    return ERR_CMD_OR_ARGS_TOO_BIG;
                }

                if (strlen(clist->commands[cmd_count].args) > 0) {
                    strcat(clist->commands[cmd_count].args, " ");
                }
                strcat(clist->commands[cmd_count].args, arg_token);
                arg_token = strtok_r(NULL, " ", &inner_ctx);
            }
        }

        cmd_count++; 
        cmd_token = strtok_r(NULL, PIPE_STRING, &outer_ctx);
    }

    if (cmd_token != NULL) {
        free(cmd_copy);
        return ERR_TOO_MANY_COMMANDS;
    }

    clist->num = cmd_count;
    free(cmd_copy);
    return OK;
}
