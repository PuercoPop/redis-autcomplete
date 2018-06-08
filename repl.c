#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "map.h"

#define UNUSED __attribute__((unused))

enum command_t {
  ADD_CANDIDATE,
  GET_CANDIDATES,
  UNKNOWN_COMMAND,
};

typedef struct Command {
  enum command_t type;
  char *candidate;
} Command;

static struct sigaction sa;

static struct Map *candidates_map;

static char *input;
static char *prompt = "db> ";
static char *history_file;

void sigint(const int);
void setup();
void die(char *, ...);
Command parse_command(char *);
void execute_command(Command);
bool print_candidate(const char *, void *, void *);

void die(char *msg, ...)
{
        va_list ap;

        va_start(ap, msg);
        vfprintf(stderr, msg, ap);
        va_end(ap);
        exit(EXIT_FAILURE);
}

void
sigint(const int UNUSED signo)
{
        write_history(history_file);
        exit(EXIT_SUCCESS);
}

bool print_candidate(const char *key, void *value, UNUSED void *date)
{
  printf("Candidate: %s | %s\n", key, (char *)value);
  return true;
}

Command
parse_command(char *input)
{
  Command ret;

  // TODO: Replace with strsep
  char *keyword_add_candidate = strtok(input, " ");
  char *candidate = strtok(NULL, " ");

  if (!strncasecmp("ADD_CANDIDATE", keyword_add_candidate, 13) && candidate) {
    ret.type = ADD_CANDIDATE;
    ret.candidate = input;
  } else if (!strncasecmp("GET_CANDIDATES", keyword_add_candidate, 14)) {
      ret.type = GET_CANDIDATES;
  } else {
    ret.type = UNKNOWN_COMMAND;
  }
  return ret;
}

void
execute_command(Command cmd)
{
  bool err;
  switch(cmd.type) {
  case ADD_CANDIDATE:
    err = map_put(candidates_map, cmd.candidate, NULL);
    if(err) {
      printf("Candidate %s added.\n", cmd.candidate);
    } else {
      printf("Unable to add Candidate %s.\n", cmd.candidate);
    }
    break;
  case GET_CANDIDATES:
    map_iterate(candidates_map, &print_candidate, NULL);
    break;
  default:
    printf("Unknown command\n");
  }
}

void
setup()
{
  	
        int err;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sigint;
        if(sigaction(SIGINT, &sa, NULL))
                die("Could not install SIGINT handler.\n");

        
        candidates_map = map_new();
        
        
        const char *home = getenv("HOME");
        if(!home || !*home)
                die("$HOME is not set.\n");

        err = asprintf(&history_file, "%s/.acr_repl_history", home);

        if (err <0)
                die("Could not allocate readline history file name.\n");

        using_history();
        read_history(history_file);
}

int
main(UNUSED int argc, UNUSED char *argv[])
{
        setup();
        Command cmd;
        for(;;) {
                input = readline(prompt);
                if(!input) {
                        printf("EOF.\n");
                        write_history(history_file);
                        exit(EXIT_SUCCESS);
                }
                add_history(input);
                cmd = parse_command(input);
                execute_command(cmd);
        }
        
        free(input);

        return EXIT_SUCCESS;
}
