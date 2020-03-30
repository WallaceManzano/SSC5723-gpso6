#include "mylib.h"



int main(int argc, char** argv){
	printf("ls | wc | wc\n");
	printf("return status = %d\n", pipeline(stdin, stdout, "ls", "wc", "wc", NULL));

	printf("ls | wc\n");
	printf("return status = %d\n", pipeline(stdin, stdout, "ls", "wc", NULL));

	printf("la | wc\n");
	printf("return status = %d\n", pipeline(stdin, stdout, "la", "wc", NULL));
	
	return 0;
}
