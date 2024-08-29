// #include "minishell.h"

// #define SIGINT 2    //ctrl+C
// #define SIGQUIT 3   //ctrl+"\"
// #define SIGTERM 15  //ctrl+D


// void handle_ctrl_c()
// {
//     // printf("\n");
// 	rl_on_new_line();
//     rl_redisplay();
// }
// void handle_ctrl_d()
// {
//     exit(-127);
// }
// void handle_signal()
// {
//     signal(SIGINT, handle_ctrl_c);
//     signal(SIGQUIT, handle_ctrl_d);
// }


#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h> // Assuming you're using readline

#define SIGINT 2    // Ctrl+C
#define SIGQUIT 3   // Ctrl+
#define SIGTERM 15  // Termination signal (not Ctrl+D by default)

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void handle_ctrl_c()
{
    printf("\n");
	rl_on_new_line();  // Resets the input line in readline
    rl_replace_line("", 0);  // Clears the current input line
    rl_redisplay();  // Redisplay the prompt
}

void handle_ctrl_d()
{
    exit(127);
}

void handle_signal()
{
    signal(SIGINT, handle_ctrl_c);
    signal(SIGTERM, handle_ctrl_d);
}

int main()
{
    // Disable readline's internal signal handling
    rl_catch_signals = 0;
    handle_signal();
    while (1)
    {
        char *input = readline("Prompt> ");
        // if (!input) {
        //     break;  // Ctrl+D (EOF) pressed
        // }
        free(input);
    }

    return 0;
}

