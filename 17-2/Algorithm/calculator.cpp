#include <cstdio>
#define TRUE 1
#define FALSE 0
#define LEN 100

// Stack functions
typedef struct Stack {
    int top;
    int s_arr[LEN];
} stack;
void s_init(stack *pstack) {
    pstack->top = -1;
}
int s_is_empty(stack *pstack) {
    if (pstack->top == -1)
        return TRUE;
    else
        return FALSE;
}
void s_append(stack *pstack, int data) {
    pstack->top += 1;
    pstack->s_arr[pstack->top] = data;
}
int s_pop(stack *pstack) {
    int r_idx;

    if (s_is_empty(pstack)) {
        puts("Stack is empty!");
        return -1;
    }

    r_idx = pstack->top;
    pstack->top -= 1;

    return pstack->s_arr[r_idx];
}

// Stack for calculation
typedef struct Calc_Stack {
    int top;
    double s_arr[LEN];
} calc_stack;
void cs_init(calc_stack *pstack) {
    pstack->top = -1;
}
int cs_is_empty(calc_stack *pstack) {
    if (pstack->top == -1)
        return TRUE;
    else
        return FALSE;
}
void cs_append(calc_stack *pstack, double data) {
    pstack->top += 1;
    pstack->s_arr[pstack->top] = data;
}
double cs_pop(calc_stack *pstack) {
    int r_idx;

    if (cs_is_empty(pstack)) {
        puts("Stack is empty!");
        return -1;
    }

    r_idx = pstack->top;
    pstack->top -= 1;

    return pstack->s_arr[r_idx];
}

// Global variables
stack infix, postfix, oper;
calc_stack calc;
int size = 0;
int count_par = 0;
double result;

// Helper functions
int read_input(FILE *fp) {
    char data;
    s_init(&infix);

    fp = fopen("input_calc.txt", "r");
    if (!fp) {
        puts("Cannot read the file!");
        return -1;
    }

    while (fscanf(fp, "%c", &data) != EOF) {
        if ((data == '(') || (data == ')'))
            count_par++;
        size++;
        s_append(&infix, data);
    }
    fclose(fp);

    return 0;
}
int write_output(FILE *fp) {
    fp = fopen("output_calc.txt", "w");
    if (!fp) {
        puts("Cannot write the file!");
        return -1;
    }

    fprintf(fp, "%.2f", result);
    fclose(fp);

    return 0;
}

void infix_to_postfix(void) {
    int i;
    s_init(&oper);
    s_init(&postfix);

    for (i = 0; i <= size; i++) {
        if (i == size) {
            while (!(s_is_empty(&oper))) {
                s_append(&postfix, s_pop(&oper));
            }
            break;
        }
        if ((infix.s_arr[i] == '1') || (infix.s_arr[i] == '2') || (infix.s_arr[i] == '3') ||
            (infix.s_arr[i] == '4') || (infix.s_arr[i] == '5') || (infix.s_arr[i] == '6') ||
            (infix.s_arr[i] == '7') || (infix.s_arr[i] == '8') || (infix.s_arr[i] == '9')) {
            s_append(&postfix, infix.s_arr[i]);
        } else {
            if ((infix.s_arr[i] == '*') || (infix.s_arr[i] == '/')) {
                if (s_is_empty(&oper)) {
                    s_append(&oper, infix.s_arr[i]);
                } else if (!(s_is_empty(&oper))) {
                    if ((oper.s_arr[oper.top] == '*') || (oper.s_arr[oper.top] == '/')) {
                        while ((oper.s_arr[oper.top] == '*') || (oper.s_arr[oper.top] == '/')) {
                            s_append(&postfix, s_pop(&oper));
                        }
                        s_append(&oper, infix.s_arr[i]);
                    } else {
                        s_append(&oper, infix.s_arr[i]);
                    }
                }
            } else if ((infix.s_arr[i] == '+') || (infix.s_arr[i] == '-')) {
                if (s_is_empty(&oper)) {
                    s_append(&oper, infix.s_arr[i]);
                } else if (!(s_is_empty(&oper))) {
                    if (oper.s_arr[oper.top] == '*' || oper.s_arr[oper.top] == '/' ||
                        oper.s_arr[oper.top] == '+' ||
                        oper.s_arr[oper.top] == '-') {
                        while ((oper.s_arr[oper.top] == '*') || (oper.s_arr[oper.top] == '/') ||
                               (oper.s_arr[oper.top] == '+') || (oper.s_arr[oper.top] == '-')) {
                            s_append(&postfix, s_pop(&oper));
                        }
                        s_append(&oper, infix.s_arr[i]);
                    } else {
                        s_append(&oper, infix.s_arr[i]);
                    }
                }
            } else if (infix.s_arr[i] == '(') {
                s_append(&oper, infix.s_arr[i]);
            } else if (infix.s_arr[i] == ')') {
                while (oper.s_arr[oper.top] != '(') {
                    s_append(&postfix, s_pop(&oper));
                }
                s_pop(&oper);
            }
        }
    }
}
void calculator(void) {
    cs_init(&calc);
    double value;

    for (int i = 0; i <= size - count_par; i++) {
        if (i == size - count_par) {
            result = calc.s_arr[calc.top];
            break;
        }
        if (postfix.s_arr[i] == '1') {
            value = 1.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '2') {
            value = 2.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '3') {
            value = 3.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '4') {
            value = 4.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '5') {
            value = 5.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '6') {
            value = 6.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '7') {
            value = 7.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '8') {
            value = 8.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '9') {
            value = 9.0;
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '+') {
            value = calc.s_arr[calc.top - 1] + calc.s_arr[calc.top];
            cs_pop(&calc);
            cs_pop(&calc);
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '-') {
            value = calc.s_arr[calc.top - 1] - calc.s_arr[calc.top];
            cs_pop(&calc);
            cs_pop(&calc);
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '*') {
            value = calc.s_arr[calc.top - 1] * calc.s_arr[calc.top];
            cs_pop(&calc);
            cs_pop(&calc);
            cs_append(&calc, value);
        } else if (postfix.s_arr[i] == '/') {
            value = calc.s_arr[calc.top - 1] / calc.s_arr[calc.top];
            cs_pop(&calc);
            cs_pop(&calc);
            cs_append(&calc, value);
        }
    }
}

int main(void) {
    int i;
    FILE *fp, *fp2;

    read_input(fp);

    for (i = 0; i < size; i++) {
        printf("%c", infix.s_arr[i]);
    }

    printf("\n");
    infix_to_postfix();

    for (i = 0; i < size - count_par; i++) {
        printf("%c", postfix.s_arr[i]);
    }

    printf("\n");
    calculator();

    printf("%.2f", result);
    write_output(fp2);

    return 0;
}