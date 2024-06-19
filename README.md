Implementing the game of Mastermind in C. 
These are the following rules that were implemented:
  - The individual digits of the code will be drawn from the hex digits: [0-9a-f]
  - There will be 6 digits per code
  - Duplicates are allowed
  - X's and O's are printed after each guess if it's the correct value and/or postion. X is for "correct value and position" and O is for "correct value wrong position"
  - Upper limit of 20 guesses per code.
  - Random numbers are selected using the mt19937.c code provided from my CS205 class
