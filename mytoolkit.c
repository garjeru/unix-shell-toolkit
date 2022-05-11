#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>

#define BUFF 81
#define SEPARATORS " \t\n"

void checkIo(char **args);
int countPipe(char **args);
void runPipe2(char **args, int pipeIdx, int pipeIdx2);
void runPipe(char **args, int pipeIdx);
int hasPipe(char **args);
int hasPipe2(char ** args, int pipeIdx);

int input, output, i, j;
char *inputFile;
char *temp;
char *outputFile;
char **cmds;
int inputLen;
int fds[2];
int child[2];
int child1[3];
char *cmd1[20];
char *cmd2[20];
char *cmd3[20];
int pipeIdx;
int fds1[2];
int fds2[2];
int pipeIdx2;

void checkIo(char **args){
	input = 0;
	i = 0;
	output = 0;

	while(args[i] != NULL){
	   if(strcmp(args[i], "<") == 0){
		args[i] = '\0';
		inputFile = args[i+1];
		input = 1;	
	    }
	    else if(strcmp(args[i], ">") == 0){
                outputFile = args[i+1];
		args[i] = '\0';
                output = 1;
		break;     
            }
	i++;
	}
}

int countPipe(char **args){
    int count = 0;
    i = 0;
    while(args[i] != NULL){
        if(strcmp(args[i], "|") == 0){
	    count = count + 1;	
        }
	i++;	
    }
    inputLen = i;
    return count;
}

int hasPipe(char **args){
    int i = 0;
    while(args[i] != NULL){
	if(strcmp(args[i], "|") == 0){
            return i;
	}else {
	    i++;
	}
    }
    return -1;
}

int hasPipe2(char **args, int pipeIdx){
    int i = pipeIdx + 1;
    while(args[i] != NULL){
        if(strcmp(args[i], "|") == 0 && pipeIdx != i){
            return i;
        }else {
            i++;
        }
    }
    return -1;
}


void runPipe(char **args, int pipeIdx){
    pipe(fds);
    if(fork() == 0){
        close(fds[1]);
	close(STDIN_FILENO); dup(fds[0]);
        execvp(args[pipeIdx+1], args+pipeIdx+1);
	exit(0);	
    }
    if(fork() == 0){
        close(fds[0]);
	close(STDOUT_FILENO); dup(fds[1]);
	execvp(args[0], args);
    }
    close(fds[0]); close(fds[1]);
    wait(&child[0]);
    wait(&child[1]);
}

void runPipe2(char **args, int pipeIdx, int pipeIdx2){
    pipe(fds1);
    pipe(fds2);
    
    if(fork() == 0){
        close(fds1[0]);
        close(fds1[1]);
        close(fds2[1]);
        close(STDIN_FILENO); dup(fds2[0]);
        execvp(args[pipeIdx2+1], args+pipeIdx2+1);
        exit(0);
    }    

    if(fork() == 0){
        close(fds1[1]);
        close(STDIN_FILENO); dup(fds1[0]);
	close(fds2[0]);
        close(STDOUT_FILENO); dup(fds2[1]);
        execvp(args[pipeIdx+1], args+pipeIdx+1);
        exit(0);
    }
    if(fork() == 0){
        close(fds1[0]);
        close(STDOUT_FILENO); dup(fds1[1]);
        execvp(args[0], args);
    }
    close(fds1[0]); close(fds1[1]);
    close(fds2[0]); close(fds2[1]);
    wait(&child1[0]);
    wait(&child1[1]);
    wait(&child1[2]);
}

int main(int argc, char* argv[]){
    char line[BUFF];
    char cwd[256];
    char * args[1024];
    char ** arg;
    int stat;
    pid_t pid;
    int fd_in;
    int fd_out;
    int pipeCount;

    while(1){
	printf("$ ");
	input = 1;	
	if(fgets(line, BUFF, stdin) == NULL){
	    break;
	}
	else {
	    arg = args;
	    *arg++ = strtok(line, SEPARATORS);  /* tokenize input */
            while((*arg++ = strtok(NULL, SEPARATORS)));
	    
	    checkIo(args);
            
            pipeCount = countPipe(args);
         
	    if(strcmp(args[0], "myexit") == 0){ /* terminate the toolkit */
                break;
            }
	    else if(pipeCount > 0){  /* pipe implementation */
		
		pipeIdx = hasPipe(args);
		
		if(pipeIdx >= 0){
		    
            for(i = 0; i < pipeIdx; i++){
                cmd1[i] = args[i];
            }
		    
		    args[pipeIdx] = '\0';
			 
 		    if(pipeCount == 1){
                     	runPipe(args, pipeIdx);
		    }else {
		        pipeIdx2 = hasPipe2(args, pipeIdx);
			args[pipeIdx2] = '\0';
			runPipe2(args, pipeIdx, pipeIdx2);	        
		     }		        
		}
	    }

	    else if (strcmp(args[0], "mycd") == 0){ /* change current working directory */
		if(chdir(args[1])){
	           printf("Please provide correct directory path\n"); 
		}else{
		   getcwd(cwd, sizeof(cwd));
                   printf("%s\n", cwd);
		}
		
            }
	    else if(strcmp(args[0], "mypwd") == 0){  
                getcwd(cwd, sizeof(cwd));
		printf("%s\n", cwd);
		
	    }
	    else if(strcmp(args[0], "mytimeout") == 0){
		if((pid = fork()) == 0) {
                    execvp(args[2], args+2);
		    kill(getppid(), SIGKILL);
                }
                else if(pid != -1){
		    sleep(atoi(args[1]));
		    if(kill(0, 0) == 0){
                        kill(0, SIGKILL);
		    }
                } 

			
	    }
	    else if(input == 1 && output == 1){

                fd_in = open(inputFile, O_RDONLY);
		fd_out = open(outputFile, O_RDWR | O_CREAT | O_TRUNC, 0777);
    
                if(fork() == 0) {
                    dup2(fd_in, 0);
		    dup2(fd_out, 1);
                    if(execvp(args[0], args) == 1){
                        exit(-1);
                    }

                }
                else{
                    wait(&stat);
                    close(fd_in);
		    close(fd_out);
                }

            }
	    else if(output == 1 && input == 0){	
		fd_out = open(outputFile, O_RDWR | O_CREAT | O_TRUNC, 0777);
			
		if(fork() == 0) {
		    dup2(fd_out, 1);
		    if(execvp(args[0], args) == 1){
			exit(-1);
	            }
		    
		}
		else{
		    wait(&stat);
		    close(fd_out);
		}	
	   
	    } else if(input == 1 && output == 0){     
		
		fd_in = open(inputFile, O_RDONLY);
                
		if(fork() == 0) {
		    dup2(fd_in, 0);
                    if(execvp(args[0], args) == 1){
                        exit(-1);
                    }

                }
                else{
                    wait(&stat);
		    close(fd_in);
                }

            }
	    else {
	        if(fork() == 0){
		    if(execv(args[0], args) == 1){
			exit(-1);	
		    }
		}else {
		  wait(&stat);
		}
				
	    } 
	           
	} 
    }
    return 0;
}
