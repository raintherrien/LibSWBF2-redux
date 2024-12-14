#include "pch.h"
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>

#ifdef WIN32

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

#elif !defined(NDEBUG) && defined(__GNUC__)

// Yeah this is awful. It's not my fault this library segfaults!
static void sigsegv_handler(int sig)
{
	void *array[10];
	size_t size;

	size = backtrace(array, 10);

	fprintf(stderr, "LibSWBF2 signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

struct RegisterSignalHandlers {
	RegisterSignalHandlers() {
		signal(SIGSEGV, sigsegv_handler);
	}
};

static RegisterSignalHandlers register_signal_handlers_singleton;

#endif
