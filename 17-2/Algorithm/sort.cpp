#include <cstdio>
#include <cstring>
#include <ctime>

int size;

// Bubble sort function
void bubble_sort(int *list, int size) {
    int i, j;
    int tmp;

    for (i = size - 1; i > 0; i--) {
        for (j = 0; j < i; j++) {
            if (list[j] > list[j + 1]) {
                tmp = list[j + 1];
                list[j + 1] = list[j];
                list[j] = tmp;
            }
        }
    }
}

// Insertion sort function
void insertion_sort(int* list, int start, int end) {
    int i, j;
    int tmp;

    for (i = start + 1; i < end + 1; i++) {
        for (j = i; j > start; j--) {
            if (list[j - 1] > list [j]) {
                tmp = list[j];
                list[j] = list[j - 1];
                list[j -1] = tmp;
            } else {
                break;
            }
        }
    }
}

// Merge sort functions
void merge (int* list, int* l_list, int l_size, int* r_list, int r_size) {
    int l_idx = 0;
    int r_idx = 0;
    int tmp_idx = 0;

    while (l_idx < l_size && r_idx < r_size) {
        if (l_list[l_idx] <= r_list[r_idx]) {
            list[tmp_idx] = l_list[l_idx];
            l_idx++;
            tmp_idx++;
        }
        else {
            list[tmp_idx] = r_list[r_idx];
            r_idx++;
            tmp_idx++;
        }
    }

    while (l_idx < l_size) {
        list[tmp_idx]= l_list[l_idx];
        l_idx++;
        tmp_idx++;
    }

    while (r_idx < r_size)  {
        list[tmp_idx]= r_list[r_idx];
        r_idx++;
        tmp_idx++;
    }
}
void merge_sort(int *list, int size) {
    int i;
    int *l_list, *r_list;
    int l_size, r_size;

    if (size < 10) {
        insertion_sort(list, 0, size - 1);
        return;
    }

    l_size = size / 2;
    r_size = size - l_size;
    l_list = new int[l_size];
    r_list = new int[r_size];


    for (i = 0; i < l_size; i++) {
        l_list[i] = list[i];
    }

    for (i = 0; i < r_size; i++) {
        r_list[i] = list[i + l_size];
    }

    merge_sort(l_list, l_size);
    merge_sort(r_list, r_size);
    merge(list, l_list, l_size, r_list, r_size);

    delete[] l_list;
    delete[] r_list;
}

// Heap sort functions
int parent_idx(int idx) {
    return (idx - 1) / 2;
}
int l_child_idx(int idx) {
    return idx * 2 + 1;
}
int r_child_idx(int idx) {
    return idx * 2 + 2;
}
int high_pri_child_idx(int *list, int num_of_data, int idx) {
    if (l_child_idx(idx) > num_of_data - 1)
        return 0;
    else if (l_child_idx(idx) == num_of_data - 1)
        return l_child_idx(idx);
    else {
        if (list[l_child_idx(idx)] > list[r_child_idx(idx)])
            return r_child_idx(idx);
        else
            return l_child_idx(idx);
    }
}
void h_append(int *list, int num, int data) {
    int idx = num;

    while (idx != 0) {
        if (data < list[parent_idx(idx)]) {
            list[idx] = list[parent_idx(idx)];
            idx = parent_idx(idx);
        } else
            break;
    }

    list[idx] = data;
}
int h_pop(int *list, int num_of_data) {
    int d_data = list[0];
    int last_data = list[num_of_data - 1];
    int p_idx = 0;
    int c_idx;

    while (c_idx = high_pri_child_idx(list, num_of_data, p_idx)) {
        if (last_data < list[c_idx])
            break;

        list[p_idx] = list[c_idx];
        p_idx = c_idx;
    }

    list[p_idx] = last_data;

    return d_data;
}
void heap_sort(int *list) {
    int *tmp_list = new int [size];
    int num_of_data = 0;

    for (int i = 0; i < size; i++) {
        h_append(tmp_list, i, list[i]);
        num_of_data++;
    }

    for (int i = 0; i < size; i++) {
        list[i] = h_pop(tmp_list, num_of_data);
        num_of_data--;
    }

    delete [] tmp_list;
}

