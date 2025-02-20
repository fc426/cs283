#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#include "dshlib.h"

static int previous_exit_code = 0;

int free_cmd_buff(cmd_buff_t *cmd_buff) {
	if (cmd_buff->_cmd_buffer) {
		free(cmd_buff->_cmd_buffer);
		cmd_buff->_cmd_buffer = NULL;
	}
	for (int i = 0; i < cmd_buff->argc; i++) {
		if (cmd_buff->argv[i]) {
			free(cmd_buff->argv[i]);
			cmd_buff->argv[i] = NULL;
		}
	}
	cmd_buff->argc = 0;
	return OK;
}

int clean_cmd_buff(cmd_buff_t *cmd_buff) {
	for (int i = 0; i < cmd_buff->argc; i++) {
		if (cmd_buff->argv[i]) {
			free(cmd_buff->argv[i]);
			cmd_buff->argv[i] = NULL;
		}
	}
	cmd_buff->argc = 0;
	if (cmd_buff->_cmd_buffer) {
		cmd_buff->_cmd_buffer[0] = '\0';
	}
	return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
	cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX * sizeof(char));
	if (!cmd_buff->_cmd_buffer) {
		return ERR_MEMORY;
	}
	cmd_buff->argc = 0;
	for (int i = 0; i < CMD_ARGV_MAX; i++) {
		cmd_buff->argv[i] = NULL;
	}
	return OK;
}

Built_In_Cmds match_command(const char *input) {
	if (strcmp(input, EXIT_CMD) == 0) return BI_CMD_EXIT;
	if (strcmp(input, "cd") == 0) return BI_CMD_CD;
	if (strcmp(input, "dragon") == 0) return BI_CMD_DRAGON;
	if (strcmp(input, "rc") == 0) return BI_RC;
	return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
	Built_In_Cmds type = match_command(cmd->argv[0]);
	if (type == BI_CMD_EXIT) exit(0);
	if (type == BI_CMD_CD) {
		if (cmd->argc == 2) {
			if (chdir(cmd->argv[1]) != 0) {
				perror("cd failed");
				return ERR_EXEC_CMD;
			}
		} else if (cmd->argc > 2) {
			fprintf(stderr, "cd: too many arguments\n");
			return ERR_CMD_ARGS_BAD;
		}
		return BI_EXECUTED;
	}
	if (type == BI_CMD_DRAGON) {
		if (cmd->argc > 1) {
			fprintf(stderr, "dragon: too many arguments\n");
			return ERR_CMD_ARGS_BAD;
		}
		print_dragon();
		return BI_EXECUTED;
	}
	if (type == BI_RC) {
		printf("%d\n", last_exit_code);
		return BI_EXECUTED;
	}
	return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork failed");
		return ERR_EXEC_CMD;
	}
	if (pid == 0) {
		execvp(cmd->argv[0], cmd->argv);
		int err = errno;
		if (err == ENOENT) fprintf(stderr, "Command not found\n");
		else if (err == EACCES) fprintf(stderr, "Permission denied\n");
		else perror("execvp failed");
		exit(err);
	}
	int status;
	waitpid(pid, &status, 0);
	previous_exit_code = WEXITSTATUS(status);
	return status;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
	int argc = 0, token_index = 0;
	char token[ARG_MAX], *p = cmd_line;
	bool in_quote = false, in_token = false;
	while (*p != '\0') {
		if (in_quote) {
			if (*p == '"') {
				in_quote = false;
				token[token_index] = '\0';
				if (token_index > 0) {
					if (argc >= CMD_ARGV_MAX - 1) return ERR_CMD_ARGS_BAD;
					cmd_buff->argv[argc++] = strdup(token);
				}
				token_index = 0;
				in_token = false;
			} else {
				token[token_index++] = *p;
			}
		} else {
			if (*p == '"') {
				in_quote = true;
				in_token = true;
			} else if (isspace((unsigned char)*p)) {
				if (in_token) {
					token[token_index] = '\0';
					if (argc >= CMD_ARGV_MAX - 1) return ERR_CMD_ARGS_BAD;
					cmd_buff->argv[argc++] = strdup(token);
					token_index = 0;
					in_token = false;
				}
			} else {
				in_token = true;
				token[token_index++] = *p;
			}
		}
		p++;
	}
	if (in_quote || in_token) {
		token[token_index] = '\0';
		if (argc < CMD_ARGV_MAX - 1) cmd_buff->argv[argc++] = strdup(token);
	}
	cmd_buff->argv[argc] = NULL;
	cmd_buff->argc = argc;
	return OK;
}

int exec_local_cmd_loop() {
	cmd_buff_t cmd;
	if (alloc_cmd_buff(&cmd) != OK) {
		fprintf(stderr, "Failed to allocate command buffer\n");
		return ERR_MEMORY;
	}
	char cmd_buff[SH_CMD_MAX];
	int interactive = isatty(STDIN_FILENO);
	while (1) {
		printf("%s", SH_PROMPT);
		if (interactive) fflush(stdout);
		if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
			printf("\n");
			break;
		}
		cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
		clean_cmd_buff(&cmd);
		if (strlen(cmd_buff) == 0) continue;
		strncpy(cmd._cmd_buffer, cmd_buff, SH_CMD_MAX);
		if (build_cmd_buff(cmd_buff, &cmd) != OK) {
			fprintf(stderr, "Error parsing command line\n");
			continue;
		}
		if (cmd.argc == 0) continue;
		if (match_command(cmd.argv[0]) != BI_NOT_BI) exec_built_in_cmd(&cmd);
		else exec_cmd(&cmd);
	}
	free_cmd_buff(&cmd);
	return OK;
}

