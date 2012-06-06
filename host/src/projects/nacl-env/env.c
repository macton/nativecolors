#include <locale.h>
#include <stdio.h>

// TODO:
// ENV(1)    
// 
// NAME
//       env - run a program in a modified environment
//
// SYNOPSIS
//       env [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...]
//
// DESCRIPTION
//       Set each NAME to VALUE in the environment and run COMMAND.
//
//       -i, --ignore-environment
//              start with an empty environment
//
//        -0, --null
//               end each output line with 0 byte rather than newline
// 
//        -u, --unset=NAME
//               remove variable from the environment
// 
//        --help display this help and exit
// 
//        --version
//               output version information and exit
// 
//        A mere - implies -i.  If no COMMAND, print the resulting environment.
// 

int
main(int argc, char *argv[])
{
  extern char** environ;
  char**        ep;

  setlocale(LC_ALL, "");

  for (ep = environ; *ep; ep++)
  {
    (void)printf("%s\n", *ep);
  }

  return (0);
}
