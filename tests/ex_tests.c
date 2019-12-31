#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function) (const char *data);

int run_lib_function(char *lib_file, char *func_to_run, char *data);

int main(int argc, char *argv[])
{
	run_lib_function("./build/libex.so", "uppercase", "hello");

	return 0;
}

int run_lib_function(char *lib_file, char *func_to_run, char *data)
{
	int rc = 0;

	void *lib = dlopen(lib_file, RTLD_NOW);
	check(lib != NULL, "Failed to open the library %s: %s", lib_file,
	      dlerror());

	lib_function func = dlsym(lib, func_to_run);
	check(func != NULL,
	      "Did not find %s function in the library %s: %s", func_to_run,
	      lib_file, dlerror());

	rc = func(data);
	check(rc == 0, "Function %s return %d for data: %s", func_to_run,
	      rc, data);

	rc = dlclose(lib);
	check(rc == 0, "Failed to close %s", lib_file);

	return 0;

	error:
	return 1;
}