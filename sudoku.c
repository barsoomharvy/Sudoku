#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define threadCount 27 // number of threads
#define length 9

bool validThread[threadCount] = {0};

typedef struct
{
    int row;
    int column;
} parameters;

int sudoku[9][9] = {
{6, 2, 4, 5, 3, 9, 1, 8, 7},
{5, 1, 9, 7, 2, 8, 6, 3, 4},
{8, 3, 7, 6, 1, 4, 2, 9, 5},
{1, 4, 3, 8, 6, 5, 7, 2, 9},
{9, 5, 8, 2, 4, 7, 3, 6, 1},
{7, 6, 2, 3, 9, 1, 4, 5, 8},
{3, 7, 1, 9, 5, 6, 8, 4, 2},
{4, 9, 6, 1, 8, 2, 5, 7, 3},
{2, 8, 5, 4, 7, 3, 9, 1, 6}
};

void *checkBox(void* data)
{
    // casting data variable to type paramters
    // and setting its variables with row and column.
    parameters *dataptr = (parameters*) data; 
    int row = dataptr->row;
    int column = dataptr->column;

    // Our error checks revolve around making sure that row and column
    // are mulitple of 3s meaning mod 3 must be 0. 
    // Also if they are greater than 6 since we will need to access the 
    // next 3 values. 

    if (row % 3 != 0 || column % 3 != 0 || row > 6 || column > 6)
    {
        fprintf(stderr, "row or column are invalid for checking Box ");
        pthread_exit(NULL);

    }

    // We set a bool array to account for what we've seen and check 
    // 9 values which are the 3 columns and rows specified 
    // and set seen[val-1] to true when value isn't seen
    bool seen[9] = {0};

    for (int  i = row; i < row + 3; i++)
    {
        for (int j = column; j < column + 3; j++)
        {
            int val = sudoku[i][j];
            if ((val >= 1 || val <= 9) && seen[val-1] == false)
            {
                seen[val-1] = true;
            }
            else 
            {
                pthread_exit(NULL);
            }
        }
    }

    // We know this thread must be valid since it didn't exit thread

    validThread[row + column/3] = true;
    pthread_exit(NULL);
}

void *checkRow(void* data) 
{
    // casting data variable to type paramters
    // and setting its variables with row and column.
    parameters *dataptr = (parameters*) data; 
    int row = dataptr->row;
    int column = dataptr->column;

    // When col isn't 0 & row is greater than 8, we print error message
    if (column != 0 || row > 8)
    {
        fprintf(stderr, "row or column are invalid for checking row");
        pthread_exit(NULL);
    }

    // We now loop for length size to check if num is between 1 & 9 and hasn't been
    // seen by setting the seen ones to true. If it's seen or not between 1-9
    // we exit the thread

    bool seen[9] = {0};
    for (int i = 0; i < length; i++)
    {
        int val = sudoku[row][i]; // value that will be added if not seen

        if ((val >= 1 || val <= 9) && seen[val - 1] == false)
        {
            seen[val - 1] = true;
        }
        else // exiting  if value is seen already or not in range.
        {
            pthread_exit(NULL);
        }
   }

    // We now know that the row valid so we set the current thread 
    // value to true and then exit thread. We use 9 since row 
    // accounts for the other 9 which make up the 27 which is num
    // of threads.

    validThread[row + 9] = true;
    pthread_exit(NULL);

}


void *checkColumn(void* data) 
{
    // casting data variable to type paramters
    // and setting its variables with row and column.
    parameters *dataptr = (parameters*) data; 
    int row = dataptr->row;
    int column = dataptr->column;

    // When row isn't 0 & col is greater than 8, we print error message
    if (row != 0 || column > 8)
    {
        fprintf(stderr, "row or column are invalid for checking column");
        pthread_exit(NULL);
    }

    // We now loop for length size to check if num is between 1 & 9 and hasn't been
    // seen by setting the seen ones to true. If it's seen or not between 1-9
    // we exit the thread

    bool seen[9] = {0};
    for (int i = 0; i < length; i++)
    {
        int val = sudoku[i][column]; // value that will be added if not seen

        if ((val >= 1 || val <= 9) && seen[val - 1] == false)
        {
            seen[val - 1] = true;
        }
        else // exiting  if value is seen already or not in range.
        {
            pthread_exit(NULL);
        }
   }

    // We now know that the column valid so we set the current thread 
    // value to true and then exit thread. We use 18 since column 
    // accounts for the other 9 which make up the 27 which is num
    // of threads.

    validThread[column + 18] = true;
    pthread_exit(NULL);

}

int main()
{
    // Create the threads to open them
    pthread_t threads[threadCount];

    int thread = 0; // to specify which thread to open
    
    // looping through each value to check its validity for sudoku
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            // When values are multiples of 3, we check the box validity
            if ( i % 3 == 0 && j % 3 == 0)
            {
                // Set the parameter values using malloc with size of parameter
                parameters *boxData = (parameters*) malloc(sizeof(parameters));
                boxData->row = i;
                boxData->column = j;

                // Now we create the thread that calls the checkBox method for
                // sudoku validity 
                pthread_create(&threads[thread++], NULL, checkBox, boxData);
            }

            // When i is 0, we check the column for sudoku validity.
            if (i == 0)
            {
                // Set the parameter values using malloc with size of parameter
                parameters *rowData = (parameters*) malloc(sizeof(parameters));
                rowData->row = i;
                rowData->column = j;

                // Now we create the thread that calls the checkBox method for
                // sudoku validity 
                pthread_create(&threads[thread++], NULL, checkColumn, rowData);
            }

            // When j is 0, we check row's sudoku validity.
            if (j == 0)
            {
                // Set the parameter values using malloc with size of parameter
                parameters *columnData = (parameters*) malloc(sizeof(parameters));
                columnData->row = i;
                columnData->column = j;

                // Now we create the thread that calls the checkBox method for
                // sudoku validity 
                pthread_create(&threads[thread++], NULL, checkRow, columnData);
            }
        }
    }


    // Joining threads to main one.
    for (int i = 0; i < threadCount; i++)
    {
        pthread_join(threads[i], NULL);
    }


    // We loop through the valid thread array, if one element is false,
    // then the 2D array is not valid for sudoku.
    for (int i = 0; i < threadCount; i++)
    {
        if (!validThread[i])
        {
            printf("Sudoku is invalid\n");
            return 0;
        }
    }

    printf("SUDOKU IS VALID!!!!!!\n");
    return 0;
}
