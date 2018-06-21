/**
 *  Processes is a C++ program that recreates the output for a passed argv[1]
 *  of the unix call ps -A | grep argv[1] | wc -l
 *    
 *  Functionality:
 *  - Argv[1] is an process name whose number of occurances running we want 
 *  to display
 *  - Run with terminal call ./processes argv[1]
 *  - Returns number of processes running with name argv[1]
 *  
 *  Assumptions:
 *  - Processes.cpp may give a larger result than the unix call if ps -A in this 
 *  class counts the processes.cpp call as an extra running process
 *  - Input arguments are formatted properly to work with grep normally
 *  
 *  @file   processes.cpp
 *  @author <Robert Boyer> 
 *  @class  CSS430 Assignment 1A
 *  @date   <4/13/2016> 
 **/

#include <sys/types.h>
#include <sys/wait.h>   // for wait
#include <stdio.h>      // for printf
#include <stdlib.h>     // for exit
#include <unistd.h>     // for fork, execlp
#include <iostream>     // for cerr, cout
#define MAXSIZE 4096

using namespace std;

int main(int argc, const char * argv[])
{
   enum {RD, WR}; // pipe fd index RD=0, WR=1
   int n;
   char buff[MAXSIZE];
   int fd[2], fd1[2], fd2[2]; //file discriptors for 3 pipes
   pid_t pid; // child process id

   if (argc < 2) {
	   cerr << "not enough arguments" << endl;
	   exit(EXIT_FAILURE);
   }
   if (pipe(fd) < 0) { //create pipe 1
	   cerr << "pipe 1 error" << endl;
	   exit(EXIT_FAILURE);
   }
   if (pipe(fd1) < 0) { //create pipe 2
	   cerr << "pipe 2 error" << endl;
	   exit(EXIT_FAILURE);
   }
   if (pipe(fd2) < 0) { //create pipe 3
	   cerr << "pipe 3 error" << endl;
	   exit(EXIT_FAILURE);
   }

   pid = fork();

   if (pid < 0) {
	   cerr << "fork error" << endl; //failed to fork
	   exit(EXIT_FAILURE);
   }
   else if (pid == 0) {                                                
	   
	   pid = fork(); //fork to child

	   if (pid < 0) {
		   cerr << "fork error" << endl; //failed to fork
		   exit(EXIT_FAILURE);
	   }
	   else if (pid == 0) {

		   pid = fork(); //fork to grand child

		   if (pid < 0) {
			   cerr << "fork error" << endl; //failed to fork
			   exit(EXIT_FAILURE);
		   }

		   if (pid == 0) {                                                     //Great-Grand-Child
			   pid = fork(); //fork to great grandchild
	
			   if (pid < 0) {
				   cerr << "fork error" << endl; //failed to fork
				   exit(EXIT_FAILURE);
			   }
			   else if (pid == 0) {
				   // cout << "Starting Great-Grand-Child" << endl;

				   // close unused pipes
				   close(fd2[RD]);//close grandchilds RD
				   close(fd1[RD]);  close(fd1[WR]); //close childs RD, close grandchilds WR
				   close(fd[RD]);   close(fd[WR]); //close parents RD, close childs WR

				   // connect pipes
				   dup2(fd2[WR], 1); // stdout --> greatgrandchild's pipe write

				   execlp("/bin/ps", "ps", "-A", NULL);
				   //cout << "Great-Grand-Child Done!" << endl;
			   }
			   else {                                                          //Grand-Child
				   //cout << "Starting Grand-Child" << endl;
				   
				   // close unused pipes
				   close(fd[RD]); close(fd[WR]); //close parents RD, close child's WR
				   close(fd1[RD]); //close childs RD
				   close(fd2[WR]); //close greatgrandchild's WR

				   // connect pipes
				   dup2(fd2[RD], 0); // stdin --> grandchild's pipe read
				   dup2(fd1[WR], 1); // stdout --> grandchild's pipe write

				   execlp("/bin/grep", "grep", argv[1], NULL);
				   wait(NULL); 
				   //cout << "Grand-Child Done!" << endl;
			   }

		   }
		   else {                                                              //Child
			   //cout << "Starting Child" << endl;
			   
			   //close  unused pipes
			   close(fd[RD]); //close parent's RD
			   close(fd1[WR]); //close grandchild's WR
			   close(fd2[RD]); close(fd2[WR]); //close grandchild's RD, close greatgrandchild's WR

			   //connect pipes
			   dup2(fd1[RD], 0); //stdin --> childs pipe read
			   dup2(fd[WR], 1); // stdout --> childs pipe write
			   execlp("/usr/bin/wc", "wc", "-l", NULL);
			   wait(NULL); // WAIT HERE 
			   //cout << "Child Done!" << endl;
		   }
	   }
   }
   else {                                                                      //Parent
	   //cout << "Starting Parent" << endl;

	   //close unused pipes 
	   close(fd1[RD]); close(fd1[WR]); //close child's RD, close grandchild's WR
	   close(fd2[RD]); close(fd2[WR]); //close grandchild's RD, close great grandchild's WR

	   //connect pipes
	   close(fd[WR]); // close child's WR end of pipe & connect pipes
	   dup2(fd[RD], 0); // stin --> parent's pipe read
	   wait(NULL);

	   //char buf[MAXSIZE];
	   n = read(fd[RD], buff, MAXSIZE); // use this raw read!
	   buff[n] = '\0'; // make sure cstring is terminated
	   cout << buff; // write to stout
	   //cout << "Parent Done!" << endl;
	   exit(EXIT_SUCCESS);
   }

   exit(EXIT_SUCCESS);
}


