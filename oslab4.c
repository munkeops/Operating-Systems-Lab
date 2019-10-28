#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h> 


FILE *brieffptr;
FILE *detailfptr;


void execute(char * file_name)
{
    FILE * filePointer; 
    filePointer = fopen(file_name,"r"); 
    char str1[100];
    

    while(fscanf(filePointer, "%[^\n]", str1)==1)
    {     
        pid_t pid;
        pid=fork();
        if(pid==0)//child process
        {
            
            char* args1[10];
           
            char *token3=strtok(str1," ");
            
            int i=0;
            while (token3!=NULL)
            {
                args1[i]=token3;
                i++;
                token3=strtok(NULL," ");
            }
            args1[i]=NULL;
            
            execvp(args1[0],args1); 
            exit(EXIT_SUCCESS);
        }
        else if(pid<0)
        {
            fprintf(stderr,"error in forking \n");
            return EXIT_FAILURE;
        }
        else
        {
            fscanf(filePointer, "%[^\n]", str1);
            char strt[100];
            strcpy(strt,str1);
            fprintf(detailfptr,"%s\n",str1);
            char *token=strtok(strt," ");
            fprintf(brieffptr,"%s\n",token);
            fseek(filePointer, SEEK_CUR, SEEK_SET+1);
            
           
            wait(NULL);
        }
    }

    fclose(filePointer);

}

void exec(char *s)
{
    //printf("hi\n");
    //printf("%s\n",s);

    pid_t pid;
    pid=fork();
    if(pid==0)//child process
    {
        
        char* args1[10];
        
        char *token3=strtok(s," ");
        
        int i=0;
        while (token3!=NULL)
        {
            args1[i]=token3;
            i++;
            token3=strtok(NULL," ");
        }
        args1[i]=NULL;
        printf("%s\n",args1[0]);
        execvp(args1[0],args1);
        exit(0); 
        
    }
    else if(pid<0)
    {
        fprintf(stderr,"error in forking \n");
        return EXIT_FAILURE;
    }
    else
    {
        wait(NULL);
    }
}

int isNumber(char* s) 
{ 
    int num=(s[0]-'0');
    if( num>=0 && num<9)
    {
        return num;
    } 
    else
    {
        num=-1;
        return num;
    }
    
} 
  
int main(int argc,char * argv[])
{
    
   
    int i=0;

    
    brieffptr = fopen("brief.txt", "w");
    detailfptr = fopen("detail.txt", "w");

    if(brieffptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    if(detailfptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    
    //int i=0;
    //for(i=0;i<)
    if(argv[1]==NULL)
    {
        printf("file name not provided. file name needed\n");
        return 0;
    }
    
   
    int count=1;
    


    while(argv[count]!=NULL)
    {
        execute(argv[count]);
        count++;
    }
    fclose(brieffptr);
    fclose(detailfptr);

    
    char str2[30];
    
    char str2cpy[100];
    char *token2;;
   

    while(1)
    {
        printf("Please enter your command\n");
        fgets(str2,30,stdin);
        //printf("%s\n",str2);
        strcpy(str2cpy,str2);
        token2=strtok(str2cpy," ");
        //printf("%s\n",token2);

           // char* args[10];
        if(strncmp(token2,"EXEC",strlen("EXEC"))==0)
        {
            token2=strtok(NULL,"");
            char* command=(char *)malloc(100*sizeof(char));

            int num=isNumber(token2);
            //printf("%d\n",num);
            if(num>=0)
            {
                //printf("hi\n");
                
                detailfptr = fopen("detail.txt", "r");
                if(detailfptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                int counter=0;
                while(fscanf(detailfptr, "%[^\n]", command)==1 )
                {
                    if(counter==num)
                    {
                        printf("%s\n",command);
                        exec(command);
                        break;
                    }
                   
                    fseek(detailfptr, SEEK_CUR, SEEK_SET+1);
                    counter++;
                }
                if(counter<num)
                {
                    printf("invalid entry\n");
                }
                
                
                
                
                              
                fclose(detailfptr);
                //execute(detailfptr);
                //remove("program.txt");
                //WRITE TO A FILE AND THEN EXECUTE FUNCTION TO EXECUTE IT
            }
            else
            {

                strncpy (command,str2+5, strlen(str2+5) );
                printf("%s",command);
                
                exec(command);
            }
        }
        

        else if(strncmp(token2,"HISTORY",strlen("HISTORY"))==0)
        {

            token2=strtok(NULL,"");
            if(strncmp(token2,"BRIEF",strlen("BRIEF"))==0)
            {
                char command[100];
                brieffptr = fopen("brief.txt", "r");
                if(brieffptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                while(fscanf(brieffptr, "%[^\n]", command)==1 )
                {
                    printf("%s\n",command);
                    fseek(brieffptr, SEEK_CUR, SEEK_SET+1);
                   
                }
                fclose(brieffptr);
            }
            else if(strncmp(token2,"DETAIL",strlen("DETAIL"))==0)
            {
                char command[100];
                detailfptr = fopen("detail.txt", "r");
                if(detailfptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                while(fscanf(detailfptr, "%[^\n]", command)==1 )
                {
                    printf("%s\n",command);
                    fseek(detailfptr, SEEK_CUR, SEEK_SET+1);
                   
                }
                fclose(detailfptr);
            }
            else if(token2==NULL)
            {
                printf("invalid command \n");
            }
            else
            {
                printf("invalid command \n");
            }
            
        }
        else if(strncmp(str2cpy,"STOP",strlen("STOP"))==0)
        {
            return 0;
        }
        else 
        {
            printf("invalid command \n");
            
        }
        
        
    }

    //free(brief_commands);
    //free(detail_commands);
    
}