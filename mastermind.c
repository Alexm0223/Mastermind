#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/* Alex Martinez-Lopez, CS205, Final Project, alemart2@pdx.edu*/

//Numbers is drawn from the mt19937 code 
#include "mt19937.c"

#define CODE_LENGTH 6   //Length of the code to guess
#define MAX_GUESSES 20  //Maximum number of guesses allowed

// Function to generate a random code
void generate_code(char *code)
{
    static int initialized = 0;
    if(!initialized)
    {
        init_genrand(time(NULL));   //Seed the mt19937 generator with the current time
        initialized = 1;
    }
    // This will generate a random hexadecimal code of length CODE_LENGTH(6)
    for (int i = 0; i < CODE_LENGTH; ++i)
    {
        code[i] = "0123456789abcdef"[genrand_int32() % 16];
    }
    code[CODE_LENGTH] = '\0'; //Null-terminate the string
}

// Function to evaluate the user's guess against the code
void evaluate_guess(const char code[], const char guess[], char feedback[])
{
    int correct_position = 0, correct_value = 0;
    int code_count[16] = {0};
    int guess_count[16] = {0};

    //count the exact matches and track the occurrences of each digit
    for(int i = 0; i < CODE_LENGTH; i++)
    {
        if(code[i] == guess[i])
        {
            correct_position++; // Increment correct positions
        }
        else
        {
            // Determine the index for the digit in the code
            int code_digit_index;
            if (code[i] > '9') 
            {
                // If the character is a letter (a-f), convert it to an index starting from 10
                code_digit_index = code[i] - 'a' + 10;
            } 
            else 
            {
                // If the character is a number (0-9), convert it to an index starting from 0
                code_digit_index = code[i] - '0';
            }

            // Determine the index for the digit in the guess
            int guess_digit_index;
            if (guess[i] > '9') 
            {
                // If the character is a letter (a-f), convert it to an index starting from 10
                guess_digit_index = guess[i] - 'a' + 10;
            } 
            else   
            {
                 // If the character is a number (0-9), convert it to an index starting from 0
                 guess_digit_index = guess[i] - '0';
            }

            code_count[code_digit_index]++;   // Count occurrences of digits in the code
            guess_count[guess_digit_index]++;  // Count occurrences of digits in the guess
        }
        
    }

  // Count correct values in wrong positions
    for (int i = 0; i < 16; i++) {
        correct_value += (code_count[i] < guess_count[i]) ? code_count[i] : guess_count[i];
    }
    
    // Build feedback string
    for (int i = 0; i < correct_position; i++) 
    {
        feedback[i] = 'X';  // Mark correct positions with 'X'
    }
    for (int i = 0; i < correct_value; i++)
     {
        feedback[correct_position + i] = 'O';   // Mark correct values in wrong positions with 'O'
    }
    feedback[correct_position + correct_value] = '\0';  // Null-terminate the feedback string
}

int main()
{
    char code[CODE_LENGTH + 1];     //Array to store the code
    char guess[CODE_LENGTH + 1];    //Array to store the user's guess
    char feedback[CODE_LENGTH + 1];

    //generate the random code
    generate_code(code);

    //welcome message and instructions
    printf("Welcome to Mastermind! Guess the %d-digit code made up of hexadecimal digits (0-9, a-f).\n", CODE_LENGTH);
    printf("For each guess, you'll get feedback:\n");
    printf("X = correct digit in correct position\n");
    printf("O = correct digit in wrong position\n\n");

    //loop for each guess attempt
    for (int tries = 0; tries < MAX_GUESSES; tries++)
    {
        printf("Enter your guess (e.g. 1a2b3c): ");
        
        if(scanf("%6s", guess) != 1 || strlen(guess) != CODE_LENGTH)    // Read in the user's guess
        {
            printf("Invalid input. Please try again.\n");
            while(getchar() != '\n');   //clear the input buffer
            tries--;    //decrement try counter as it was invalid input
            continue;
        }

        //check if there's any extra input (more than 6 characters)
        int extra_char = getchar();
        if (extra_char != '\n' && extra_char != EOF)
        {
            printf("Invalid input. Please enter exactly %d hexadecimal digits.\n", CODE_LENGTH);
            while (extra_char != '\n' && extra_char != EOF)
            {
                extra_char = getchar();  // Clear the rest of the buffer
            }
            tries--;  // Decrement the try counter as it was invalid input
            continue;
        } 

        //check if the guess only contains hexadecimal digits;
        int valid = 1;
        for (int j = 0; j < CODE_LENGTH; j++)
        {
            if(!isxdigit(guess[j]))
            {
                valid = 0;
                break;
            }
        }

        if(!valid)
        {
            printf("Invalid guess. Please enter only hexadecimal digits.\n");
            continue;
        }

        evaluate_guess(code, guess, feedback);  // Evaluate the guess and generate feedback
        printf("Feedback: %s\n\n", feedback);
        
        // Check if the guess is correct
        if (strcmp(guess, code) == 0) {
            printf("Congratulations! You guessed the code: %s\n", code);
            return 0;
        }

    }

    //Print out message if the user fails to guess the code within the 20 allowed attempts
    printf("You have used all %d attemps. The code was: %s\n", MAX_GUESSES, code);
    return 0;   //exit the game because there are no more attempts
}