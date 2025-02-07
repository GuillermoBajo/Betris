#include <iostream>

using namespace std;

// CONSTANTS AND TYPES

// Maximum number of rows and columns for the board
const int MAXDIM = 100;

// Maximum number of pieces in the input
const int MAXINPUT = 3000;

// Size in number of squares for each piece
const int PIECESIZE = 4;

// Number of defined pieces
const int PIECESDEF = 5;

// Color numeric codes
const int BLACK = 40;
const int RED = 41;
const int GREEN = 42;
const int YELLOW = 43;
const int BLUE = 44;
const int MAGENTA = 45;
const int CYAN = 46;
const int WHITE = 47;

// Board type
struct tpBoard
{
    int nRows;                  // Number of rows in the board
    int nCols;                  // Number of columns in the board
    int matrix[MAXDIM][MAXDIM]; // The value of each cell indicates the piece number from the input that occupies it.
                                // If the value is -1, the cell is empty.
                                // The coordinates of the top-left corner cell are (0,0)
};

// Piece type (color and shape)
struct tpPiece
{
    int color;               // Piece color code
    int shape[PIECESIZE][2]; // Coordinates of the piece's squares
                             // The first value is the row number, the second is the column number
                             // The top-left corner coordinate is (0,0)
                             // Example shape: [[0,0], [0,1], [1,0], [2,0] ] is the piece XX
                             //                                                           X
                             //                                                           X
};

// Definition of existing pieces. The number of each piece is its position in this vector
const tpPiece pieceTypes[PIECESDEF] = {
    {BLUE, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},  // 0  XXXX
    {GREEN, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}}, // 1  X
                                               //    X
                                               //    X
                                               //    X
    {RED, {{0, 0}, {0, 1}, {1, 0}, {2, 0}}},   // 2  XX
                                               //    X
                                               //    X
    {CYAN, {{0, 0}, {0, 1}, {0, 2}, {1, 2}}},  // 3  XXX
                                               //      X
    {YELLOW, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}} // 4  XX
                                               //    XX
};

// FUNCTIONS

// Pre: delay is the time pause to make after displaying the state of the board.
//      displayBoard shows the board in its current state
// Post: If delay > 0, the board has been displayed with the corresponding delay. Otherwise, it's simply displayed
void displayWithDelay(const int delay, tpBoard &board, const int inputArray[MAXINPUT]);

// Pre: 0 <= n <= MAXINPUT
// Post: A random sequence of size -n pieces has been generated and stored in inputArray
void generateRandomSequence(int &n, int inputArray[MAXINPUT]);

// Pre: True
// Post: User input data from terminal has been stored in respective variables
void inputData(int argc, char *argv[], int &target, int &delay, int inputArray[MAXINPUT], tpBoard &board);

// Pre: True
// Post: All components of board.matrix are -1 (empty square)
void initializeBoard(tpBoard &board);

// Pre: inputArray contains the numbers of the pieces that enter, and board contains the current state
//      of the board, where some of those pieces might be placed
// Post: The board has been displayed on screen (each piece drawn with its color)
void displayBoard(const tpBoard &board, const int inputArray[MAXINPUT]);

// Pre: board is the current state of the board and target is the number of target rows to complete
// Post: Returns false if the current board state is not the end, and returns true if it is,
//       meaning that the target rows have been completed
bool isSolution(const tpBoard board, const int target);

// Pre: n indicates the position in inputArray where the piece is located that we want to check if it's
//      placeable in the column "column"
// Post: If the piece can be placed, it returns the row where it will be placed, otherwise returns -1
int isPlaceable(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column);

// Pre: 0 < row < board.nRows ∧ 0 < column < board.nCols ∧ 0 <= n <= MAXINPUT
// Post: The piece has been placed in the board's matrix
void placePiece(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column, const int row);

// Pre: 0 < row < board.nRows ∧ 0 < column < board.nCols ∧ 0 <= n <= MAXINPUT
// Post: The piece has been removed from the board's matrix
void removePiece(tpBoard &board, const int inputArray[MAXINPUT], const int n, const int column, const int row);

// Pre: outputArray contains the column assigned to each piece in the same order as in inputArray, n is the total number of
//      pieces placed
// Post: The game results have been displayed on the screen (input pieces, columns assigned to them, and the number of
//       pieces placed)
void outputData(const int inputArray[MAXINPUT], const int outputArray[MAXINPUT], const int n, const int target);

// Pre: The first n pieces from inputArray have been placed on the board in the columns indicated in outputArray
// Post: If the placed pieces complete at least target rows without gaps, then it returns the number of pieces placed,
//       the columns in which they have been placed, and the board with the pieces placed
//       If not, returns -1
int searchSolution(tpBoard &board, const int inputArray[MAXINPUT], int outputArray[MAXINPUT], const int target, int n, const int delay);
