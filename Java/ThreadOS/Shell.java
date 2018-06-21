import java.io.*;
import java.util.*;

class Shell extends Thread
{
   //command line string to contain the full command
   private String cmdLine;

   // constructor for shell
   public Shell( ) {
      cmdLine = "";
   }

   // required run method for this Shell Thread
   public void run( ) {

      int count = 1;//counter from 1 to n

      while(true)
      {
         SysLib.cout("Shell[" + count + "]% "); //Prompt for output from user

         StringBuffer input = new StringBuffer(); //Create string buffer for input
         SysLib.cin(input); //store input into string buffer


         if(input.toString().equals("exit")) {//Break loop if command equals exit
            SysLib.exit();
            break;
         }
         else if(input.toString().equals("")) {//Continue loop if command equals "nothing"
            continue;
         }

         String [] semiColonArgs = input.toString().split(";"); //break up input by ';' into array slots

         for(int i = 0; i < semiColonArgs.length; i++) { //run for number of iterations equal to colon array length

            String [] ampersandArgs = semiColonArgs[i].split("&"); //break up semicolon strings by '&' into 2nd array
            int numberOfArgs = ampersandArgs.length; //total arguments

            for(int j = 0; j < ampersandArgs.length; j++){ //run for number of iterations equal to amper array length

               String[] args = SysLib.stringToArgs(ampersandArgs[j]); // array stores strings converted to arguments
               int tid = SysLib.exec(args);
               if(tid < 0){
                  numberOfArgs--;
                  continue;
               }
               else{
                  SysLib.cout("Started Thread tid=" + tid + "\n"); // outputs tid
               }
            }
            
            for(int j = 0; j < numberOfArgs; j++)
               SysLib.join();
         }

         count++;//increase counter
      }
   }
}
