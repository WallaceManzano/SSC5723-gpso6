#include "mylib.h"


int pipeline (FILE *fpin, FILE *fpout, const char *cmd1, const char* cmd2, ...){
	va_list args;
	int pipefd[2], prevfd = fileno(fpin), count = 2, status, i;
	const char **cmds;
	
	va_start(args, cmd2);
	while(va_arg(args, char *))
		count++;		
	
	va_end(args);

	cmds = malloc(count*sizeof(char*));
	cmds[0] = cmd1;
	cmds[1] = cmd2;

	/* count args */
	va_start(args, cmd2);
	for(i = 2; i < count; i++)
		cmds[i] = va_arg(args, char *);

	va_end(args);

	/* exec args */
	for(i = 0; i < count; i++){
		if(i + 1 != count)
			pipe(pipefd);
		else
			pipefd[1] = fileno(fpout);
		
		if(!fork()){
			dup2(prevfd, 0);
			dup2(pipefd[1], 1);
			execlp(cmds[i], cmds[i], NULL);

			/* if the command was not executed */
			return -1;
		}
		
		
		/* not to close stdout */
		if(pipefd[1] != 1) 
			close(pipefd[1]);
		
		/* not to close stdin */
		if(prevfd) 
			close(prevfd);
		prevfd = pipefd[0];
	}

	/* wait children */
	for(i = 0; i < count; i++){
		wait(&status);

		/* check some error */
		if(!WIFEXITED(status))
			return -1;
	}

	return WEXITSTATUS(status);
}

