#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char **env)
{
	// char *arr;
	(void)argc;
	execve("/usr/bin/env", &argv[1], env);
}