/* Megan Fanning
* Project:Small Shell in C
* references:http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
* http://linux.die.net/man/
*/
#include <stdio.h>
#include <stdlib.h>  /* rand(), srand()*/
#include <time.h>   /*time*/
#include <sys/stat.h> /*mkdir*/
#include <sys/types.h> /*pid*/
#include <unistd.h> /*pid*/
#include <string.h>/*strcpy*/
#include <sys/types.h>
#include <sys/wait.h> /*wait*/
#include <fcntl.h>
#define MAX_INPUT  2048
#define MAX_ARGS 512
int exit_status =0;

/*-----ProtoTypes----------*/
void prompt(char*);
void parse(char*);
void cd(char*);
void shStatus();
void exitShell(int*);
void execSh(char*, char*[],int);
void parentWait();
void redirect(char* file,int);
void backgroundProcess(char*[],int);
/*--------------------------*/

/*** Prompt ***/
/*prompting for a command line using PATH  */ 
/*command [arg1 arg2 ...] [< input_file] [> output_file] [&]}*/
/* 2048 characters, and a maximum of 512 arguments*/

void prompt(char userInput[]){
	/*print prompt*/
    printf(":");
    fflush(stdout);
    /*take user input into char array and store length*/
    /*scanf ("%s",userInput);*/
    fgets(userInput,MAX_INPUT,stdin);

    parse(userInput);
};

void sigHandler(int sig){
	signal(sig, SIG_IGN);
}

