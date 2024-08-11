#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char b[6][2][2];
} board_t;

typedef struct Move {
    int move;
    int rot;
} Move;

typedef struct QueueNode {
    board_t board;
    struct QueueNode* next;
    struct Move move;
} QueueNode;



typedef struct Queue {
    int size;
    QueueNode* head;
    QueueNode* tail;
} Queue;

Queue* init_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        perror("Error: Unable to allocate memory for the queue");
        exit(EXIT_FAILURE);
    }
    queue->head = queue->tail = NULL;
    queue->size = 0;
    return queue;
}

int is_empty_queue(Queue* queue) {
    return (queue->head == NULL);
}

void enqueue_with_move(Queue* queue, board_t board, Move move) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        perror("Error: Unable to allocate memory for a new queue node");
        exit(EXIT_FAILURE);
    }
    newNode->board = board;
    newNode->move = move;
    newNode->next = NULL;

    if (queue->tail == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
}

Move dequeue_with_move(Queue* queue, board_t* board) {
    if (is_empty_queue(queue)) {
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    }

    *board = queue->head->board;
    Move move = queue->head->move;

    QueueNode* temp = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp);
    queue->size--;
    return move;
}

void destroy_queue(Queue* queue) {
    while (!is_empty_queue(queue)) {
        board_t board;
        dequeue_with_move(queue, &board);
    }
    free(queue);
}

void print_queue(Queue* q) {
    QueueNode* current = q->head;
    while (current) {
        current = current->next;
    }
}

void print_board(board_t board) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                printf("%c", board.b[i][j][k]);
            }
        }
    }
}

int is_valid(board_t board) {
    return 1;
}

int is_same(board_t b1, board_t b2) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                if (b1.b[i][j][k] != b2.b[i][j][k]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

board_t make_move(board_t board_p, int move, int rot) {
    board_t board = board_p;

    int move_faces[3][2][4] = { { {0, 4, 2, 5}, {0, 5, 2, 4} }, { {0, 1, 2, 3}, {0, 3, 2, 1} },
        { {1, 4, 3, 5}, {1, 5, 3, 4} }
    };
    if (rot == 1 || rot == 0) {
        int x_1, x_2, y_1, y_2;
        if (move == 0 || move == 2) {
            x_1 = x_2 = y_2 = 1;
            y_1 = 0;
        } else if (move == 1) {
            y_1 = y_2 = x_2 = 1;
            x_1 = 0;
        }
        char temp_1 = board.b[move_faces[move][rot][0]][x_1][y_1];
        char temp_2 = board.b[move_faces[move][rot][0]][x_2][y_2];
        for (int i = 0; i < 3; i++) {
            if (move == 2) {
                if (i == 0 || i == 2) {
                    board.b[move_faces[move][rot][i]][x_2][y_1] = board.b[move_faces[move][rot][i + 1]][y_1][y_1];
                    board.b[move_faces[move][rot][i]][x_2][x_2] = board.b[move_faces[move][rot][i + 1]][x_2][y_1];
                } else if (i == 1) {
                    board.b[move_faces[move][rot][i]][y_1][y_1] = board.b[move_faces[move][rot][i + 1]][x_2][y_1];
                    board.b[move_faces[move][rot][i]][x_2][y_1] = board.b[move_faces[move][rot][i + 1]][x_2][x_2];
                }
                continue;
            }
            board.b[move_faces[move][rot][i]][x_1][y_1] = board.b[move_faces[move][rot][i + 1]][x_1][y_1];
            board.b[move_faces[move][rot][i]][x_2][y_2] = board.b[move_faces[move][rot][i + 1]][x_2][y_2];
        }
        if (move == 2) {
            x_2 = 1;
            y_1 = y_2 = x_1 = 0;
        }
        board.b[move_faces[move][rot][3]][x_1][y_1] = temp_1;
        board.b[move_faces[move][rot][3]][x_2][y_2] = temp_2;
    }
    if (rot == 2) {
        make_move(board, move, 1);
        make_move(board, move, 1);
    }

    return board;
}

void init_board(board_t* board) {
    *board = (board_t) {
        { { {'r', 'r'}, {'r', 'r'} },
            { {'g', 'g'}, {'g', 'g'} },
            { {'b', 'b'}, {'b', 'b'} },
            { {'o', 'o'}, {'o', 'o'} },
            { {'w', 'w'}, {'w', 'w'} },
            { {'y', 'y'}, {'y', 'y'} }
        }
    };
}

void print_move(int move, int rot) {
    char* move_names[3][3] = { { "Rp", "R1", "R2" }, { "Up", "U1", "U2" }, { "Fp", "F1", "F2" } };
    printf("%s -> ", move_names[move][rot]);
}

void solve(board_t board, board_t board_final) {
    Queue* q = init_queue();
    enqueue_with_move(q, board, (Move) { -1, -1 });

    while (!is_empty_queue(q)) {
        board_t v;
        Move last_move = dequeue_with_move(q, &v);
        if (is_same(v, board_final)) {
            // Print the sequence of moves
            printf("Solved! Sequence of moves:\n");
            while (last_move.move != -1) {
                print_move(last_move.move, last_move.rot);
                v = make_move(v, last_move.move, 3 - last_move.rot); // Reverse the move
                last_move = dequeue_with_move(q, &v);
            }
            printf("\n");
            return;
        }

        for (int move = 0; move < 3; move++) {
            for (int rot = 0; rot < 3; rot++) {
                board_t u = make_move(v, move, rot);
                if (is_valid(u)) {
                    // Enqueue the move along with the state
                    enqueue_with_move(q, u, (Move) { move, rot });
                }
            }
        }
    }
    printf("No solution found.\n");
}

int main() {
    board_t board;
    board_t board_final;
    init_board(&board);
    init_board(&board_final);

    printf("Starting board:\n");
    board = make_move(board, 0, 0);
    print_board(board);
    printf("\n");

    solve(board, board_final);
    return 0;
}
