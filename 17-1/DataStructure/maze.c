#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define MALLOC(p, s) if (!((p) = malloc(s))) { \
                        fprintf(stderr, "Insufficient memory for malloc!"); \
                        exit(EXIT_FAILURE); \
                        }

#define CALLOC(p, n, s) if (!((p) = calloc(n,s))) { \
                        fprintf(stderr, "Insufficient memory for calloc!"); \
                        exit(EXIT_FAILURE); \
                        }

#define REALLOC(p, s) if (!((p) = realloc(p, s))) { \
                        fprintf(stderr, "Insufficient memory for realloc!"); \
                        exit(EXIT_FAILURE); \
                        }

typedef struct Coordinate {
    int xpos;
    int ypos;
} Cor;
typedef struct Array_Stack {
    Cor *s_arr;
    int top;
    unsigned int s_len;
} Stack;
typedef struct Offsets {
    int vert;
    int horiz;
    int path;
} offsets;

void StackInit(Stack *pstack);
int SIsEmpty(Stack *pstack);
int SIsFull(Stack *pstack);
void SAppend(Stack *pstack, Cor cor);
Cor SPop(Stack *pstack);
Cor TopElem(Stack *pstack);

char **Make2dArray(int, int);
int ReadRow(void);
int ReadCol(void);

void RatInAMaze(void) {
    int i, j, k;
    int row, col;
    char **maze;

    row = ReadRow();
    col = ReadCol();
    maze = Make2dArray(row, col);

    FILE *fp = fopen("hw2_input.txt", "r");

    if (!fp) {
        puts("Cannot open the file!");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            fscanf(fp, "%c ", &maze[i][j]);
        }
    }

    fclose(fp);

    // Make new maze surrounding by *
    char **new_maze;
    new_maze = Make2dArray(row + 2, col + 2);

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            new_maze[i + 1][j + 1] = maze[i][j];
        }
    }

    for (i = 0; i < row + 2; i++) {
        for (j = 0; j < col + 2; j++) {
            new_maze[0][j] = '*';
            new_maze[row + 1][j] = '*';
        }
        new_maze[i][0] = '*';
        new_maze[i][col+1] = '*';
    }

    // Declare variables to search path
    short found = FALSE;
    short dir;
    int index = 0;
    int warp_num = 0;
    Cor start, end, warp, cur_pos, next_pos;
    offsets move[8] = {{-1, 0, TRUE}, {-1, 1, TRUE}, {0, 1, TRUE}, {1, 1, TRUE}, {1, 0, TRUE}, {1, -1, TRUE}, {0, -1, TRUE}, {-1, -1, TRUE}};
    Stack Warp;
    StackInit(&Warp);

    // Make 2D Array for indexing
    int **mark;
    CALLOC(mark, row + 2, sizeof(*mark));
    for (i = 0; i < row + 2; i++)
        CALLOC(mark[i], col + 2, sizeof(**mark));

    // Store start, end and warp points
    for (i = 0; i < row + 2; i++) {
        for (j = 0; j < col + 2; j++) {
            switch (new_maze[i][j]) {
                case ('s'):
                    start.xpos = i;
                    start.ypos = j;
                    break;
                case ('e'):
                    end.xpos = i;
                    end.ypos = j;
                    break;
                case ('w'):
                    warp.xpos = i;
                    warp.ypos = j;
                    SAppend(&Warp, warp);
                    warp_num++;
                    break;
                default:
                    break;
            }
        }
    }


    // Initialization
    mark[start.xpos][start.ypos] = ++index;
    cur_pos.xpos = start.xpos;
    cur_pos.ypos = start.ypos;

    // Start indexing
    while (!found) {
        if (mark[end.xpos][end.ypos]) {
            found = TRUE;
        } else if (!move[0].path && !move[1].path && !move[2].path && !move[3].path && !move[4].path && !move[5].path && !move[6].path && !move[7].path) {
            printf("There is no path!\n");
            found = TRUE;
        } else {
            for (dir = 0; dir < 8; dir++)
                move[dir].path = FALSE;
            for (i = 0; i < row + 2; i++) {
                for (j = 0; j < col + 2; j++) {
                    if (mark[i][j] == index) {
                        cur_pos.xpos = i;
                        cur_pos.ypos = j;
                        for (dir = 0; dir < 8; dir++) {
                            next_pos.xpos = cur_pos.xpos + move[dir].vert;
                            next_pos.ypos = cur_pos.ypos + move[dir].horiz;

                            if ((new_maze[next_pos.xpos][next_pos.ypos] == 'w') && (!(mark[next_pos.xpos][next_pos.ypos]))) {
                                move[dir].path = TRUE;
                                mark[next_pos.xpos][next_pos.ypos] = index + 1;
                                for (k = 0; k < warp_num; k++) {
                                    mark[Warp.s_arr[k].xpos][Warp.s_arr[k].ypos] = index + 1;
                                }
                            } else if ((new_maze[next_pos.xpos][next_pos.ypos] != '*') &&
                                       (!(mark[next_pos.xpos][next_pos.ypos]))) {
                                move[dir].path = TRUE;
                                mark[next_pos.xpos][next_pos.ypos] = index + 1;
                            }
                        }
                    }
                }
            }
        }
        index++;
    }

    // Declare variables to print shortest path
    int count = mark[end.xpos][end.ypos];
    Stack Path;
    StackInit(&Path);

    // Initialization
    cur_pos.xpos = end.xpos;
    cur_pos.ypos = end.ypos;
    SAppend(&Path, end);

    // Store shortest path into Stack
    if (mark[end.xpos][end.ypos]) {
        while (count > 1) {
            for (dir = 0; dir < 8; dir++) {
                next_pos.xpos = cur_pos.xpos + move[dir].vert;
                next_pos.ypos = cur_pos.ypos + move[dir].horiz;
                if (!((TopElem(&Path).xpos == cur_pos.xpos) && (TopElem(&Path).ypos == cur_pos.ypos))) {
                    if (SIsFull(&Path)) {
                        REALLOC(Path.s_arr, 2 * Path.s_len * sizeof(Cor));
                    }
                    SAppend(&Path, cur_pos);
                }

                if (mark[next_pos.xpos][next_pos.ypos] == count - 1) {
                    cur_pos.xpos = next_pos.xpos;
                    cur_pos.ypos = next_pos.ypos;
                    count--;
                } else if (new_maze[cur_pos.xpos][cur_pos.ypos] == 'w') {
                    for (k = 0; k < warp_num; k++) {
                        cur_pos.xpos = Warp.s_arr[k].xpos;
                        cur_pos.ypos = Warp.s_arr[k].ypos;
                        if (mark[next_pos.xpos][next_pos.ypos] == count - 1) {
                            cur_pos.xpos = next_pos.xpos;
                            cur_pos.ypos = next_pos.ypos;
                            count--;
                        }
                    }
                }
            }
        }
    }

    if (!((TopElem(&Path).xpos == cur_pos.xpos) && (TopElem(&Path).ypos == cur_pos.ypos))) {
        if (SIsFull(&Path)) {
            REALLOC(Path.s_arr, 2 * Path.s_len * sizeof(Cor));
        }
        SAppend(&Path, start);
    }

    // Print shortest path
    FILE *fp2 = fopen("hw2_output.txt", "w");

    if (!fp2) {
        puts("Cannot open the file!");
        exit(-1);
    }

    if (mark[end.xpos][end.ypos]) {
        fprintf(fp2, "%d\n", mark[end.xpos][end.ypos] - 1);
        while (!SIsEmpty(&Path)) {
            fprintf(fp2, "(%d, %d) ", TopElem(&Path).xpos, TopElem(&Path).ypos);
            SPop(&Path);
        }
    } else {
        fprintf(fp2, "NULL\n");
    }

    fclose(fp2);

    // Free allocated memories
    free(Path.s_arr);

    for (i = 0; i < row + 2; i++)
        free(mark[i]);
    free(mark);

    for (i = 0; i < row + 2; i++)
        free(new_maze[i]);
    free(new_maze);

    for (i = 0; i < row; i++)
        free(maze[i]);
    free(maze);

}