/*** Parse ***/
void parse(char userInput[]){
	/*catch corner cases
	 *  if user hits enter without entering a command, prompt again
	 *  if user enters a comment # ignore line */
	if (userInput[0]=='\n' || userInput[0]=='#'){
		return;
	}

	// catch control-C kill signal
	signal(SIGINT, sigHandler);

	/* tokenize user input*/
	char *userCommand;
	/*char *args = malloc( sizeof(char*) * ( 512 + 1 ) );*/
	char *args[MAX_ARGS];
	/*trim off newline*/
	userInput = strtok(userInput, "\n");
	/*tokenize userInput */
	args[0] = strtok(userInput, " ");
	int argNum=0;

	while(args[argNum] != NULL){
		argNum++;
		args[argNum]=strtok(NULL, " ");
	}
	argNum++;
	args[argNum]=NULL;
	/*null terminate array*/
	userCommand=args[0];
	
	/*Control flow- check command */
	if(0==strcmp(userCommand,"cd")){
		cd(args[1]);
		return;
	}else if (0==strcmp(userCommand,"status")){
		shStatus();
		return;
	}else if (0==strcmp (userCommand,"exit")){
		exitShell(0);
		return;
	}else if(0==strcmp(args[argNum-2],"&")){
		backgroundProcess(args,argNum);
		return;
	}else {
		execSh(userCommand,args, argNum);
	}
};
/* */
void backgroundProcess(char* args[],int argsLength){
	/**set shell process to foreground and child process to background **
	 * setpgid() sets the PGID of the process specified by pid to pgid.
	 * If pid is zero, then the process ID of the calling process is used.
	 * If pgid is zero, then the PGID of the process specified by pid is made
	 *  the same as its process ID. ||setpgid(pid_t pid, pid_t pgid);*/
	//setpgid(0,0);

	int status;

	int cpid;
	cpid=fork();

		/*get shells pgid
		 * The getpgid() function shall return the process group ID of the process whose process ID is equal to pid.
		 *  If pid is equal to 0, getpgid() shall return the process group ID of the calling process.*/
	pid_t shellpgid= getpgid(0);

		/*set shell to run in foreground*/
	tcsetpgrp(fileno(stdout),shellpgid);

	/*check if child*/
	if (cpid==0){
	    	/*execute child process*/
		//redirect stdin/out to /dev/null
		redirect("/dev/null",fileno(stdin));
		redirect("/dev/null",fileno(stdout));

		char* shortArgs[MAX_ARGS];
		int i;

		for (i=0;i<argsLength-2;i++){
			shortArgs[i]=args[i];
		}
		shortArgs[i]=NULL;
		args=shortArgs;

		execvp(args[0],args);
	}else{
		pid_t backgroundProcess=waitpid(cpid, &status, WUNTRACED | WNOHANG);
		exit_status=waitpid(cpid, &status,WNOHANG);
		printf("beginning background process, id number: %d\n",backgroundProcess);
	}

}
/*** parentWait ***/
/* returns the exit status of the child */
void parentWait(){
	int status;
	int cpid;
	cpid=wait(&status);

	exit_status=waitpid(cpid, &status,WNOHANG);

	if(cpid==-1){
		perror("wait failed");
		exit(1);
	}

	if (WIFEXITED(status)){
		exit_status = WEXITSTATUS(status);
		//printf("exit status was %d \n", exit_status);
	}else{
		printf("Child terminated by signal:");
		int signal=WTERMSIG (status);
		printf("%d \n",signal);
		fflush(stdout);
	}
}
void redirect(char* file, int stream){
	int fd;
	int fd2;

	if(stream==fileno(stdin)){
		fd=open(file, O_CREAT|O_TRUNC|O_RDONLY, 0644);
	}else{
		fd=open(file, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	}

	if (fd<0){
		perror("open");
   		exit(1);
	}
	/*redirect std in/out to fd label fd2*/
	fd2=dup2(fd,stream);

	if (fd2<0){
		perror("open");
		exit_status = 1;
   		exit(1);
	}
	close(fd);
}

/*** execSh ***/
void execSh(char userInput[], char* args[], int numArgs){
	/* create new process*/
	char *file;
	int cpid; 
	cpid=fork();

	/*check if child*/
	if (cpid==0){
    	/*execute child process*/
		/* check for special characters */
		int i;
		int redirectIndex=-1;

		for (i=0;i<numArgs-1;i++){
			if(0==strcmp(args[i],"<")){
		        /*next chars are input file*/
				file = args[i+1];
		        redirect(file,fileno(stdin));
		        if(redirectIndex==-1){
		        	redirectIndex=i;
		        }
		    }
			if(0==strcmp(args[i],">")){
		        /*next chars are output*/
				file = args[i+1];
		        redirect(file,fileno(stdout));
		        if(redirectIndex==-1){
		        	redirectIndex=i;
		        }
		    }
		}
        if(redirectIndex!=-1){
        	char* shortArgs[MAX_ARGS];
			for (i=0;i<redirectIndex;i++){
				shortArgs[i]=args[i];
			}
        	shortArgs[i]=NULL;
        	args=shortArgs;
        }
		/*execute*/
    	/*execlp(userInput, userInput, (char*) NULL);*/
    	execvp(args[0],args);
    	/*if exec fails print failure message if exec success these lines are never reached*/
    	perror("exec error");
    	int status;
    	if (WIFEXITED(status)){
    			exit_status = WEXITSTATUS(status);
    	}
	}
	else if(cpid < 0){
    	printf("fork failed\n");
    	perror("fork()");
    	int status;
    	if (WIFEXITED(status)){
    			exit_status = WEXITSTATUS(status);
    	}
	}
	else if(cpid > 0){
		parentWait();
	}
}

/*** cd ***/
void cd(char* filename){
	if(filename==NULL){
		//char* home=getenv("/home");
		chdir("/home");
	}else{
		chdir(filename);
	}
	int status;
	if (WIFEXITED(status)){
			exit_status = WEXITSTATUS(status);
	}
}

/*** exit ****/
void exitShell(int *runProgram){
	/* close files, deallocate memory Atexit()? _exit? */
	*runProgram=0;
	int status;
}
	
/*** status ****/
void shStatus(){
	//get status from waitpid of parent exit status or terminating signal of the last foreground process
	printf("Exit status of last command was : %d \n", exit_status);
	fflush(stdout);
};

int main (){
	/*define variables*/
	int runProgram;
	runProgram=1;

	char userInput[2048];
	
	while(runProgram){
		/*prompting for a command line */ 
		/*pass in char array and accept the string length from the function*/
		prompt(userInput);
		/*then call parse user input*/
	}
	return 0;
};
