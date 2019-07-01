#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int bt_data;
typedef bt_data bst_data;
typedef struct bt_node {
    bt_data data;
    struct bt_node *left;
    struct bt_node *right;
    struct bt_node *parent;
} bt_node;

bt_node *make_bt_node(void);
bt_data get_data(bt_node *bt);
void set_data(bt_node *bt, bt_data data);
bt_node *get_left_sub_tree(bt_node *bt);
bt_node *get_right_sub_tree(bt_node *bt);
void make_left_sub_tree(bt_node *main, bt_node *sub);
void make_right_sub_tree(bt_node *main, bt_node *sub);
bt_node *remove_left_sub_tree(bt_node *bt);
bt_node *remove_right_sub_tree(bt_node *bt);
void change_left_sub_tree(bt_node *main, bt_node *sub);
void change_right_sub_tree(bt_node *main, bt_node *sub);

void bst_make_and_init(bt_node **p_root);
bst_data bst_get_data(bt_node *bst);
void bst_insert(bt_node **p_root, bst_data data);
bt_node *bst_search(bt_node *bst, bst_data target);
bt_node bst_remove(bt_node *bst);
void attach(bt_node **p_root, bt_node *bt);

void show_preorder(bt_node *bt);
void T3_preorder(bt_node *bt);
void T4_preorder(bt_node *bt);
void file_print(bt_node *bt);

void union_bst(void);

int main(void) {
    double result;
    clock_t start, end;

    start = clock();
    union_bst();
    end = clock();

    result = (double) (end - start) / CLOCKS_PER_SEC;

    printf("\nActual running time: %f\n", result);

    return 0;
}

int T3_count = 0;
int T4_count = 0;

void union_bst(void) {
    int key;
    int i = 0;
    int T3_na, T3_nd;
    int T4_na, T4_nd;
    int count1 = 0, count2 = 0;
    char ch;
    bt_node *T1_root;
    bt_node *T2_root;
    bt_node *T3_root;
    bt_node *T4_root;
    bst_make_and_init(&T1_root);
    bst_make_and_init(&T2_root);
    bst_make_and_init(&T3_root);
    bst_make_and_init(&T4_root);

    FILE *fp = fopen("hw3_input.txt", "r");

    if (!fp) {
        puts("Cannot open the file!");
        exit(-1);
    }

    // make T1
    fscanf(fp, "%d", &count1);
    if (count1) {
        while (fscanf(fp, "%d", &key) != EOF) {
            fscanf(fp, "%c", &ch);
            if (ch == '\n')
                i++;

            bst_insert(&T1_root, key);
            bst_insert(&T3_root, key);

            if (i == count1 - 1)
                break;
        }
    }

    // make T2
    fscanf(fp, "%d", &count2);
    if (count2) {
        while (fscanf(fp, "%d", &key) != EOF) {
            fscanf(fp, "%c", &ch);
            bst_insert(&T2_root, key);
            bst_insert(&T4_root, key);
        }
    }

    fclose(fp);

    attach(&T3_root, T2_root);
    attach(&T4_root, T1_root);

    bt_node *s_node = bst_search(T3_root, get_data(T2_root));
    T3_preorder(s_node);
    s_node = bst_search(T4_root, get_data(T1_root));
    T4_preorder(s_node);

    T3_na = count2 - T3_count + 1;
    T3_nd = T3_na - 1;
    T4_na = count1 - T4_count + 1;
    T4_nd = T4_na - 1;

    //------------------------------------------------------------------------------------------------------------------
    // print output
    FILE *ofp = fopen("hw3_output.txt", "w");

    if (!ofp) {
        printf("Cannot open the file!");
        exit(-1);
    }

    if (T3_na > T4_nd)
        fprintf(ofp, "%d\n", get_data(T4_root));
    else
        fprintf(ofp, "%d\n", get_data(T3_root));
    fclose(ofp);

    ofp = fopen("hw3_output.txt", "a");
    if (T3_na > T4_na) {
        file_print(T4_root);
        fprintf(ofp, "%d\n%d", T4_na, T4_nd);
    } else {
        file_print(T3_root);
        fprintf(ofp, "%d\n%d", T3_na, T3_nd);
    }

    fclose(ofp);

}

bt_node *make_bt_node(void) {
    bt_node *n_node = (bt_node *) malloc(sizeof(bt_node));
    n_node->left = NULL;
    n_node->right = NULL;
    n_node->parent = NULL;

    return n_node;
}
bt_data get_data(bt_node *bt) {
    return bt->data;
}
void set_data(bt_node *bt, bt_data data) {
    bt->data = data;
}
bt_node *get_left_sub_tree(bt_node *bt) {
    return bt->left;
}
bt_node *get_right_sub_tree(bt_node *bt) {
    return bt->right;
}
void make_left_sub_tree(bt_node *main, bt_node *sub) {
    if (main->left != NULL)
        free(main->left);

    main->left = sub;
}
void make_right_sub_tree(bt_node *main, bt_node *sub) {
    if (main->right != NULL)
        free(main->right);

    main->right = sub;
}
bt_node *remove_left_sub_tree(bt_node *bt) {
    bt_node *del_node;

    if (bt != NULL) {
        del_node = bt->left;
        bt->left = NULL;
    }

    return del_node;
}
bt_node *remove_right_sub_tree(bt_node *bt) {
    bt_node *del_node;

    if (bt != NULL) {
        del_node = bt->right;
        bt->right = NULL;
    }

    return del_node;
}
void change_left_sub_tree(bt_node *main, bt_node *sub) {
    main->left = sub;
}
void change_right_sub_tree(bt_node *main, bt_node *sub) {
    main->right = sub;
}

