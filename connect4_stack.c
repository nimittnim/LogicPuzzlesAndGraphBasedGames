#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
// Stack
typedef struct StackNode {
    char value;
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

void push_stack(Stack* s, int val) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        perror("Memory Allocation for StackNode Failed");
        exit(EXIT_FAILURE);
    }
    newNode->value = val;
    newNode->next = s->head;
    s->head = newNode;
    s->size++;
}

int pop_stack(Stack* s) {
    if (s->head == NULL) {
        perror("Stack is Empty");
        exit(EXIT_FAILURE);
    }
    int temp = s->head->value;
    StackNode* tempNode = s->head;
    s->head = s->head->next;
    free(tempNode);
    s->size--;
    return temp;
}

void print_stack(Stack* s) {
    StackNode* current = s->head;
    while (current) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
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
//Stack


// Connect4 

/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
enum {
    EMPTY = 0,
    RED,
    BLUE,
};

typedef struct {
    Stack* cols[5];
}board_t;
typedef char player_t;
typedef struct {
    int col;
    int score;
} move_t;


#define MAX_ORD (28629151)

uint8_t computed_moves[31][31][31][31][31];

uint8_t encode_move(move_t move)
{
    int col = move.col;
    int score = move.score;
    uint8_t b = 0;
    if (!(0 <= col && col <= 6)) {
        printf("%d", col);
    }

    assert(0 <= col && col <= 6);  
    b |= col; 

    switch (score) {
    case -1: b |= 1 << 6; break;
    case 0: b |= 1 << 5; break;
    case 1: b |= 1 << 4; break;
    }
    return b;
}


move_t decode_move(uint8_t b)
{
    move_t m;

    m.col = (b & 0x7);
    if (b & 0x10) m.score = 1;
    if (b & 0x20) m.score = 0;
    if (b & 0x40) m.score = -1;
    return m;
}

int expo(int a, int ex){
    if (ex == 0){
        return 1;
    }
    int temp = a;
    for (int i = 0; i < ex-1; i++){
        a *= temp;
    }
    return a;
}


int* ord(board_t *board)
{
    int d;
    int* i = (int*)malloc(5 * sizeof(int));
    for (int col = 0; col < 5; col++) {
        Stack* current_col = board->cols[col];
        StackNode* current = current_col->head;
        int col_size = current_col-> size;
        if (col_size == 0){
            i[col] = 0;
            continue;
        }
        int shift = expo(2, col_size)-1;
        i[col] += shift;
        int p = 1;
        while (current){
            switch (current->value) {
            case 'B': d = 0; break;
            case 'R': d = 1; break;
            }
            i[col] += d * p;
            p *= 2;
            current = current->next;
        }
    }
    return i;
}

int is_full(board_t *board){
    for (int i = 0;i < 5;i++){
        Stack* current_col = board->cols[i];
        if (current_col->size < 4){
            return 0;
        }
    }
    return 1;
}

int has_won(board_t *board, player_t player) {
    // Check horizontally
    for (int row = 0; row < 4; row++) {
            int consecutive = 0;
            for (int i = 0; i < 5; i++) {
                StackNode* current;
                Stack* current_col = board->cols[i];

                if (current_col->size > row) {
                    current = current_col->head;
                    int j = 0;
                    while (j < current_col->size - row - 1) {
                        current = current->next;
                        j++;
                    }
                }
                else{
                    break;
                }

                if (current) {
                    if (current->value == player) {
                        consecutive++;
                        if (consecutive == 4) {
                            return 1; // Player has won
                        }
                    } else {
                        consecutive = 0;
                    }
                }
            }
        }



     // Check vertically
    for (int col = 0; col < 5; col++) {
        Stack* current_col = board->cols[col];
        StackNode* current = current_col->head;
        int consecutive = 0;

        while (current) {
            if (current->value == player) {
                consecutive++;
                if (consecutive == 4) {
                    return 1; // Player has won
                }
            } else {
                consecutive = 0;
            }
            current = current->next;
        }
    }

    // Check diagonally (left to right)
    for (int col = 0; col < 2; col++) {
        int consecutive = 0;
        for (int i = 0; i < 4; i++) {
            int curr_row =  i;
            int curr_col = col + i;
            int current_size = board->cols[curr_col]->size;
            StackNode* current;
            Stack* current_col = board->cols[curr_col];
            if (current_size > curr_row) {
                current = current_col->head;
                int j = 0;
                while (j < current_col->size - curr_row - 1) {
                    current = current->next;
                    j++;
                }
                if (current->value == player) {
                    consecutive++;
                    if (consecutive == 4) {
                        return 1; // Player has won
                    }
                } else {
                    consecutive = 0;
                }
            }
            else{
                break;
            }
        }
    }


    // Check diagonally (right to left)
        for (int col = 4; col > 2; col--) {
            int consecutive = 0;
            for (int i = 0; i < 4; i++) {
                int curr_row = i;
                int curr_col = col - i;
                int current_size = board->cols[curr_col]->size;
                StackNode* current;
                Stack* current_col = board->cols[curr_col];
                if (current_size > curr_row) {
                    current = current_col->head;
                    int j = 0;
                    while (j < current_col->size - curr_row - 1) {
                        current = current->next;
                        j++;
                    }
                    if (current->value == player) {
                        consecutive++;
                        if (consecutive == 4) {
                            return 1; // Player has won
                        }
                    } else {
                        consecutive = 0;
                    }
                }
                else{
                    break;
                }
            }
        }
    return 0;
}


player_t other_player(player_t player){
    player_t other;
    if (player == 'R'){
        other = 'B';
    }
    else{
        other = 'R';
    }
    return other;
}

int valid_move(board_t *board, int col, player_t player){
    if (col > 4 || col < 0){
        return 0;
    }
    if (board->cols[col]->size < 4){
        return 1;
    }
    else{
        return 0;
    }
}

void make_move(board_t *board, int col, player_t player){
    push_stack(board->cols[col], player); 
}

void undo_move(board_t *board, int col){
    pop_stack(board->cols[col]); 
}

void print_board(board_t *board){
    char b[5][4];
    for (int row = 0; row < 5; row++){
        Stack* current_col = board->cols[row];
        int col = 0;
        while ( col < 5 - current_col->size - 1){
            b[row][col] = '.';
            col++;
        }
        StackNode* current = current_col->head;
        while (current) {
        b[row][col] =  current->value;
        current = current->next;
        col++;
        }
    }
    for (int row = 0; row < 4; row++){
        for (int col = 0; col < 5; col++){
            printf("%2c ",b[col][row]);
        }
        printf("\n");
    }
}

move_t best_move(board_t *board, player_t player)
{
    move_t candidate;

    // print_board(board);

    // candidate = (move_t){
    //     .col = 4,
    //     .score = 1
    // };
    // return candidate;
    move_t response;
    
    int no_candidate = 1;



    // printf("===\n");
    // print_board(board);

 
    int* o = ord(board);
    // for (int i = 0; i < 5; i++){
    //     printf("%d ",o[i]);
    // }
    // printf("\n");
    
    if (computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]]) {
        return decode_move(computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]]);
    }
    // Checking if next move is winning
    for (int col = 0; col < 5; ++col){
        
        if (valid_move(board, col, player)){
            make_move(board, col, player);
            
            if (has_won(board, player)){
                undo_move(board, col);
                computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate = (move_t) {
                    .col = col,
                    .score = 1
                });
                // printf("%s\n","check");
                return candidate;
            }
            undo_move(board, col);
        }
    }

    // Finding best move based on other player's response
    for (int col = 0; col < 5; ++col){
        if (valid_move(board, col, player)){
            make_move(board, col, player);
            if (is_full(board)) {
                undo_move(board, col);
                computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate = (move_t) {
                .col = col,
                .score = 0
                });
                return candidate;
            }
            response = best_move(board, other_player(player));
            undo_move(board, col);
            if (response.score == -1){
                computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate = (move_t) {
                .col = col,
                .score = 1
                });
                return candidate;
            }
            else if (response.score == 0){
                computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate = (move_t) {
                .col = col,
                .score = 0
                });
                no_candidate = 0;
            }
            else{
                if (no_candidate){
                    computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate = (move_t) {
                    .col = col,
                    .score = -1
                    });
                }
                no_candidate = 0;
            }
        }
    }
    computed_moves[o[0]][o[1]][o[2]][o[3]][o[4]] = encode_move(candidate);
    return candidate;
}



