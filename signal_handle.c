#include "minishell.h"

#define SIGINT 2    //ctrl+C
#define SIGQUIT 3   //ctrl+"\"
#define SIGTERM 15  //ctrl+D

void handle_signals(int sig)
{
    if (sig == SIGINT)
        exit(1);
    else if (sig == SIGTERM)
    {
        
    }
}
sig_t signal(int sig, sig_t func)
{
    if (sig == SIGINT)
        exit(1);
    if 
}