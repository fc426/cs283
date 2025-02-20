1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We utilize fork/execvp instead of calling execvp directly becuse we would lost control over the shell ask the shell is replaced by the external command of execvp. Utilizing fork creates the process to call execvp. the process then can executeexternal commands while our shell runs simulatenously.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork system call fails it returns -1 due to it not being able to create a new process. In the implementation, I have an error message denoting the failure, and halting further executing of any command.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() finds the command in the directories laid out by the environment variable. If said command can not be found, execvp treis to find an executable file with the matching name.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait call pauses the parent process until the child processis done. Whatwould happen if we didn't call it is that the parent process would run without the child process, thus making the system less efficient.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATU() returns the valaue of the chuld proces from wait(). It tells us if the child process was executed properly. Its important becaue it lets the code know what to do next.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  In my implememntation, it was handeled by setting a boolean to true when a " is found via the parser. This is important because it can mess up the arguments but rathers fixes it all nto one.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  This was more so of a structure compared to a list of commands. A challenge was to make sure spacing didn't mess up any commands or arguments being passed. This can leave the shell or user with errors if not handeled properly.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in linux are to let it known whether a process has executed or anything specific with it. They differ from IPC because they transfer data in between processes, and they contain certain commands such as termination.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL: typical use case is forcing a process to terminate. Only when a process refuses to terminate. SIGTERM: Default termination signal. Handeled a lot better than SIGKILL and cleans up. SIGINT: Interrupts a process and can be used to abort a process.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, the process is paused. Debugger are able to halt the execution to fix any problems but the program itself can not stop it.