// Quicksort function
void quick_sort(int* list, int start, int end) {
    if (end - start <= 10)
        insertion_sort(list, start, end);
    else if (start < end) {
        int mid = (start + end) / 2;
        int l_idx = start;
        int r_idx = end;
        int pivot;
        int tmp;

        pivot = (list[start] > list[mid] ? start : mid) < (list[mid] > list[end] ? mid : end)
                ? (list[start] > list[mid] ? start : mid) : (list[mid] > list[end] ? mid : end);

        while (l_idx < r_idx) {
            while (list[l_idx] <= list[pivot] && l_idx < end)
                l_idx++;
            while (list[r_idx] > list[pivot] && r_idx > start)
                r_idx--;

            if (l_idx < r_idx) {
                tmp = list[l_idx];
                list[l_idx] = list[r_idx];
                list[r_idx] = tmp;
            }
        }

        tmp = list[r_idx];
        list[r_idx] = list[pivot];
        list[pivot] = tmp;

        quick_sort(list, start, r_idx - 1);
        quick_sort(list, r_idx + 1, end);
    }
}

int main(int argc, char** argv) {
    FILE *ifp;
    FILE *ofp;
    clock_t start, end;
    double result;

    ifp = fopen(argv[2], "r");
    fscanf(ifp, "%d", &size);
    int* list = new int[size];
    for (int i = 0; i < size; i++)
        fscanf(ifp, "%d", &list[i]);
    fclose(ifp);

    if (strcmp(argv[1], "B") == 0) {
        start = clock();
        bubble_sort(list, size);
        end = clock();

        ofp = fopen(argv[3], "w");
        fprintf(ofp, "%d\n", size);
        for (int i = 0; i < size; i++)
            fprintf(ofp, "%d ", list[i]);
        fclose(ofp);

        result = (double) (end - start) / CLOCKS_PER_SEC;
        printf("\nBubble sort for %d integers took %f seconds.\n", size, result);
    } else if (strcmp(argv[1], "I") == 0) {
        start = clock();
        insertion_sort(list, 0, size - 1);
        end = clock();

        ofp = fopen(argv[3], "w");
        fprintf(ofp, "%d\n", size);
        for (int i = 0; i < size; i++)
            fprintf(ofp, "%d ", list[i]);
        fclose(ofp);

        result = (double) (end - start) / CLOCKS_PER_SEC;
        printf("\nInsertion sort for %d integers took %f seconds.\n", size, result);
    } else if (strcmp(argv[1], "M") == 0) {
        start = clock();
        merge_sort(list, size);
        end = clock();

        ofp = fopen(argv[3], "w");
        fprintf(ofp, "%d\n", size);
        for (int i = 0; i < size; i++)
            fprintf(ofp, "%d ", list[i]);
        fclose(ofp);

        result = (double) (end - start) / CLOCKS_PER_SEC;
        printf("\nMerge sort for %d integers took %f seconds.\n", size, result);
    } else if (strcmp(argv[1], "H") == 0) {
        start = clock();
        heap_sort(list);
        end = clock();

        ofp = fopen(argv[3], "w");
        fprintf(ofp, "%d\n", size);
        for (int i = 0; i < size; i++)
            fprintf(ofp, "%d ", list[i]);
        fclose(ofp);

        result = (double) (end - start) / CLOCKS_PER_SEC;
        printf("\nHeap sort for %d integers took %f seconds.\n", size, result);
    } else if (strcmp(argv[1], "Q") == 0) {
        start = clock();
        quick_sort(list, 0, size - 1);
        end = clock();

        ofp = fopen(argv[3], "w");
        fprintf(ofp, "%d\n", size);
        for (int i = 0; i < size; i++)
            fprintf(ofp, "%d ", list[i]);
        fclose(ofp);

        result = (double) (end - start) / CLOCKS_PER_SEC;
        printf("\nQuicksort for %d integers took %f seconds.\n", size, result);
    }

    delete [] list;

    return 0;
}