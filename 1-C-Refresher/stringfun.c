#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    if (!buff || !user_str) {
        // Null pointer provided
        return -2; // Return -2 to indicate an error condition
    }

    char *buff_ptr = buff; // Pointer to traverse the buffer
    char *str_ptr = user_str; // Pointer to traverse the input string
    char prev_char = '\0'; // To track the previous character for handling consecutive whitespace
    int user_len = 0; // Track the length of the user-supplied string (excluding dots)

    // Traverse the user input string and copy non-whitespace characters to the buffer
    while (*str_ptr != '\0' && user_len < len) {
        if (*str_ptr == ' ' || *str_ptr == '\t') {
            // If it's a whitespace character
            if (prev_char != ' ') {
                // Replace consecutive whitespace with a single space
                *buff_ptr = ' ';
                buff_ptr++;
                user_len++;
                prev_char = ' '; // Set previous character to space
            }
        } else {
            // Non-whitespace character, copy it to the buffer
            *buff_ptr = *str_ptr;
            buff_ptr++;
            user_len++;
            prev_char = *str_ptr; // Set previous character to the current non-whitespace character
        }

        str_ptr++;
    }

    // Check if the input string is too large to fit in the buffer
    if (*str_ptr != '\0') {
        fprintf(stderr, "error: Provided input string is too long\n");
        return -1; // Return -1 to indicate the string is too large
    }

    // Fill the remainder of the buffer with '.' characters
    while (user_len < len) {
        *buff_ptr = '.';
        buff_ptr++;
        user_len++;
    }

    // Return the length of the user-supplied string (excluding padding dots)
    return user_len;
}
    

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}


void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff,int len, int str_len){
if (!buff) {
        // Error: Null pointer provided
        fprintf(stderr, "error: Null buffer pointer\n");
        return -1;
    }

    int word_count = 0;
    int in_word = 0; // Tracks if currently inside a word

    // Iterate over the buffer up to 'len' bytes (ensure we don't exceed buffer size)
    for (int i = 0; i < len && i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '.') {
            if (!in_word) {
                // Start of a new word
                in_word = 1;
                word_count++;
            }
        } else {
            // End of a word
            in_word = 0;
        }
    }

    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int str_len) {
    if (!buff) {
        // Error: Null pointer provided
        fprintf(stderr, "error: Null buffer pointer\n");
        return;
    }

    printf("Reversed String: ");
    for (int i = str_len - 1; i >= 0; i--) {
        putchar(buff[i]);
    }
    putchar('\n');
}

void word_print(char *buff, int str_len) {
    if (!buff) {
        // Error: Null pointer provided
        fprintf(stderr, "error: Null buffer pointer\n");
        return;
    }

    printf("Word Print\n----------\n");

    int word_index = 1;
    int char_count = 0; // Track the number of characters in the current word
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (buff[i] != ' ' && buff[i] != '.') {
            if (!in_word) {
                // Start of a new word
                if (word_index > 1) {
                    // Print the character count of the previous word
                    printf(" (%d)\n", char_count);
                }
                printf("%d. ", word_index);
                word_index++;
                char_count = 0; // Reset character count for the new word
                in_word = 1;
            }
            putchar(buff[i]);
            char_count++;
        } else {
            if (in_word) {
                // End of a word
                in_word = 0;
            }
        }
    }

    // Print the character count for the last word
    if (char_count > 0) {
        printf(" (%d)\n", char_count);
    }
}
int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
	If arv[1] does not exist, it makes sur the option starts wth a -, verifying that the option
	selected is in the correct format. Makes sure the correct flag is uesed, thats why it is safe.

    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
	This checks if the user provided a string for input after only providing  flag. This makes sure the correct arguments are provided to prevent errors.
	*/
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));  // Allocate memory for 50 bytes
    if (!buff) {
    	fprintf(stderr, "error: Memory allocation failed\n");
    	exit(99); //exit with code 99 if malloc does not work
	}
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r': // Reverse string
        reverse_string(buff, user_str_len);  // Implemented reverse_string
        break;

        case 'w': // Word print
        word_print(buff, user_str_len);  // Implemented word_print
        break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//      
/*
This is good practice to provide both the pointer and length because it allows us to chnge the function later on and to
work with buffers of different sizs. Furthermore, it keeps every consistent with less chances of memory problems. 
It overall makes things much clearer.
*/
