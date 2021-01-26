#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Event handler
void alarm_handler(int signo){
  switch (signo) {
    
    case SIGALRM:
      printf("Alarm\n"); 
      break;

    case SIGINT:
      printf("CTRL+C pressed!\n"); 
      break;
    
    case SIGTSTP:
      printf("CTRL+Z pressed!\n"); 
      break;
    
    default:
      printf("Unrecognized signal!\n"); 
      break;
  }
}

// Timer
int main(void)
{
  // register the signal handler
  if (signal(SIGALRM, alarm_handler) == SIG_ERR
      || signal(SIGINT, alarm_handler) == SIG_ERR
      || signal(SIGTSTP, alarm_handler) == SIG_ERR) {

    printf("failed to register alarm handler.\n");
    exit(1);
  }

  while (1){ 
    alarm(2); // sends SIGARLM in 2 seconds
    sleep(10); // wait until alarm goes off
  } 
}
