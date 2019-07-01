#include <cstdio>
#define TRUE 1
#define FALSE 0
#define LEN 225

// Global variables
int size;
typedef struct Coordinate {
    int ver;
    int hor;
    int step = 0;
} cor;
cor start, end, cur;

// Helper function
int read_input(FILE *fp) {

    fp = fopen("input_knight.txt", "r");
    if (!fp) {
        puts("Cannot read the input!");
        return -1;
    }

    fscanf(fp, "%d", &size);
    fscanf(fp, "%d %d %d %d", &start.ver, &start.hor, &end.ver, &end.hor);

    fclose(fp);
    return 0;
}
int write_output(FILE *fp) {
    fp = fopen("output_knight.txt", "w");

    if (!fp) {
        puts("Cannot write the output!");
        return -1;
    }

    if (start.hor == end.hor && start.ver == end.ver)
        fprintf(fp, "%d", cur.step);
    else
        fprintf(fp, "%d", cur.step + 1);
    fclose(fp);

    return 0;
}

// Queue function
typedef struct Queue {
    cor q_arr[LEN];
    int front;
    int rear;
} que;
void q_init(que *pq) {
    pq->front = 0;
    pq->rear = 0;
}
int q_empty(que *pq) {
    if (pq->front == pq->rear)
        return TRUE;
    else
        return FALSE;
}
void q_append(que *pq, cor pos) {
    pq->q_arr[pq->rear] = pos;
    pq->rear += 1;
}
int q_pop(que *pq) {
    if (q_empty(pq)) {
        puts("Queue is empty!");
        return -1;
    }

    pq->front += 1;
    return 0;
}

// Minimum path function
void min_path(void) {
    int i, j;
    int chess[size][size];
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            chess[i][j] = 1;
        }
    }

    que pos;
    q_init(&pos);
    q_append(&pos, start);

    chess[start.ver][start.hor] = 0;
    cur = start;
    cor d0, d1, d2, d3, d4, d5, d6, d7;

    while (chess[end.ver][end.hor]) {
        d0 = {cur.ver - 2, cur.hor - 1, cur.step + 1};
        d1 = {cur.ver - 2, cur.hor + 1, cur.step + 1};
        d2 = {cur.ver - 1, cur.hor + 2, cur.step + 1};
        d3 = {cur.ver + 1, cur.hor + 2, cur.step + 1};
        d4 = {cur.ver + 2, cur.hor + 1, cur.step + 1};
        d5 = {cur.ver + 2, cur.hor - 1, cur.step + 1};
        d6 = {cur.ver + 1, cur.hor - 2, cur.step + 1};
        d7 = {cur.ver - 1, cur.hor - 2, cur.step + 1};

        if ((d0.ver >= 0) && (d0.hor >= 0) && (chess[d0.ver][d0.hor])) {
            chess[d0.ver][d0.hor] = 0;
            q_append(&pos, d0);
        }
        if ((d1.ver >= 0) && (d1.hor <= size - 1) && (chess[d1.ver][d1.hor])) {
            chess[d1.ver][d1.hor] = 0;
            q_append(&pos, d1);
        }
        if ((d2.ver >= 0) && (d2.hor <= size - 1) && (chess[d2.ver][d2.hor])) {
            chess[d2.ver][d2.hor] = 0;
            q_append(&pos, d2);
        }
        if ((d3.ver <= size - 1) && (d3.hor <= size - 1) && (chess[d3.ver][d3.hor])) {
            chess[d3.ver][d3.hor] = 0;
            q_append(&pos, d3);
        }
        if ((d4.ver <= size - 1) && (d4.hor <= size - 1) && (chess[d4.ver][d4.hor])) {
            chess[d4.ver][d4.hor] = 0;
            q_append(&pos, d4);
        }
        if ((d5.ver <= size - 1) && (d5.hor >= 0) && (chess[d5.ver][d5.hor])) {
            chess[d5.ver][d5.hor] = 0;
            q_append(&pos, d5);
        }
        if ((d6.ver <= size - 1) && (d6.hor >= 0) && (chess[d6.ver][d6.hor])) {
            chess[d6.ver][d6.hor] = 0;
            q_append(&pos, d6);
        }
        if ((d7.ver >= 0) && (d7.hor >= 0) && (chess[d7.ver][d7.hor])) {
            chess[d7.ver][d7.hor] = 0;
            q_append(&pos, d7);
        }

        q_pop(&pos);
        cur.ver = pos.q_arr[pos.front].ver;
        cur.hor = pos.q_arr[pos.front].hor;
        cur.step = pos.q_arr[pos.front].step;
    }
}

int main(void) {
    FILE *fp, *fp2;

    read_input(fp);
    min_path();
    write_output(fp2);

    return 0;
}