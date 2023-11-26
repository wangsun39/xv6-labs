#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);
  int buf[10] = {0};
  if (fork() == 0)
  {
  close(p2[0]);
  close(p1[1]);
  // 引用管道读端，故改变了标准输入。
  read(p1[0], buf, 1);
  printf("%d: received ping\n", getpid());
  write(p2[1], "b", 1);
  exit(0);

  // 一个文件
  }
  else
  {
  close(p1[0]);
  close(p2[1]);
  write(p1[1], "a", 1);
  read(p2[0], buf, 1);
  printf("%d: received pong\n", getpid());
  exit(0);
  }

  return 0;
}


