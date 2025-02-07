#include <iostream>
#include <unistd.h>
#include <time.h>
#include <random>
#include <ctime>
#include <cstdlib>
#include "betris.hpp"

using namespace std;

// Pre: delay is the time pause to make after displaying the state of the board.
//      displayBoard shows the board in its current state
// Post: If delay > 0, the board has been displayed with the corresponding delay. Otherwise, it's simply displayed
void displayWithDelay(const int delay, tpBoard &board, const int inputArray[MAXINPUT])
{

    if (delay > 0)
    {
        displayBoard(board, inputArray);
        usleep(delay * 1000);
    }
    // If delay <= 0, we ignore it
    else
    {
        displayBoard(board, inputArray);
    }
}

// Pre: 0 <= n <= MAXINPUT
// Post: A random sequence of size -n pieces has been generated and stored in inputArray
void generateRandomSequence(int &n, int inputArray[MAXINPUT])
{

    n = -n;
    time_t seed = time(nullptr);
    srand(seed);

    int i = 0;
    // Assign random pieces to inputArray
    while (i < n)
    {
        inputArray[i] = 0 + rand() % (PIECESDEF);
        i++;
    }
    // Set the end of the pieces
    inputArray[i] = -1;
}

// Pre: True
// Post: User input data from terminal has been stored in respective variables
void inputData(int argc, char *argv[], int &target, int &delay, int inputArray[MAXINPUT], tpBoard &board)
{

    // We ignore argv[0] as it contains irrelevant information
    board.nRows = atoi(argv[1]);
    board.nCols = atoi(argv[2]);
    target = atoi(argv[3]);
    delay = atoi(argv[4]);
    int pieceIndex = 0;

    // If N1 <0, generate a random sequence of -N1 pieces
    if (atoi(argv[5]) < 0)
    {
        int p = atoi(argv[5]);
        generateRandomSequence(p, inputArray);
    }
    else
    {
        int i = 5;
        // Read the entered pieces and assign them to inputArray
        while (i < argc - 1)
        {
            inputArray[pieceIndex] = atoi(argv[i]);
            pieceIndex++;
            i++;
        }
        // Set the end of the pieces
        inputArray[pieceIndex] = -1;
    }
}

// Pre: True
// Post: All components of board.matrix are -1 (empty square)
void initializeBoard(tpBoard &board)
{
    for (int i = 0; i < board.nRows; i++)
    {
        for (int j = 0; j < board.nCols; j++)
        {
            board.matrix[i][j] = -1;
        }
    }
}

// Pre: inputArray contains the numbers of the pieces that enter, and board contains the current state
//      of the board, where some of those pieces might be placed
// Post: The board has been displayed on screen (each piece drawn with its color)
void displayBoard(const tpBoard &board, const int inputArray[MAXINPUT])
{
    system("clear");
    int pieceColor;
    int n;
    for (int i = 0; i < board.nRows; i++)
    {
        for (int j = 0; j < board.nCols; j++)
        {

            // If the square is empty, we don't paint it
            if (board.matrix[i][j] == -1)
            {
                cout << " ";
            }
            // If it has a valid value, we paint it with the corresponding color
            else
            {
                n = inputArray[board.matrix[i][j]];
                pieceColor = pieceTypes[n].color;
                cout << "\033[" << pieceColor << "m" << " " << "\033[0m";
            }
        }
        cout << endl;
    }
}

// Pre: board is the current state of the board and target is the number of target rows to complete
// Post: Returns false if the current board state is not the end, and returns true if it is,
//       meaning that the target rows have been completed
bool isSolution(const tpBoard board, const int target)
{

    int completed = 0;  // Total completed rows
    int count = 0;      // Total filled squares in a row
    bool stillPossible; // Variable to check if the row being read can still be completed
    int j;

    for (int i = 0; i < board.nRows; i++)
    {
        // Reset the necessary variables
        j = 0;
        stillPossible = true;

        while (j < board.nCols && stillPossible)
        {
            // If the square is empty, exit the loop and read the next row
            if (board.matrix[i][j] == -1)
            {
                stillPossible = false;
            }
            // Otherwise, count one more square
            else
            {
                count++;
            }

            j++;
        }
        // After reading a row, if the counter equals the total number of columns, we've completed a row
        if (count == board.nCols)
        {
            completed++;
        }
        // Reset the square counter when moving to the next row
        count = 0;
    }
    // Return true if and only if at least the target number of rows have been completed
    return (completed >= target);
}

