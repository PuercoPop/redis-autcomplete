#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define UNUSED __attribute__((unused))

static struct sigaction sa;

static char *input;
static char *prompt = "db> ";
static char *history_file;

void sigint(const int);
void setup();
void die(char *, ...);


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

void
setup()
{
        int err;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sigint;
        if(sigaction(SIGINT, &sa, NULL))
                die("Could not install SIGINT handler.\n");
        
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
        for(;;) {
                input = readline(prompt);
                if(!input) {
                        printf("EOF.\n");
                        write_history(history_file);
                        exit(EXIT_SUCCESS);
                }
        }
        add_history(input);
        write_history(input);

        // IOU

        free(input);

        return EXIT_SUCCESS;
}
