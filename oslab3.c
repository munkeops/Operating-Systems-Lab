#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>



int main()
{
    char str1[100];
    FILE * filePointer; 
    filePointer = fopen("oslab2.txt","r");

    while(fscanf(filePointer, "%[^\n]", str1)==1)
    {
        int mypipe[2];
        // create the pipe
        if (pipe(mypipe))
        {
            fprintf(stderr,"failed to create a pipe");
            return EXIT_FAILURE;
        }

        pid_t pid;
        pid=fork();
        if(pid==0)//child process
        {
            char command[100];
            close(mypipe[1]);
            read(mypipe[0],command,100);
            //char *args[]={command,NULL}; 
            //printf("%s\n",args[0]);
            char *token=strtok(command," ");
            char* args[10];
            int i=0;
            while (token!=NULL)
            {
                args[i]=token;
                i++;
                //printf("%s\n",token);
                token=strtok(NULL," ");
            }
            args[i]=NULL;
            //char *args[]={str1,NULL}; 
            execvp(args[0],args); 
            //printf("%s\n",args[0]);
            //return 0;
            close(mypipe[0]);
            return 0;
           
        }
        else if(pid<0)
        {
            fprintf(stderr,"error in forking \n");
            return EXIT_FAILURE;
        }
        else
        {
            fscanf(filePointer, "%[^\n]", str1);
            //printf("the string : %s \n",str1);
            fseek(filePointer, SEEK_CUR, SEEK_SET+1);
            close(mypipe[0]);
            write(mypipe[1],str1,strlen(str1)+1);
            close(mypipe[1]);
            wait(NULL);
            //return EXIT_SUCCESS;
            //exit(0);
        }
    
    }

    fclose(filePointer);
    return 0;
}