// Pre: n indicates the position in inputArray where the piece is located that we want to check if it's
//      placeable in the column "column"
// Post: If the piece can be placed, it returns the row where it will be placed, otherwise returns -1
int isPlaceable(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column)
{

    int row = -1; // Initially set row = -1; if it is not modified during the function, the piece won't be placeable
    bool stillPlaceable = true;
    tpPiece pieceToCheck = pieceTypes[inputArray[n]];
    int j;

    for (int i = 0; i < board.nRows; i++)
    {
        // Reset necessary variables
        j = 0;
        while (j < PIECESIZE && stillPlaceable)
        {

            int a = pieceToCheck.shape[j][0];
            int b = pieceToCheck.shape[j][1];

            // Check if the coordinates to be checked are already occupied
            bool alreadyOccupied = board.matrix[i + a][column + b] != -1;
            // Check that the piece isn't being placed outside the board limits
            bool limits = ((column + b >= board.nCols) || (i + a >= board.nRows));

            // If any of the conditions for placing the piece are violated, it's not placeable
            if (alreadyOccupied || limits)
            {

                stillPlaceable = false;
            }
            j++;
        }
        // After the main loop, if the piece is still placeable, the row where it will start being placed is assigned
        if (stillPlaceable)
        {
            row = i;
        }
    }
    return row;
}

// Pre: 0 < row < board.nRows ∧ 0 < column < board.nCols ∧ 0 <= n <= MAXINPUT
// Post: The piece has been placed in the board's matrix
void placePiece(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column, const int row)
{

    tpPiece pieceToPlace = pieceTypes[inputArray[n]];

    for (int i = 0; i < PIECESIZE; i++)
    {

        int a = pieceToPlace.shape[i][0];
        int b = pieceToPlace.shape[i][1];
        // To place the piece, we add the x coordinate of the piece to the row and the y coordinate to the column
        board.matrix[row + a][column + b] = n;
    }
}

// Pre: 0 < row < board.nRows ∧ 0 < column < board.nCols ∧ 0 <= n <= MAXINPUT
// Post: The piece has been removed from the board's matrix
void removePiece(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column, const int row)
{

    tpPiece pieceToRemove = pieceTypes[inputArray[n]];

    for (int i = 0; i < PIECESIZE; i++)
    {

        int a = pieceToRemove.shape[i][0];
        int b = pieceToRemove.shape[i][1];
        // To remove the piece, we set the corresponding position to -1
        board.matrix[row + a][column + b] = -1;
    }
}

// Pre: outputArray contains the column assigned to each piece in the same order as in inputArray, n is the total number of
//      pieces placed
// Post: The game results have been displayed on the screen (input pieces, columns assigned to them, and the number of
//       pieces placed)
void outputData(const int inputArray[MAXINPUT], const int outputArray[MAXINPUT], const int n, const int target)
{

    // Display input pieces
    cout << "Input (piece numbers) : ";
    int i = 0;
    while (inputArray[i] != -1)
    {
        cout << inputArray[i] << " ";
        i++;
    }
    cout << endl;

    // Display the total number of pieces placed (-1 in case no solution is found)
    cout << "Number of pieces placed: " << n << endl;

    // Display the columns assigned to each piece placed
    cout << "Output (column numbers) : ";
    for (int j = 0; j < n; j++)
    {
        cout << outputArray[j] << " ";
    }
    cout << endl;
}

// Pre: The first n pieces from inputArray have been placed on the board in the columns indicated in outputArray
// Post: If the placed pieces complete at least target rows without gaps, then it returns the number of pieces placed,
//       the columns in which they have been placed, and the board with the pieces placed
//       If not, returns -1
int searchSolution(tpBoard &board, const int inputArray[MAXINPUT], int outputArray[MAXINPUT], const int target, int n, const int delay)
{

    int total = -1; // If no solution is found, this variable won't be modified
    int row;

    // Base case
    if (isSolution(board, target))
    {
        return n;
    }

    else
    {

        int column = 0;
        bool solved = false;

        while (column < board.nCols && !solved)
        {

            // Check if inputArray[n] is placeable, and if so, assign a row
            row = isPlaceable(board, inputArray, n, column);

            if (row != -1)
            {
                // If it's placeable, place it, display the board, and update outputArray
                placePiece(board, inputArray, n, column, row);
                displayWithDelay(delay, board, inputArray);
                outputArray[n] = column;

                // Use recursion to call the next piece
                total = searchSolution(board, inputArray, outputArray, target, n + 1, delay);

                // If no solution is found, remove the piece from the corresponding call
                if (total == -1)
                {
                    removePiece(board, inputArray, n, column, row);
                }
                // Otherwise, exit the loop and pass the value of total from the current call to the first call
                else
                {
                    solved = true;
                }
            }
            column++;
        }
    }
    return total;
}
