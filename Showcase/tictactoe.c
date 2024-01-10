#include "my_syscalls.h"

#define SIZE 3

char board[SIZE][SIZE];

void initializeBoard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void printBoard()
{
    my_printstr("-------------\n");
    for (int i = 0; i < SIZE; i++)
    {
        my_printstr("| ");
        my_putchar(board[i][0]);
        my_printstr(" | ");
        my_putchar(board[i][1]);
        my_printstr(" | ");
        my_putchar(board[i][2]);
        my_printstr(" |\n");
        my_printstr("-------------\n");
    }
}

int isBoardFull()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == ' ')
            {
                return 0;
            }
        }
    }
    return 1;
}

int isWinner(char player)
{
    // Check rows
    for (int i = 0; i < SIZE; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
        {
            return 1;
        }
    }

    // Check columns
    for (int j = 0; j < SIZE; j++)
    {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
        {
            return 1;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
    {
        return 1;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
    {
        return 1;
    }

    return 0;
}

int makeMove(int row, int col, char player)
{
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ')
    {
        return 0;
    }

    board[row][col] = player;
    return 1;
}

int main()
{
    initializeBoard();
    char currentPlayer = 'X';

    while (1)
    {
        printBoard();

        int row, col;
        my_printstr("It's  ");
        my_putchar(currentPlayer);
        my_printstr("'s turn.\n ");
        my_printstr("Enter row (1 - 3) : ");
        row = my_scanfint() - 1 ;
        my_printstr("Enter column (1 - 3) : ");
        col= my_scanfint() - 1;
        if (makeMove(row, col, currentPlayer))
        {
            if (isWinner(currentPlayer))
            {

                my_printstr("Player ");
                my_putchar(currentPlayer);
                my_printstr(" wins!\n");
                break;
            }
            else if (isBoardFull())
            {
                my_printstr("It's a draw!\n");
                break;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
        else
        {
            my_printstr("Invalid move. Try again.\n");
        }
    }

    my_exit();
}
