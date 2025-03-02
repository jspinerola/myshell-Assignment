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
Updated by Julian Spindola, March 1st, 2025. (jspindola@tamusa.edu)
Tasks-
Fork a child process and execute command specified by user. Parse user input into tokens and stor them in an array of character strings.

Pass such args into execvp(char *command, char *params[]). Ex. execvp(args[0], args)

Check for whether & was included in user input, and determine if parent must invoke wait()

if user enters !!, execute most recent command. Achieved with a buffer. Command is echoed onto screen, then placed into buffer. If nothing is in buffer, print "No commands in history."
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAXARGS 20 /* max number of cmdline args ? */

#define ARGLEN 100 /* token lengths per argument*/

#define CMDPROMPT "Command please: "

#define ARGPROMPT "next argument please: "

#define prompt(n) printf("%s", (n) == 0 ? CMDPROMPT : ARGPROMPT);

//Added flag variable 'runConcurrently' to execute() function
void execute(char *arglist[], int runConcurrently);
char *makestring(char *buf);

int main()
{
  char *arglist[MAXARGS + 1]; /* an array of pointers */
  int numargs;                /* the index of the array */
  char argbuf[ARGLEN];        /* read stuff here */
  int willRunConcurrently; //New flag variable, 'runConcurrently', to indicate whether wait() will be invoked
  char *commandbuffer[MAXARGS + 1] = {NULL}; //New pointer arrayc 'commandbuffer', to store previous command. 
  // char *makestring();

  numargs = 0;
  willRunConcurrently = 0; //Initialize flag variable to 0, indicating that the program will still invoke wait() and NOT run concurrently. Default Behavior.
  while (numargs < MAXARGS)
  {
    prompt(numargs); /* prompt user for arg */

    if (gets(argbuf) && *argbuf) /* if arg, add to the list */
    {
      arglist[numargs++] = makestring(argbuf);
    }
    else /* exit or execute cmd */
    {
      /****** Logic for Task 2 *****/
      /*If user adds an ampersand to the end of their command, the parent and child processes will run concurrently. */
      if (numargs > 0 && strcmp(arglist[numargs - 1], "&") == 0) // If user enters & to end of command...
      {
        willRunConcurrently = 1; //Set run concurrently flag to 1, indicating wait will NOT be evoked
        arglist[numargs - 1] = NULL;
        numargs--;
      }
      else
      {
        willRunConcurrently = 0; //If not, reset flag to 0
      }

      if (numargs == 0) /* no command? */
        break;          /* y: break */

      arglist[numargs] = NULL; /* add NULL to the list to close the list*/

      // Copy command into buffer

      /***** Logic for Task 3 *****/
      /*If user enters '!!', run and echo the previous command entered. */
      if (numargs == 1 && strcmp(arglist[0], "!!") == 0)
      { //If user enters !!...
        printf("Previous Command Executed: ");
        for (int i = 0; commandbuffer[i] != NULL; i++) {
          printf("%s ", commandbuffer[i]);
        }
        printf("\n"); //Print previous command
        if (commandbuffer[0] == NULL)
        {
          printf("No commands in history.\n"); //If there is nothing in buffer, print the following. 
        }
        else
        {
          execute(commandbuffer, willRunConcurrently); /* Execute previous command */
        }
      }
      else
      {
        //If user doens't enter !!
        memcpy(commandbuffer, arglist, sizeof(arglist));
        //Store previous command to buffer

        execute(arglist, willRunConcurrently); /* Execute Command */
      }

      numargs = 0; /* reset the index */
      willRunConcurrently = 0; /* Reset concurrency*/
    }
  }
  return 0;
}

/***** Logic for Task 1 *****/
// Ensure command runs as a child process and parent waits for process to complete UNLESS flag runConcurrently is true.
void execute(char *arglist[], int runConcurrently)
{
  //Create child process by evoking fork()
  pid_t childProcessID = fork();

  if (childProcessID < 0)
  {
    printf("Fork Failed :(\n");
  }
  else if (childProcessID == 0)
  {
    // Execute command if fork is created
    execvp(arglist[0], arglist);
    perror("execvp failed");
    exit(1);
  }
  else
  {
    if (runConcurrently != 0)
    {
      //If runConcurrently flag is active, early return function and do NOT evoke wait()
      return;
    }
    // Evoke wait(), meaning parent process will wait for child process. 
    wait(NULL); 
  }
  return;
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
