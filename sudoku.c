#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int b[9][9]; // 0 is the blank cell.
} Sudoku_board;

// Stack
typedef struct StackNode {
    Sudoku_board sb;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* head;
    int size;
} Stack;

Stack* init_stack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (s == NULL) {
        perror("Memory Allocation for Stack failed");
        exit(EXIT_FAILURE);
    }
    s->head = NULL;
    s->size = 0;
    return s;
}

void push_stack(Stack* s, Sudoku_board sb) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        perror("Memory Allocation for StackNode Failed");
        exit(EXIT_FAILURE);
    }
    newNode->sb = sb;
    newNode->next = s->head;
    s->head = newNode;
    s->size++;
}

Sudoku_board pop_stack(Stack* s) {
    if (s->head == NULL) {
        perror("Stack is Empty");
        exit(EXIT_FAILURE);
    }
    Sudoku_board temp = s->head->sb;
    StackNode* tempNode = s->head;
    s->head = s->head->next;
    free(tempNode);
    s->size--;
    return temp;
}

int is_empty_stack(Stack* s) {
    return s->head == NULL;
}

void destroy_stack(Stack* s) {
    while (!is_empty_stack(s)) {
        pop_stack(s);
    }
    free(s);
}
// Stack

void clear(int seen[10]) {
    for (int i = 0; i < 10; ++i) {
        seen[i] = 0;
    }
}

int is_valid(const Sudoku_board s) {
    int seen[10];
    for (int row = 0; row < 9; ++row) {
        clear(seen);
        for (int col = 0; col < 9; ++col) {
            int v = s.b[row][col];
            if (v && seen[v]) { return 0; }
            seen[v] = 1;
        }
    }
    for (int col = 0; col < 9; ++col) {
        clear(seen);
        for (int row = 0; row < 9; ++row) {
            int v = s.b[row][col];
            if (v && seen[v]) { return 0; }
            seen[v] = 1;
        }
    }
    int boxrow = 0;
    int boxcol = 0;
    while (boxrow < 9) {
        while (boxcol < 9) {
            clear(seen);
            for (int r = 0; r < 3; ++r) {
                for (int c = 0; c < 3; ++c) {
                    int v = s.b[boxrow + r][boxcol + c];
                    if (v && seen[v]) { return 0; }
                    seen[v] = 1;
                }
            }
            boxcol += 3;
        }
        boxrow += 3;
        boxcol = 0;
    }
    return 1;
}

Sudoku_board place(const Sudoku_board s, int i, int j, int v) {
    Sudoku_board t = s;
    assert(t.b[i][j] == 0);
    t.b[i][j] = v;
    return t;
}

void print_board(const Sudoku_board s) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int v = s.b[r][c];
            if (v) printf("%d ", s.b[r][c]);
            else printf(". ");
        }
        printf("\n");
    }
}

void read_board(Sudoku_board* s) {
    int v;
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            scanf("%d", &v);
            s->b[r][c] = v;
        }
    }
}

int is_full(const Sudoku_board s) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (s.b[r][c] == 0) return 0;
        }
    }
    return 1;
}

int* find_first_blank(const Sudoku_board s) {
    static int pos[2];
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (s.b[r][c] == 0) {
                pos[0] = r;
                pos[1] = c;
                return pos;
            }
        }
    }
    assert(0);
}

Sudoku_board solve(const Sudoku_board s) {
    Stack* st = init_stack();
    push_stack(st, s);
    while (!is_empty_stack(st)) {
        Sudoku_board v = pop_stack(st);
        if (is_full(v)) {
            return v;
        }
        int* pos = find_first_blank(v);
        for (int val = 1; val <= 9; ++val) {
            Sudoku_board u = place(v, pos[0], pos[1], val);
            if (is_valid(u)) {
                push_stack(st, u);
            }
        }
    }
    assert(0);
}

void init_sudoku_board(Sudoku_board* s) {
    *s = (Sudoku_board){
        {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        }
    };
}

int main() {
    Sudoku_board s;
    init_sudoku_board(&s);
    print_board(s);
    printf("===\n");
    Sudoku_board t = solve(s);
    print_board(t);
    return 0;
}