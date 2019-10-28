#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int main() {
   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }



char hostname[HOST_NAME_MAX];
char username[LOGIN_NAME_MAX];
gethostname(hostname, HOST_NAME_MAX);
getlogin_r(username, LOGIN_NAME_MAX);

       printf("Current host : %s\n",hostname);
       printf("Current user : %s\n",username);



   return 0;

}