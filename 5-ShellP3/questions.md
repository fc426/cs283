1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?
The shell collects IDs of chld processes of the pipepline and places them in array. Then, it forkss all the children in which the parents then enters the loop of waitpid(). This implementations ensures that the shell is waiting for each of the processes before allowinng for a new prompt. Without waitpid the shell would not wait for the process therefore causing errors.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

Its imperative to close all loose ends so all signals of EOF are properly transmitted. If any pipes are left open it could lead instability and deadlocks composed within the shell.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is a built in because it has to change the directory in order to process. If CD was implemented as an external process it would be difficult, the design would be difficult, and just wouldn't be useful. Rather, implemented as a built in so it can change the current working directory of its own shell processs.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To modify the array, I would have expanded a lot of the structures within the code. Using different data structures to manage memory to make sure everything is allocated properly is something I would have implemented. However, the tradeoffs would include difficult design as well making sure all resources are probably allocated. All this would need to be dude with realloc(), and other data stuctures.
