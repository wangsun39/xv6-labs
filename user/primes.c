#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int rd)
{
  int sp[2];
  int val;
  read(rd, &val, 4);
  int prm=val;
  printf("prime %d\n",val);
  if (val>=31)
  {
    close(rd);
    exit(0);
  }
  pipe(sp);
  if (fork()!=0)
  {
    close(sp[0]);
    while(read(rd, &val, 4))
    {
      if (val%prm==0)
      {
        continue;
      }

      write(sp[1],&val,4);
    }
    close(sp[1]);
    close(rd);
    wait(0);
    // exit(0);
  }
  else
  {
    close(sp[1]);
    prime(sp[0]);
    // exit(0);
  }
}

int main(int argc, char *argv[])
{

  int mp[2];
  pipe(mp);
  if (fork() != 0)
  {
    // parent
    close(mp[0]);
    for (int i=2;i<36;i++)
    {
      write(mp[1],&i,4);
    }
    close(mp[1]);
    wait(0);
  }
  else
  {
    close(mp[1]);
    prime(mp[0]);
  }

  exit(0);
}


