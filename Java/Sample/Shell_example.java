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

      // build a simple command that invokes PingPong 
      cmdLine = "PingPong abc 100";

      // must have an array of arguments to pass to exec()
      String[] args = SysLib.stringToArgs(cmdLine);
      SysLib.cout("Testing PingPong\n");

      // run the command
      int tid = SysLib.exec( args );
      SysLib.cout("Started Thread tid=" + tid + "\n");

      // wait for completion then exit back to ThreadOS
      SysLib.join();
      SysLib.cout("Done!\n");
      SysLib.exit();
   }
}
