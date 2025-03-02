/*
* GB.Tony Liang, Feb 5, 2022 (gliang@tamusa)
* For CSCI 3326 @ TAMUSA
*
* This program simulates a UNIX shell for one iteration (i.e., one command).
* After a command is entered, execvp() executes the command in the same
process.
* After the command execution, the process terminated naturally.
*
*/
/*
Tasks-
Fork a child process and execute command specified by user. Parse user input into tokens and stor them in an array of character strings.

Pass such args into execvp(char *command, char *params[]). Ex. execvp(args[0], args)

Check for whether & was included in user input, and determine if parent must invoke wait()

if user enters !!, execute most recent command. Achieved with a buffer. Command is echoed onto screen, then placed into buffer. If nothing is in buffer, print "No commands in history."

Approach -



*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#define MAXARGS 20 /* max number of cmdline args ? */

#define ARGLEN 100 /* token lengths per argument*/

#define CMDPROMPT "Command please: "

#define ARGPROMPT "next argument please: "

#define prompt(n) printf("%s", (n) == 0 ? CMDPROMPT : ARGPROMPT);

void execute(char *arglist[]);
char *makestring(char *buf);

int main()
{
  char *arglist[MAXARGS + 1]; /* an array of pointers */
  int numargs;                /* the index of the array */
  char argbuf[ARGLEN];        /* read stuff here */
  // char *makestring();

  numargs = 0;
  while (numargs < MAXARGS)
  {
    prompt(numargs); /* prompt user for arg */

    if (gets(argbuf) && *argbuf) /* if arg, add to the list */
    {
      arglist[numargs++] = makestring(argbuf);
    }
    else /* exit or execute cmd */
    {
      if (numargs == 0) /* no command? */
        break;          /* y: break */

      arglist[numargs] = NULL; /* add NULL to the list to close the list*/

      execute(arglist); /* execute the list */

      numargs = 0; /* reset the index */
    }
  }
  return 0;
}

// Execution Logic!!
void execute(char *arglist[])
{
  execvp(arglist[0], arglist);
  perror("execvp failed");
  exit(1);
}

// Makes string for args ??
char *makestring(char *buf)
{
  // char *cp, *malloc();
  char *cp;
  if ((cp = malloc(strlen(buf) + 1)))
  { /* get memory */

    strcpy(cp, buf); /* copy chars */

    return cp; /* return pointer*/
  }
  fprintf(stderr, "out of memory\n");
  exit(1);
}
