#include <string.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

using namespace std;

pid_t compile_and_execute(const char * code, const char * libraries, char ** args) {
	/*dump code to file*/
	char * tmp = tmpnam(NULL);
	stringstream cppfile, exefile, cmd;
	cppfile<<tmp<<".cpp";
	exefile<<tmp<<".out";
	FILE * f = fopen(cppfile.str().c_str(), "w");
	unsigned int len = strlen(code);
	if(fwrite(code, sizeof(char), len, f)<len) return -1;
	fclose(f);
	/*compile the file*/
	//printf("compile %s\n", cppfile.str().c_str());
	cmd << "g++ -o "<<exefile.str()<<" "<<cppfile.str()<<" $DEFAULT_LIBRARIES ";
	if(libraries) cmd<<libraries;

	if(system(cmd.str().c_str())) {
		char code_name[16];
		unsigned int cpy_size = 16; if (len < cpy_size) cpy_size = len;
		strncpy(code_name, code, cpy_size);
		printf("!compilation of %s failed\n", code_name);
		return -1;
	}
	/*execute the file*/
	pid_t child = fork();
	if(child) return child; /*parent returns, child executes*/

	execv(exefile.str().c_str(), args); /*this shouldn't return*/

	char code_name[16];
	unsigned int cpy_size = 16; if (len < cpy_size) cpy_size = len;
	strncpy(code_name, code, cpy_size);
	printf("!execution of %s failed %s\n", code_name, strerror(errno));
	exit(0);
}
 
pid_t compile_and_execute(const char * code, const char * libraries, int n_args, va_list args) {
	char ** argarr =0;
	argarr = (char**)calloc(sizeof(char*),(n_args+1));
	for(int i = 0; i < n_args ; i++) {
		argarr[i]=va_arg(args, char*);
	}
	va_end(args);
	pid_t child = compile_and_execute(code, libraries, argarr);
	if(child>0) free(argarr);
	return child;
}

pid_t compile_and_execute(const char * code, const char * libraries, int n_args, ...) {
	va_list args;
	va_start (args, n_args);
	return compile_and_execute(code, libraries, n_args, args);
}

#if 0

int main (int argc, char * argv[]) {
	char test_code[] = "#include <string.h> \n\
#include <stdio.h> \n\
int main(int argc, char *argv[]) { \n\
	printf(\"say started with %s (ppid is %s)\\n\", argv[1], argv[0]); \n\
	printf(\"%s\\n\", argv[1]); \n\
}\n "; 

	char * input_data=0;
	if(argc>1) input_data = argv[1];
	compile_and_execute(test_code, (char*)0, 1, input_data, (char*)0);
}

#endif


