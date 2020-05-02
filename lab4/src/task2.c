#include<stdlib.h>

#include <sys/types.h>

#include <unistd.h>

int main() 
{

   pid_t child_pid;

// Creation child process
   child_pid = fork();

   if (child_pid > 0) 
   {

      // Parent process, make half-minute pause
      sleep(30);

    }
    else
    {

      // child process immediately complete
      exit(0);

     }

    return 0;
}
