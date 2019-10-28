#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h> 
#include <unistd.h> 
#include <string.h> 
#include <limits.h>

FILE * commandsfptr;

#define RSH_RL_BUFSIZE 1024
char *rsh_read_line(void)
{
  int bufsize = RSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "rsh: allocation error\n");
    exit(EXIT_FAILURE);
  }


  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


#define RSH_TOK_BUFSIZE 64
#define RSH_TOK_DELIM " \t\r\n\a"
char **rsh_split_line(char *line)
{
  int bufsize = RSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "rsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  commandsfptr = fopen("commandslist.txt","a");
  
  fprintf(commandsfptr,"%s\n",line);
  fclose(commandsfptr);

  token = strtok(line, RSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += RSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "rsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, RSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


int rsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;
  FILE *fptr;
  fptr =fopen("pidhist.txt","a");
  int i=0,flag=0;
  while(args[i]!=NULL)
  {
     i++;
  }
  if(strncmp(args[i-1],"&",1)==0)
  {
    args[i-1]=NULL;
    flag=1;
  }


  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("rsh");
    }
    //fprintf(commandsfptr,"%s\n",args[0])
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("rsh");
  } else {
    // Parent process
    fprintf(fptr,"%s %d\n",args[0],pid);
    fclose(fptr);
    // while(args[i]!=NULL)
    // {
    //   i++;
    // }
    // int j=0;
    // while(args[i-1][j]!=NULL)
    // {
    //   j++;
    // }
    
    if(flag>0)
    {
      // FILE * current_pid_fptr = fopen("current_pid.txt","r");
      // fprintf(current_pid_fptr,"%s %d\n",args[0],pid);
      
      waitpid(pid, &status, WNOHANG);
      
      // fseek(current_pid_fptr,0,SEEK_SET);
      // char str1[50];
      // while(fscanf(current_pid_fptr, "%[^\n]", str1))
      // {

      // }
      //  fclose(current_pid_fptr);


    
    }
    else
    {
      do
      {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
  }

  return 1;
}


/*
  Function Declarations for builtin shell commands:
 */
int rsh_cd(char **args);
int rsh_help(char **args);
int rsh_exit(char **args);
int rsh_hist(char **args);
int rsh_pid();




/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "hist",
  "cd",
  "help",
  "exit",
  "pid",
};

int (*builtin_func[]) (char **) = {
  &rsh_hist,
  &rsh_cd,
  &rsh_help,
  &rsh_exit,
  &rsh_pid

};

int rsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int rsh_cd(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "rsh: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      perror("rsh");
    }
  }
  return 1;
}