board_t* intialize_board(){
    board_t *board = (board_t*)malloc(sizeof(board_t));
    for (int i = 0; i < 5; i++){
        board -> cols [i] = init_stack();
    }
    player_t player0 = 0;
    player_t player1 = 1;
    // make_move(board,0,player0);
    // make_move(board,0,player1);
    // make_move(board,0,player0);
    // make_move(board,0,player1);
    // make_move(board,1,player1);
    // make_move(board,1,player0);
    // make_move(board,1,player1);
    // make_move(board,1,player0);
    // make_move(board,2,player0);
    // make_move(board,2,player1);
    // make_move(board,2,player0);
    // make_move(board,2,player1);
    // make_move(board,3,player0);
    // make_move(board,3,player1);
    // make_move(board,3,player0);
    // make_move(board,3,player1);
    return board;
}

void print_key(){
    for (int col = 0; col < 5; ++col){
        printf("%2d ",col);
    }
    printf("\n");
}


void user_vs_user(){
    int move,col;
    player_t current = 'B';
    board_t *board = intialize_board();
    move_t response;
    while(1){
        print_board(board);
        printf("Enter your move : \n");
        print_key();
        scanf("%d",&move);
        int col = move;
        if (valid_move(board, col, current)){
        push_stack(board->cols[col],current);
        if (has_won(board, current)){
            printf("\n %c won\n",current);
            print_board(board);
            break;
        }
        }    
        current = other_player(current);  
    }
}

void user_vs_computer(){
    int move,col;
    player_t current = 'B';
    board_t *board = intialize_board();
    move_t response;
    player_t player;
    printf("Choose player : \n R : play second \n B : play first\n");
    scanf("%s",&player);
   
    while(1){
        if (current == player){
            while (1){
            print_board(board);
            printf("Enter your move : \n");
            print_key();
            scanf("%d",&move);
            col = move;
            if (valid_move(board, col, current)){
                make_move(board, col, current);
                break;
            }    
            else{
                printf("%s","\nIncorrect Move, please renter correct move \n");
            }
            }
        }
        else {
            response = best_move(board,current);
            if (valid_move(board, response.col, current)){
            make_move(board, response.col ,current);
            }
        }
        if (has_won(board,current)){
            print_board(board);
            printf("Player %c won",current);
            break;
        }
        if (is_full(board)){
            print_board(board);
            printf("Draw");
            break;
        }
        current = other_player(current);     
    }
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    printf("%s","Mode of Game :  \n 0 : User vs Computer \n 1 : User vs User\n");
    int mode;
    int move,col;
    player_t current = 'B';
    board_t *board = intialize_board();
    move_t response;
    scanf("%d",&mode);
    if (mode == 0){
        user_vs_computer();
    }
    else if (mode == 1){
        user_vs_user();
    }
    return 0;
}

// Connect4 
