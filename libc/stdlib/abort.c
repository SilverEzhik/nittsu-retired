#include <stdio.h>
#include <stdlib.h>

//no coming back from this
__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk) //kernel crash will lead to this
	// TODO: Add proper kernel panic.
	printf("kernel: panic: abort()\n");
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}
