#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // for exit()
#include <wait.h>   // for wait()

int main(){
	pid_t  pid;
	pid_t  pid1;

	pid = fork();  //Fork a child process

        if(pid < 0){ //Fork error
		fprintf(stderr,"Fork failed.\n");
                exit(-1);
        }
        if(pid==0)
	{   // This is the path of child process
            printf("Process B\n");
	    sleep(10);
	    exit(0);
            //execlp(pid1); // call a text editor
	}
	else
	{ // This is the path of parent process
	    printf("Process A\n");
	    pid1 = fork();
	    if (pid1 == 0)
	    {
                printf("Process C\n");
                sleep(10);
                exit(0);
	    }
	    waitpid(pid, NULL, 0);
	    waitpid(pid1, NULL, 0);
            exit(0);
        }
}
