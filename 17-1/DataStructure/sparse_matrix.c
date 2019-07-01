#include <stdio.h>
#include <time.h>

int Transpose(void)
{
    char major;
    int row, col;
    int i, j;
    int count = 0, size = 0;

    FILE *fp = fopen("hw1_input.txt", "r");

    // If fp fails, print out "Fail to read"
    if (fp == NULL) {
        printf("Fail to read");
        return -1;
    }

    fscanf(fp, "%c %d %d", &major, &row, &col); // Decide major, rows and columns

    int mat[row][col]; // Declare matrix

    // Read every values in sparse matrix
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {           // if row = n, column = m : times : n * m
            fscanf(fp, "%d", &mat[i][j]);
            if (mat[i][j] != 0)
                count++;
        }
    }
    fclose(fp);

    // Declare output matrices
    int Output_a[3][count];
    int Output_b[3][count];

    // Transpose Matrix by major
    if (major == 'r') {
        // Declare Step arrays
        int Step1[col];
        int Step2[col];
        for (i = 0; i < col; i++) {
            Step1[i] = 0;
            Step2[i] = 0;
        }

        // Put in values to output_a matrix and step1 array
        for (i = 0; i < row; i++) {
            for (j = 0; j < col; j++) {
                if (mat[i][j] != 0) {
                    Output_a[0][size] = i + 1; // rows
                    Output_a[1][size] = j + 1; // columns
                    Output_a[2][size] = mat[i][j]; // values
                    Step1[j]++;
                    size++;
                }
            }
        }

        // Making step 2 array
        for (i = 1; i < col; i++) {
            Step2[i] = Step1[i - 1] + Step2[i - 1];
        }

        // Deciding the position while sorting
        for (i = 0; i < size; i++) {
            Output_b[0][Step2[Output_a[1][i] - 1]] = Output_a[1][i];
            Output_b[1][Step2[Output_a[1][i] - 1]] = Output_a[0][i];
            Output_b[2][Step2[Output_a[1][i] - 1]] = Output_a[2][i];
            Step2[Output_a[1][i] - 1]++;
        }
    } else {
        int Step1[row];
        int Step2[row];
        for (i = 0; i < row; i++) {
            Step1[i] = 0;
            Step2[i] = 0;
        }

        // Put it values to Output_a matrix and Step1 array
        for (j = 0; j < col; j++) {
            for (i = 0; i < row; i++) {
                if (mat[i][j] != 0) {
                    Output_a[0][size] = i + 1; // rows
                    Output_a[1][size] = j + 1; // columns
                    Output_a[2][size] = mat[i][j]; // values
                    Step1[i]++;
                    size++;
                }
            }
        }

        // Make Step2 array
        for (i = 1; i < row; i++) {
            Step2[i] = Step1[i - 1] + Step2[i - 1];
        }

        // Deciding the position while sorting
        for (i = 0; i < size; i++) {
            Output_b[0][Step2[Output_a[0][i] - 1]] = Output_a[1][i];
            Output_b[1][Step2[Output_a[0][i] - 1]] = Output_a[0][i];
            Output_b[2][Step2[Output_a[0][i] - 1]] = Output_a[2][i];
            Step2[Output_a[0][i] - 1]++;
        }
    }

    // From now on, make txt file
    FILE *fp1 = fopen("hw1a_output.txt", "w");
    if (fp1 == NULL) {
        printf("Fail to write");
        return -1;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < size; j++) {
            fprintf(fp1, "%d ", Output_a[i][j]);
        }
        fprintf(fp1, "\n");
    }
    fclose(fp1);

    FILE *fp2 = fopen("hw1b_output.txt", "w");
    if (fp2 == NULL) {
        printf("Fail to write");
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < size; j++) {
            fprintf(fp2, "%d ", Output_b[i][j]);
        }
        fprintf(fp2, "\n");
    }
    fclose(fp2);

    return 0;
}

int main(void)
{
    clock_t start, end;
    double result;

    start = clock();
    Transpose();
    end = clock();

    result = (double) (end - start) / CLOCKS_PER_SEC;

    printf("Actual running time: %f\n", result);

    return 0;
}