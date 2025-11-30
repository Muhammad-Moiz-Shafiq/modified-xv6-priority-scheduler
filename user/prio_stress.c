#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char **argv)
{
  int n = 50;
  if(argc == 2){
    n = atoi(argv[1]);
    if(n < 1) n = 1;
    if(n > 100) n = 100;
  }
  printf("[stress] creating %d children with varying priorities\n", n);
  int created = 0;
  for(int i = 0; i < n; i++){
    int pid = fork();
    if(pid < 0){
      printf("[stress] fork failed at %d\n", i);
      break;
    }
    if(pid == 0){
      int pr = (i % (PRIO_MAX + 1));
      if(setpriority(pr) < 0) {
        printf("[stress] setpriority(%d) failed\n", pr);
        exit(1);
      }
      // brief work
      for(volatile int k = 0; k < 50000; k++) {
        if((k % 20000) == 0) yield();
      }
      exit(0);
    }
    created++;
  }

  int done = 0;
  while(done < created){
    wait(0);
    done++;
    if((done % 10) == 0) {
      printf("[stress] completed %d\n", done);
    }
  }
  printf("[stress] all %d done\n", created);
  exit(0);
}

