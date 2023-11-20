#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFFSIZE 1024
#define WRITE_BUFFSIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define LOWERCASE_CONVERT	1
#define UNSIGNED_CONVERT	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HISTORY_FILE	".simple_shell_history"
#define HISTORY_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int number;
	char *Str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@lin_count: the error count
 *@err_number: the error code for exit()s
 *@line_count_flag: if on count this line of input
 *@filename: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buff: address of pointer to cmd_buf, on if chaining
 *@cmd_buff_type: CMD_type ||, &&, ;
 *@readfile_d: the fd from which to read line input
 *@hist_count: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int lin_count;
	int err_number;
	int line_count_flag;
	char *filename;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buff; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buff_type; /* CMD_type ||, &&, ; */
	int readfile_d;
	int hist_count;
} infor_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(infor_t *);
} builtin_t;



/* builtins1.c */
int _myexit(infor_t *);
int _mycd(infor_t *);
int _myhelp(infor_t *);

/* builtins2.c */
int _myhistory(infor_t *);
int _myalias(infor_t *);


/* environ.c */
char *_getenv(infor_t *, const char *);
int _myenv(infor_t *);
int _mysetenv(infor_t *);
int _myunsetenv(infor_t *);
int populate_env_list(infor_t *);

/* errors_1.c */
void e_puts(char *);
int e_putchar(char);
int put_fd(char c, int fd);
int puts_fd(char *str, int fd);

/* errors_2.c */
int err_iatoi(char *);
void print_err(infor_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);


/* exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);


/* getline.c */
ssize_t get_input(infor_t *);
int _getline(infor_t *, char **, size_t *);
void sigintHandler(int);

/* get_info.c */
void clear_info(infor_t *);
void set_info(infor_t *, char **);
void free_info(infor_t *, int);



/* getenv.c */
char **get_environ(infor_t *);
int _unsetenv(infor_t *, char *);
int _setenv(infor_t *, char *, char *);

/* history.c */
char *get_history_file(infor_t *info);
int write_history(infor_t *info);
int read_history(infor_t *info);
int build_history_list(infor_t *info, char *buf, int linecount);
int renumber_history(infor_t *info);


/* _iatoi.c */
int interactive(infor_t *);
int is_delim(char, char *);
int _isalpha(int);
int _iatoi(char *);

/* lists_1.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* lists_2.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* loophsh.c */
int loophsh(char **);

/* memory.c */
int bfree(void **);

/* vars.c */
int is_chain(infor_t *, char *, size_t *);
void check_chain(infor_t *, char *, size_t *, size_t, size_t);
int replace_alias(infor_t *);
int replace_vars(infor_t *);
int replace_string(char **, char *);


/* parser.c */
int is_cmd(infor_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(infor_t *, char *, char *);

/* realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);


/* shloop.c */
int hsh(infor_t *, char **);
int find_builtin(infor_t *);
void find_cmd(infor_t *);
void fork_cmd(infor_t *);


/* string_1.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string_2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);


/* tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

#endif
