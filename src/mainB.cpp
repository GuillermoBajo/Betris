#include <iostream>
#include "betris.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    tpBoard board;
    int inputArray[MAXINPUT] = {};
    int target;
    int delay = 0;
    int outputArray[MAXINPUT] = {};

    // Process the user input data
    inputData(argc, argv, target, delay, inputArray, board);

    // Initialize the base game board (all squares of the board empty)
    initializeBoard(board);

    // The first call to searchSolution is made with n = 0 (with the first input piece)
    int n = 0;
    int x = searchSolution(board, inputArray, outputArray, target, n, delay);

    // If the solution has been found, we display the board as part of the output data
    system("clear");
    if (isSolution(board, target))
    {
        displayBoard(board, inputArray);
    }

    // Display the game results
    outputData(inputArray, outputArray, x, target);
}
