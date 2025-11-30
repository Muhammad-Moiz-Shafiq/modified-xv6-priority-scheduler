#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char **argv)
{
  int orig = getpriority();
  printf("[edge] initial priority = %d\n", orig);

  int rc;

  rc = setpriority(PRIO_MIN - 1);
  printf("[edge] setpriority(%d) -> %d (expect -1)\n", PRIO_MIN - 1, rc);

  rc = setpriority(PRIO_MAX + 1);
  printf("[edge] setpriority(%d) -> %d (expect -1)\n", PRIO_MAX + 1, rc);

  int now = getpriority();
  printf("[edge] after invalid sets, priority still = %d (expect %d)\n", now, orig);

  rc = setpriority(PRIO_MAX);
  printf("[edge] setpriority(%d) -> %d (expect 0)\n", PRIO_MAX, rc);
  printf("[edge] getpriority() -> %d (expect %d)\n", getpriority(), PRIO_MAX);

  rc = setpriority(PRIO_MIN);
  printf("[edge] setpriority(%d) -> %d (expect 0)\n", PRIO_MIN, rc);
  printf("[edge] getpriority() -> %d (expect %d)\n", getpriority(), PRIO_MIN);

  exit(0);
}

