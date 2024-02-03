#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

void EXEC(char cmd[], char *argv[])
{
  int pid = fork();
  if (pid > 0)
  {
    // printf("parent: child=%d\n", pid);
    pid = wait((int *)0);
    // printf("child %d is done\n", pid);
  }
  else
  {
    // printf("pid:%d, cmd: %s, v:%s, %s, p:%p,%p\n", pid, cmd, argv[0], argv[1], &argv[0], &argv[1]);
    exec(cmd, argv);
    exit(0);
  }
}
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(2, "numbers of parameter: %d < 2\n", argc);
    exit(0);
  }

  char *params[MAXARG] = {0};
  for (int i = 1; i < argc;i++)
  {
    params[i - 1] = argv[i];
  }
  char cmd[128] = {0};
  strcpy(cmd, argv[1]);
  
  for (;;)
  {
    char para[128] = {0};
    int p = 0;
    int n;
    for (;;)
    {
      char c;
      n = read(0, &c, 1);
      if (n < 0)
      {
        fprintf(2, "read error\n");
        exit(1);
      }
      if (n == 0 || c == '\n')
        break;
      para[p++] = c;
    }
    if (n == 0)
    {
      break;
    }
    para[p] = 0;
    params[argc - 1] = para;
    EXEC(cmd, params);
  }

  exit(0);
}

// #define STD_IN 0
// #define STD_OUT 1
// #define STD_ERR 2
// #define BUFFER_SIZE 512

// char *ugets(char *buf, int max) {
//     char c;
//     int i;
//     for (i = 0; i + 1 < max;) {
//         int cc = read(0, &c, 1);
//         if (cc < 1)
//             break;
//         if (c == ' ' || c == '\n' || c == '\r')
//             break;
//         buf[i ++] = c; // 不读入 ' ' 和 '\n'
//     }
//     buf[i] = '\0';
//     return buf;
// }

// int ugetcmd(char *buf, int nbuf) {
//     memset(buf, 0, nbuf);
//     ugets(buf, nbuf);
//     if (buf[0] == 0) // EOF
//         return -1;
//     return 0;
// }

// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         fprintf(STD_ERR, "Usage: xargs command\n");
//         exit(1);
//     }
//     char *_argv[MAXARG];
//     int _argc = argc - 1;
//     for(int i = 0; i < _argc; i ++) {
//         _argv[i] = argv[i + 1];
//     }
//     char buf[BUFFER_SIZE];
//     while(ugetcmd(buf, sizeof(buf)) != -1) {
//         if(fork() == 0) {
//             _argv[_argc] = buf;
//             _argc ++; // 在父进程中保留所有的输入参数, 可以删掉
//             exec(argv[1], _argv);
//             fprintf(STD_ERR, "exec %s failed\n", argv[1]); // 执行失败
//             exit(0);
//         }
//         wait(0);
//     }
//     exit(0);
// }
