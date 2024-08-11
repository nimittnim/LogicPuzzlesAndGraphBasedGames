#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<assert.h>


typedef char board_t[2][2][2][3];

typedef struct QueueNode {
    board_t board;
    struct QueueNode* next;
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

void enqueue(Queue* queue, board_t* board) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        perror("Error: Unable to allocate memory for a new queue node");
        exit(EXIT_FAILURE);
    }
    memcpy(newNode->board, *board, sizeof(board_t));
    newNode->next = NULL;

    if (queue->tail == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
}

board_t* dequeue(Queue* queue) {
    if (is_empty_queue(queue)) {
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    }

    board_t* board = (board_t*)malloc(sizeof(board_t));
    if (board == NULL) {
        perror("Error: Unable to allocate memory for the board");
        exit(EXIT_FAILURE);
    }

    memcpy(*board, queue->head->board, sizeof(board_t));
    QueueNode* temp = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp);
    queue->size--;
    return board;
}

void destroy_queue(Queue* queue) {
    while (!is_empty_queue(queue)) {
        board_t* board = dequeue(queue);
        free(board);
    }
    free(queue);
}

void print_queue(Queue* q) {
    QueueNode* current = q->head;
    while (current) {
        current = current->next;
    }
}



// Cube 

void is_valid(board_t board){
    return 1;
}

void is_same(board_t b1, board_t b2){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                for (int l = 0; l < 3; l++){
                    if (b1[i][j][k][l] != b2[i][j][k][l]){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

board_t shift(board_t board_p, int axis, int axis_value, int rot){
    board_t board;
    memcpy(board, board_p, sizeof(board));
    if (axis == 0){
        if (rot == 1){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[axis_value][0][0], sizeof(temp));
            memcpy(temp_2, board[axis_value][0][1], sizeof(temp));
            memcpy(temp_3, board[axis_value][1][0], sizeof(temp));
            memcpy(temp_4, board[axis_value][1][1], sizeof(temp));
            
            memcpy(board[axis_value][0][0], temp_2, sizeof(temp));
            memcpy(board[axis_value][0][1], temp_3, sizeof(temp));
            memcpy(board[axis_value][1][0], temp_4, sizeof(temp));
            memcpy(board[axis_value][1][1], temp_1, sizeof(temp));
        }
        else if (rot == 2){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[axis_value][0][0], sizeof(temp));
            memcpy(temp_2, board[axis_value][0][1], sizeof(temp));
            memcpy(temp_3, board[axis_value][1][0], sizeof(temp));
            memcpy(temp_4, board[axis_value][1][1], sizeof(temp));
            
            memcpy(board[axis_value][0][0], temp_2, sizeof(temp));
            memcpy(board[axis_value][0][1], temp_3, sizeof(temp));
            memcpy(board[axis_value][1][0], temp_4, sizeof(temp));
            memcpy(board[axis_value][1][1], temp_1, sizeof(temp));
            shift(board, axis, axis_value, 1);

        }
        else if (rot == -1){

        }
    }
    else if (axis == 1){
        if (rot == 1){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[0][axis_value][0], sizeof(temp));
            memcpy(temp_2, board[0][axis_value][1], sizeof(temp));
            memcpy(temp_3, board[1][axis_value][0], sizeof(temp));
            memcpy(temp_4, board[1][axis_value][1], sizeof(temp));
            
            memcpy(board[0][axis_value][0], temp_2, sizeof(temp));
            memcpy(board[0][axis_value][1], temp_3, sizeof(temp));
            memcpy(board[1][axis_value][0], temp_4, sizeof(temp));
            memcpy(board[1][axis_value][1], temp_1, sizeof(temp));
        }
        else if (rot == 2){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[0][axis_value][0], sizeof(temp));
            memcpy(temp_2, board[0][axis_value][1], sizeof(temp));
            memcpy(temp_3, board[1][axis_value][0], sizeof(temp));
            memcpy(temp_4, board[1][axis_value][1], sizeof(temp));
            
            memcpy(board[0][axis_value][0], temp_2, sizeof(temp));
            memcpy(board[0][axis_value][1], temp_3, sizeof(temp));
            memcpy(board[1][axis_value][0], temp_4, sizeof(temp));
            memcpy(board[1][axis_value][1], temp_1, sizeof(temp));
        }
        else if (rot == -1){

        }
    }
    else if (axis == 2){
        if (rot == 1){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[0][0][axis_value], sizeof(temp));
            memcpy(temp_2, board[0][1][axis_value], sizeof(temp));
            memcpy(temp_3, board[1][0][axis_value], sizeof(temp));
            memcpy(temp_4, board[1][1][axis_value], sizeof(temp));
            
            memcpy(board[0][0][axis_value], temp_2, sizeof(temp));
            memcpy(board[0][1][axis_value], temp_3, sizeof(temp));
            memcpy(board[1][0][axis_value], temp_4, sizeof(temp));
            memcpy(board[1][1][axis_value], temp_1, sizeof(temp));
        }
        else if (rot == 2){
            char temp[3];
            char temp_1[3];
            char temp_2[3];
            char temp_3[3];
            char temp_4[3];

            memcpy(temp_1, board[0][0][axis_value], sizeof(temp));
            memcpy(temp_2, board[0][1][axis_value], sizeof(temp));
            memcpy(temp_3, board[1][0][axis_value], sizeof(temp));
            memcpy(temp_4, board[1][1][axis_value], sizeof(temp));
            
            memcpy(board[0][0][axis_value], temp_2, sizeof(temp));
            memcpy(board[0][1][axis_value], temp_3, sizeof(temp));
            memcpy(board[1][0][axis_value], temp_4, sizeof(temp));
            memcpy(board[1][1][axis_value], temp_1, sizeof(temp));
            shift(board, axis, axis_value, 1);

        }
        else if (rot == -1){

        }
    }
    return board;
}

init_board(board_t board){
    board = (board_t) 
        {
            {
                {
                    {'r', 'g', 'w'},
                    {'r', 'b', 'w'}
                },
                {
                    {'r', 'g', 'w'},
                    {'r', 'g', 'w'}
                }
            },
            {
                {
                    {'r', 'g', 'w'},
                    {'r', 'g', 'w'}
                },
                {
                    {'r', 'g', 'w'},
                    {'r', 'g', 'w'}
                }
            }
        };
}

void solve(board_t board, board_t board_final){
    Queue* q = init_queue();
    enque(q, board);
    while (!is_empty_stack(q)) {
        board_t v = deque(q);
        if (is_same(v, board_final)) {
            return v;
        }
        for (int axis = 0; axis <= 2; axis++){
            for (int axis_value = 0; axis_value < 2; axis_value++){
                for (int rot = -1 ; rot < 2; rot ++){
                    board_t u = shift(v, axis, axis_value, rot);
                    if (is_valid(u)) {
                        enqueue(q, u);
                    }
                }
            }

        }
    }
}

int main(){
    board_t board;
    board_t board_final;
    init_board(board);
    solve(board, board_final);
    return 0;
}

// Cube