int main(void) {
    clock_t start, end;
    double result;

    start = clock();
    RatInAMaze();
    end = clock();

    result = (double) (end - start) / CLOCKS_PER_SEC;

    printf("Actual running time: %f\n", result);

    return 0;
}

int ReadRow(void) {
    int row = 0;
    char ch;

    FILE *fp = fopen("hw2_input.txt", "r");

    if (!fp) {
        puts("Cannot open the file!");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%c", &ch) != EOF) {
        if (ch == '\n')
            row++;
    }

    fclose(fp);
    return row + 1;
}
int ReadCol(void) {
    int col = 0;
    char ch;

    FILE *fp = fopen("hw2_input.txt", "r");

    if (!fp) {
        puts("Cannot open the file!");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fscanf(fp, "%c", &ch);
        if (ch == '\n')
            break;
        else if (ch != ' ')
            col++;
    }

    fclose(fp);
    return col;
}
char **Make2dArray(int row, int col) {
    int i;
    char **mat;

    MALLOC(mat, row * sizeof(*mat));
    for (i = 0; i < row; i++) {
        MALLOC(mat[i], col * sizeof(**mat));
    }

    return mat;
}

void StackInit(Stack *pstack) {
    pstack->s_len = 100;
    pstack->top = -1;
    MALLOC(pstack->s_arr, pstack->s_len * sizeof(Cor));
}
int SIsEmpty(Stack *pstack) {
    if (pstack->top == -1)
        return TRUE;
    else
        return FALSE;
}
int SIsFull(Stack *pstack) {
    if (pstack->top == pstack->s_len)
        return TRUE;
    else
        return FALSE;
}
void SAppend(Stack *pstack, Cor cor) {
    pstack->top++;
    pstack->s_arr[pstack->top] = cor;
}
Cor SPop(Stack *pstack) {
    int rIdx;

    if (SIsEmpty(pstack)) {
        printf("Stack is empty!");
        exit(-1);
    }

    rIdx = pstack->top;
    pstack->top--;

    return pstack->s_arr[rIdx];
}
Cor TopElem(Stack *pstack) {
    if (SIsEmpty(pstack)) {
        printf("Stack is empty!");
        exit(-1);
    }

    return pstack->s_arr[pstack->top];
}