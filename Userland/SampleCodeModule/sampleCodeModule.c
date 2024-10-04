/* sampleCodeModule.c */

#include <user_syscalls.h>
#include <user_lib.h>
#include <shell.h>

int main()
{
	// create_process(init_shell(), ...)
	init_shell();

	return 0;
}