void bst_make_and_init(bt_node **p_root) {
    *p_root = NULL;
}
bst_data bst_get_data(bt_node *bst) {
    return get_data(bst);
}
void bst_insert(bt_node **p_root, bst_data data) {
    bt_node *p_node = NULL;
    bt_node *c_node = *p_root;
    bt_node *n_node = NULL;

    // check whether key value is unique
    while (c_node != NULL) {
        if (data == get_data(c_node))
            return;

        p_node = c_node;

        if (get_data(c_node) > data) {
            c_node = get_left_sub_tree(c_node);
        } else {
            c_node = get_right_sub_tree(c_node);
        }
    }

    n_node = make_bt_node();
    set_data(n_node, data);

    if (p_node != NULL) {
        if (data < get_data(p_node)) {
            make_left_sub_tree(p_node, n_node);
            n_node->parent = p_node;
        } else {
            make_right_sub_tree(p_node, n_node);
            n_node->parent = p_node;
        }
    } else {
        *p_root = n_node;
    }
}
bt_node *bst_search(bt_node *bst, bst_data target) {
    bt_node *c_node = bst;
    bst_data c_data;

    while (c_node != NULL) {
        c_data = get_data(c_node);

        if (target == c_data)
            return c_node;
        else if (target < c_data)
            c_node = get_left_sub_tree(c_node);
        else
            c_node = get_right_sub_tree(c_node);
    }

    return NULL;
}
bt_node bst_remove(bt_node *bst) {
    bt_node *c_node = bst;
    bt_node *p_node = c_node->parent;
    bt_node *d_node;

    d_node = c_node;

    // if delete node is leaf
    if (get_left_sub_tree(d_node) == NULL && get_right_sub_tree(d_node) == NULL) {
        if (get_left_sub_tree(p_node) == d_node)
            remove_left_sub_tree(p_node);
        else
            remove_right_sub_tree(p_node);
    } else if (get_left_sub_tree(d_node) == NULL || get_right_sub_tree(d_node) == NULL) {
        bt_node *dc_node;

        if (get_left_sub_tree(d_node) != NULL)
            dc_node = get_left_sub_tree(d_node);
        else
            dc_node = get_right_sub_tree(d_node);

        if (get_left_sub_tree(p_node) == d_node)
            change_left_sub_tree(p_node, dc_node);
        else
            change_right_sub_tree(p_node, dc_node);
    } else {
        bt_node *m_node = get_right_sub_tree(d_node);
        bt_node *mp_node = d_node;
        int del_data;

        while (get_left_sub_tree(m_node) != NULL) {
            mp_node = m_node;
            m_node = get_left_sub_tree(m_node);
        }

        del_data = get_data(d_node);
        set_data(d_node, get_data(m_node));

        if (get_left_sub_tree(mp_node) == m_node)
            change_left_sub_tree(mp_node, get_right_sub_tree(m_node));
        else
            change_right_sub_tree(mp_node, get_right_sub_tree(m_node));

        d_node = m_node;
        set_data(d_node, del_data);
        free(m_node);
    }
}
void attach(bt_node **p_root, bt_node *bt) {
    if (bt == NULL)
        return;

    bst_insert(p_root, bt->data);
    attach(p_root, bt->left);
    attach(p_root, bt->right);
}

void show_preorder(bt_node *bt) {

    if (bt == NULL)
        return;

    printf("%d ", bt->data);

    show_preorder(bt->left);
    show_preorder(bt->right);
}
void T3_preorder(bt_node *bt) {
    if (bt == NULL)
        return;

    T3_count++;

    T3_preorder(bt->left);
    T3_preorder(bt->right);
}
void T4_preorder(bt_node *bt) {
    if (bt == NULL)
        return;

    T4_count++;

    T4_preorder(bt->left);
    T4_preorder(bt->right);
}
void file_print(bt_node *bt) {
    if (bt == NULL)
        return;

    FILE *fp = fopen("hw3_output.txt", "a");

    if (!fp) {
        printf("Cannot open the file!");
        exit(-1);
    }

    if (bt->left != NULL || bt->right != NULL) {
        if (bt->left != NULL) {
            fprintf(fp, "%d %d\n", get_data(bt), get_data(bt->left));
            if (bt->right != NULL) {
                fprintf(fp, "%d %d\n", get_data(bt), get_data(bt->right));
            }
        } else {
            fprintf(fp, "%d %d\n", get_data(bt), get_data(bt->right));
        }
    }
    fclose(fp);

    file_print(bt->left);
    file_print(bt->right);
}
