1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() works well here because it waits for the user to press enter before processing input, making it more efficient than reading one character at a time. Plus, it automatically stops reading when the buffer is full, preventing buffer overflow issues.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: Using malloc() allows dynamic memory allocation, giving us more flexibility compared to a fixed size array. This is especially useful for handling varying command sizes without wasting stack space.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Leading and trailing spaces can cause commands to be misinterpreted, making " cmd1" different from "cmd1". Not trimming them could lead to unexpected errors when executing commands.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: One common redirection example is echo "I like vanilla" > output.txt, which redirects standard output to a file instead of displaying it on the terminal, and a challenge in implementing this is ensuring proper file creation and preventing unintended data loss. Another example is sort < text.txt, which takes the contents of text.txt as input for the sort command, requiring us to properly handle reading from files and dealing with missing or empty files. Lastly, echo "I like vanilla" >> output.txt appends the output instead of overwriting, meaning we must correctly open the file in append mode and check if it exists, creating it when necessary.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection changes where input or output comes from, like a file instead of the terminal. Piping  connects two commands so the output of one becomes the input of another.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping error messages separate makes it clear what the intended output is and what went wrong. It also ensures that errors don’t get mixed into pipelines, which typically only process standard output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our shell should keep stdout and stderr separate for clear feedback, but users should have the option to merge them for debugging by redirecting both to the same stream when needed.
