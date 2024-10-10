/* sampleCodeModule.c */

#include <user_syscalls.h>
#include <user_lib.h>
#include <shell.h>

int main()
{
	// create_process(init_shell(), ...)
	//init_shell();
	create_process("shell", (uint64_t) init_shell, getpid(), 0, 0);

	return 0;
}