int rsh_help(char **args)
{
  int i;
  printf("Rohan's RSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < rsh_num_builtins(); i++)
  {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int rsh_exit(char **args)
{
  remove("commandslist.txt");
  return 0;
}

int rsh_hist(char **args)
{
  pid_t pid, wpid;
  int status;
  FILE *fptr;
  fptr =fopen("pidhist.txt","a");
  pid =fork();
  if(pid==0)
  {
    int lines=0;
    
    FILE * fp;
    fp=fopen("commandslist.txt","r");
    fseek(fp, 0, SEEK_SET);
  
    char str1[30]; 
    int counter=1; 
    while(fscanf(fp, "%[^\n]", str1)==1)
    {     
      //if(counter>(lines-(args[0][4]-'0')))
      printf("%d. %s \n",counter,str1);
      fseek(fp, SEEK_CUR, SEEK_SET+1);
      counter++;
                   
    }

    fclose(fp);
    exit(EXIT_SUCCESS);
  }
  else if(pid<0)
  {
    fprintf(stderr,"error in forking \n");
    return EXIT_FAILURE;

  }
  else
  {
    fprintf(fptr,"hist %d\n",pid);
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
   

  }
  fclose(fptr);
  return 1;

  

}

int rsh_histn(char **args,int num)
{
  pid_t pid, wpid;
  int status;
  FILE *fptr;
  fptr =fopen("pidhist.txt","a");
  pid =fork();

  if(pid==0)
  {
    int lines=0;

    FILE * fp;
    fp=fopen("commandslist.txt","r");
    fseek(fp, 0, SEEK_SET);


    
    char str1[30]; 
    int counter=1; 
    while(fscanf(fp, "%[^\n]", str1)==1)
      {     
        
        fseek(fp, SEEK_CUR, SEEK_SET+1);
        counter++;
                    
      }
      int bound=counter;
      counter=0;
    fseek(fp, 0, SEEK_SET);

    while(fscanf(fp, "%[^\n]", str1)==1)
      {     
        if(counter>=(bound-num-1))
        {
              //printf("%d %d \n",counter,num);
              printf("%d. %s \n",counter-bound+num+2,str1);
        }
        fseek(fp, SEEK_CUR, SEEK_SET+1);
        counter++;
                    
      }

      fclose(fp);
      exit(EXIT_FAILURE);
  }
  else if (pid<0)
  {
    fprintf(stderr,"error in forking \n");
    return EXIT_FAILURE;
  }
  else
  {
    fprintf(fptr,"histn %d\n",pid);
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
   

  }
  fclose(fptr);
  return 1;
}



int rsh_pid()
{
  printf("command name : ./a.out   process id : %d\n",getpid());
  return 1;
}

int rsh_pid_all()
{
  FILE * fptr=fopen("pidhist.txt","r");
  char str1[100];
  while(fscanf(fptr, "%[^\n]", str1)==1)
  {
    char *token = strtok(str1," ");
    char pname[20];
    strcpy(pname,token);
    token=strtok(NULL," ");
    char pid[20];
    strcpy(pid,token);


    printf("command name : %s   process id : %s\n",pname,pid);
    fseek(fptr, SEEK_CUR, SEEK_SET+1);
  }
  fclose(fptr);
  return 1;
}



int rsh_execute(char **args)
{
  int i;
  int num=0;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  if(strlen(args[0])==5)
  {
    if (args[0][0]=='h'&&args[0][1]=='i'&&args[0][2]=='s'&&args[0][3]=='t')
    {
      num=args[0][4]-'0';
      strncpy(args[0],"hist",strlen("hist"));
    }
  }
  
  
  
  if(num>0)
  {
    int val= rsh_histn(args,num);
    //printf("%d\n",val);
    return val;
  }
  else if((strncmp(args[0],"pid",strlen("pid"))==0 )&&(args[1]!=NULL) &&(strncmp(args[1],"all",strlen("all"))==0))
    return rsh_pid_all();
  else
  {
    for (i = 0; i < rsh_num_builtins(); i++)
    {
      if (strncmp(args[0], builtin_str[i],strlen(builtin_str[i])) == 0)
      {
        int val= (*builtin_func[i])(args);
        //printf("%d\n",val);
        return val;
      }
    }
  }
  return rsh_launch(args);
}

int rsh_loop(void)
{

  char *line;
  char **args;
  char **args_new;
  int status;
  char cwd[PATH_MAX];
  char cwd2[PATH_MAX];

  char hostname[HOST_NAME_MAX];
  char username[LOGIN_NAME_MAX];
  gethostname(hostname, HOST_NAME_MAX);
  getlogin_r(username, LOGIN_NAME_MAX);
  if (getcwd(cwd, sizeof(cwd)) == NULL)
  {
    perror("getcwd() error");
    return 1;
  }

  FILE *fptr;
  fptr =fopen("pidhist.txt","a");
  fprintf(fptr,"./a.out %d\n",getpid());
  fclose(fptr);


  do {
    if (getcwd(cwd2, sizeof(cwd2)) == NULL)
    {
       perror("getcwd() error");
       return 1;
    }
    printf("%s@%s:~%s> ",username,hostname,cwd2+strlen(cwd));
    line = rsh_read_line();
    args = rsh_split_line(line);
    status = rsh_execute(args);

    free(line);
    free(args);
  } while (status);
  fclose(commandsfptr);
}



int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  rsh_loop();

  // Perform any shutdown/cleanup.
  remove("pidhist.txt");
  return EXIT_SUCCESS;
}
