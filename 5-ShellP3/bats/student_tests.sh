#!/usr/bin/env bats

# File: student_tests.sh
#
# Create your unit tests suit in this file



@test "empty input- command parsing" {
    run "./dsh" <<EOF
     
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="warning:nocommandsprovideddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "quote handling including paremeters and spaces." {
    run "./dsh" <<EOF
echo "hello world"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "quote handling of redirection" {
    run "./dsh" <<EOF
echo "hello > world"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hello>worlddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "input redirection works" {
    echo "Hello World" > $HOME/tmp/re_in.txt

    run "./dsh" <<EOF
cat < $HOME/tmp/re_in.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="HelloWorlddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "input file missing" {
    run "./dsh" <<EOF
cat <
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="redirect:missinginputfileforredirectionerrorparsingcommandlinedsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "numerous input directions" {
    echo "Content1" > $HOME/tmp/re_in.txt
    echo "Content2" > $HOME/tmp/re_in2.txt

    run "./dsh" <<EOF
cat < $HOME/tmp/re_in.txt < $HOME/tmp/re_in2.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="redirect:multipleinputredirectionoperatorserrorparsingcommandlinedsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}



@test "numerous output redirection" {
    run "./dsh" <<EOF
echo "Hello" > $HOME/tmp/re_out.txt > $HOME/tmp/another.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="redirect:multipleoutputredirectionoperatorserrorparsingcommandlinedsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "input file does not exist" {
    rm -f $HOME/tmp/nonexistent.txt

    run "./dsh" <<EOF
cat < $HOME/tmp/nonexistent.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="openinputfile:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "redirection no commands" {
    run "./dsh" <<EOF
< re_input.txt | pwd | > re_output.txt
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="warning:nocommandsprovideddsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "single command exec - pipe test" {
    run "./dsh" <<EOF
echo "HelloSingle"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="HelloSingledsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipeline consisting of two commands" {
    run "./dsh" <<EOF
echo "hello.c world" | grep ".c"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="hello.cworlddsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "pipeline consisting of 3 commands" {
    run "./dsh" <<EOF
printf "3\n2\n1\n" | cat | sort
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="123dsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "pipeline consisting of the CMD_MAX commands" {
    run "./dsh" <<EOF
echo 1 | cat | cat | cat | cat | cat | cat | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="1dsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


@test "command failure" {
    run "./dsh" <<EOF
nonexistent_command | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="execvp:Nosuchfileordirectorydsh3>dsh3>dsh3>cmdloopreturned0"
    
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

