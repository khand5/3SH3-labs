#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum timer_sates {SLEEP, ALARM};

// Timer State
int STATE = SLEEP;
bool TICK = true;

// Event Handler
void alarm_handler(int signo){
  switch (signo) {
   
    /* If SIGALRM then sound alarm every two seconds */
    case SIGALRM:
      STATE = ALARM;
      TICK = true;
      break;

    /* If SIGINT then stop alarm */
    case SIGINT:
      STATE = SLEEP;
      printf("CTRL+C pressed!\n");
      break;
    
    /* If SIGTSTP then close program.*/
    case SIGTSTP:
      printf("CTRL+Z pressed!\n"); 
      exit(1);
      break;
    
  }
}

// STATE
int main(void)
{
  // Register the signal handler
  if (signal(SIGALRM, alarm_handler) == SIG_ERR
      || signal(SIGINT, alarm_handler) == SIG_ERR
      || signal(SIGTSTP, alarm_handler) == SIG_ERR) {

    printf("failed to register alarm handler.\n");
    exit(1);
  }

  while (1){ 

    if ( STATE == ALARM && TICK == true ) {   // sounds alarm
      printf("Alarm!");
    }

    printf("Tick\n"); // ticks every second
    sleep(1);

    TICK = !TICK;
  } 
}
