#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

static void run_child(int prio, const char *name) {
  if(setpriority(prio) < 0){
    printf("child %s: setpriority(%d) failed\n", name, prio);
    exit(1);
  }
  int gp = getpriority();
  printf("[basic] child %s pid=%d prio=%d start\n", name, getpid(), gp);
  // Short CPU work with occasional yields to give scheduler chances.
  for(volatile int i = 0; i < 200000; i++) {
    if((i % 50000) == 0) {
      yield();
    }
  }
  printf("[basic] child %s pid=%d prio=%d done\n", name, gp, getpid());
  exit(0);
}

int
main(int argc, char **argv)
{
  int pids[3];
  const char *names[3] = { "LOW", "MID", "HIGH" };
  int prios[3] = { PRIO_MIN, PRIO_DEFAULT, PRIO_MAX };

  // spawn HIGH last to ensure scheduler ordering determines who runs first
  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0){
      run_child(prios[i], names[i]);
    }
    pids[i] = pid;
  }

  // collect completion order
  int order[3];
  for(int i = 0; i < 3; i++){
    int st = 0;
    int pid = wait(&st);
    order[i] = pid;
  }

  printf("[basic] completion order (pid): %d -> %d -> %d\n",
         order[0], order[1], order[2]);
  printf("[basic] expected HIGH finishes first, then MID, then LOW\n");
  exit(0);
}

