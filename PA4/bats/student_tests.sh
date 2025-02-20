#!/usr/bin/env bats

@test "Change directory without permission fixed" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test-hx84

    run "${current}/dsh" <<EOF                
cd dsh-test-hx84
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="/tmp/dsh-test-hx84dsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "non existent directory cd" {
    current=$(pwd)

    cd /tmp

    run "${current}/dsh" <<EOF
cd /nonexistent_directory
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="cdfailed:Nosuchfileordirectory/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    [ "$status" -eq 0 ]
}

@test "no permision for cding into directory" {
    current=$(pwd)

    cd /tmp
    mkdir -p no_perm_dir
    chmod 000 no_perm_dir

    run "${current}/dsh" <<EOF
cd no_perm_dir
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="cdfailed:Permissiondenied/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "too many args cd" {
    current=$(pwd)

    cd /tmp

    run "${current}/dsh" <<EOF
cd dir1 dir2
pwd
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="cd:toomanyarguments/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}



@test "Empty input handeling" {
    current=$(pwd)

    run "${current}/dsh" <<EOF
       
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="dsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "leading/trailing invisible spaces" {
    current=$(pwd)

    run "${current}/dsh" <<EOF
   echo spaced   
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="spaceddsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Multiple spaces between args handeled to one" {
    current=$(pwd)
    
    run "${current}/dsh" <<EOF
echo a    b   c
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    expected_output="abcdsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

