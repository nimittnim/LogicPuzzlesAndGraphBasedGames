#include<stdio.h>
#include<stdlib.h>

typedef struct{
    char b[6][2][2];
}board_t;

typedef struct{
    char b[8][3];
}ord_board_t;

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

void enqueue(Queue* queue, board_t board) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        perror("Error: Unable to allocate memory for a new queue node");
        exit(EXIT_FAILURE);
    }
    newNode->board = board;
    newNode->next = NULL;

    if (queue->tail == NULL) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->size++;
}

board_t dequeue(Queue* queue) {
    if (is_empty_queue(queue)) {
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    }

    board_t board = queue->head->board;

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
        dequeue(queue); 
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

void print_board(board_t board){
    char my_board[8][6];
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            my_board[i][j] = ' ';
        }
    }
    my_board[2][2] = board.b[0][0][1];
    my_board[2][3] = board.b[0][1][1];
    my_board[3][2] = board.b[0][0][0];
    my_board[3][3] = board.b[0][1][0];

    my_board[2][4] = board.b[3][0][1];
    my_board[2][5] = board.b[3][1][1];
    my_board[3][4] = board.b[3][0][0];
    my_board[3][5] = board.b[3][1][0];

    my_board[2][0] = board.b[1][0][1];
    my_board[2][1] = board.b[1][1][1];
    my_board[3][0] = board.b[1][0][0];
    my_board[3][1] = board.b[1][1][0];

    my_board[0][2] = board.b[4][0][1];
    my_board[0][3] = board.b[4][1][1];
    my_board[1][2] = board.b[4][0][0];
    my_board[1][3] = board.b[4][1][0];

    my_board[4][2] = board.b[5][0][1];
    my_board[4][3] = board.b[5][1][1];
    my_board[5][2] = board.b[5][0][0];
    my_board[5][3] = board.b[5][1][0];

    my_board[6][2] = board.b[2][0][1];
    my_board[6][3] = board.b[2][1][1];
    my_board[7][2] = board.b[2][0][0];
    my_board[7][3] = board.b[2][1][0];

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            printf("%2c",my_board[i][j]);
        }
        printf("\n");
    }
}

int factorial(int m){
    if (m == 0){
        return 1;
    }
    return m*factorial(m-1);
}

ord_board_t alphabetically_sort(ord_board_t board){
        for (int k = 0; k < 8; k++){
        for (int i = 0; i < 2; i++){
        if (board.b[k][i] > board.b[k][i+1]){
            char temp = board.b[k][i];
            board.b[k][i] = board.b[k][i+1];
            board.b[k][i+1] = temp;
        }
        }
        if (board.b[k][0] > board.b[k][1]){
            char temp = board.b[k][0];
            board.b[k][0] = board.b[k][1];
            board.b[k][1] = temp;
        }
    }
    return board;
}

void print_ord_board(ord_board_t board){
    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 3; j++){
            printf("%c",board.b[i][j]);
        }
        printf("%c",' ');
    }
    printf("\n");
}

int* ord(board_t board){
    
    int* ind = (int*)malloc(2*sizeof(int));

    // printf("\n");
    // print_board(board);
    // printf("\n");
    ord_board_t solved_ord_board = (ord_board_t ){{{'r','w','b'},{'r','b','y'},{'r','y','g'},{'r','g','w'},
                                                   {'o','w','g'},{'o','g','y'},{'o','y','b'},{'o','b','w'}}};

    ord_board_t alphabetically_sorted_solved_ord_board = alphabetically_sort(solved_ord_board);


    ord_board_t ord_board;
    ord_board.b[0][0] = board.b[0][0][0];
    ord_board.b[0][1] = board.b[5][0][1];
    ord_board.b[0][2] = board.b[1][1][0];

    ord_board.b[1][0] = board.b[0][0][1];
    ord_board.b[1][1] = board.b[1][1][1];
    ord_board.b[1][2] = board.b[4][0][0];

    ord_board.b[2][0] = board.b[0][1][1];
    ord_board.b[2][1] = board.b[4][1][0];
    ord_board.b[2][2] = board.b[3][0][1];

    ord_board.b[3][0] = board.b[0][1][0];
    ord_board.b[3][1] = board.b[3][0][0];
    ord_board.b[3][2] = board.b[5][1][1];

    ord_board.b[4][0] = board.b[2][1][1];
    ord_board.b[4][1] = board.b[5][1][0];
    ord_board.b[4][2] = board.b[3][1][0];

    ord_board.b[5][0] = board.b[2][1][0];
    ord_board.b[5][1] = board.b[3][1][1];
    ord_board.b[5][2] = board.b[4][1][1];

    ord_board.b[6][0] = board.b[2][0][0];
    ord_board.b[6][1] = board.b[4][0][1];
    ord_board.b[6][2] = board.b[1][0][1];

    ord_board.b[7][0] = board.b[2][0][1];
    ord_board.b[7][1] = board.b[1][0][0];
    ord_board.b[7][2] = board.b[5][0][0];
    
    // print_ord_board(solved_ord_board);
    // print_ord_board(ord_board);
    ord_board_t alphabetically_sorted_ord_board = alphabetically_sort(ord_board);
    // print_ord_board(alphabetically_sorted_solved_ord_board);
    // print_ord_board(alphabetically_sorted_ord_board);

    int p = 1;
    int d;
    int seen[8] = { 0,0,0,0,0,0,0,0 };
    int a = 7;
    int per = 0;
    int ori = 0;
    for (int i = 0; i < 8; i++){
        int found_match = 0;
        for (int j = 0; j < 8; j++){
            int matched = 1;
            int k_1 = 0;
            for (int k = 0; k < 3; k++){
                if (alphabetically_sorted_ord_board.b[i][k] != alphabetically_sorted_solved_ord_board.b[j][k]){
                    k_1 = k;
                    matched = 0;
                    break;
                }
            }
            if (matched == 0){
                continue;
            }
            found_match = 1;
            if (ord_board.b[i][0] == alphabetically_sorted_solved_ord_board.b[i][0]){
                d = 0;
            }
            else if (ord_board.b[i][0] == alphabetically_sorted_solved_ord_board.b[i][1]){
                d = 1;
            }
            else { // (ord_board.b[i][k] == alphabetically_sorted_solved_ord_board.b[i][2])
                d = 2;
            }
            int om = 0;
            for (int m = 0; m < j; m++){
                if (!seen[m]){
                    ++om;
                }
                
            }
            ori = ori + p*d;
            seen[j] = 1;
            // printf("%d ",per);
            per = per + om*factorial(a);
            
            p = p*3;

            // printf("%d ",j);
            a--;
            break;
        }
        if (found_match == 0){
            ind[0] = -1;
            ind[1] = -1;
            return ind; // cube is unsolvable
        }
        
    }
    ind[0] = per;
    ind[1] = ori;
    return ind;
}

int is_valid(board_t board){
    return 1;
}

int is_same(board_t b1, board_t b2){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                if (b1.b[i][j][k] != b2.b[i][j][k]){
                    return 0;
                }
            }
        }
    }
    return 1;
}

board_t make_move(board_t board_p, int move, int rot){

    // move :
    // 0 - R
    // 1 - U
    // 2 - F
    // rot :
    // 0 - counter clockwise 
    // 1 - clockwise   
    // 2 - clockwise * 2

    board_t board = board_p;

    // move_faces :
    // 0 : R
    // 1 : U

    // 6 : face
    // 0 - front face
    // 1 - left face
    // 2 - back face
    // 3 - right face
    // 4 - top face
    // 5 - bottom face
    // 2 : x
    // 0 : left 
    // 1 : right
    // 2 : y
    // 0 : bottom
    // 1 : top
    int rotating_face[3] = {3, 4, 0};
    if (rot == 0){
        int temp = board.b[rotating_face[move]][0][0]; 
        board.b[rotating_face[move]][0][0] = board.b[rotating_face[move]][0][1];
        board.b[rotating_face[move]][0][1] = board.b[rotating_face[move]][1][1];
        board.b[rotating_face[move]][1][1] = board.b[rotating_face[move]][1][0];
        board.b[rotating_face[move]][1][0] = temp;
    }
    else if (rot == 1){
        int temp = board.b[rotating_face[move]][0][0]; 
        board.b[rotating_face[move]][0][0] = board.b[rotating_face[move]][1][0];
        board.b[rotating_face[move]][1][0] = board.b[rotating_face[move]][1][1];
        board.b[rotating_face[move]][1][1] = board.b[rotating_face[move]][0][1];
        board.b[rotating_face[move]][0][1] = temp;
    }
    else{
        make_move(board, move, 1);
        make_move(board, move, 1);
    }
    if (rot == 0){
        if (move == 0){
            int faces[] = {0,4,2,5};
            int x1 = 1;
            int y1 = 0;
            int x2 = 1;
            int y2 = 1;
            char temp1 = board.b[faces[0]][x1][y1];
            char temp2 = board.b[faces[0]][x2][y2];
            for (int i = 0; i < 3; i++){
                board.b[faces[i]][x1][y1] = board.b[faces[i+1]][x1][y1]; 
                board.b[faces[i]][x2][y2] = board.b[faces[i+1]][x2][y2];
            }
            board.b[faces[3]][x1][y1] = temp1;
            board.b[faces[3]][x2][y2] = temp2;
        }
        if(move == 1){
            char temp1 = board.b[0][0][1];
            char temp2 = board.b[0][1][1];
            board.b[0][0][1] = board.b[1][0][1];
            board.b[0][1][1] = board.b[1][1][1];
            board.b[1][0][1] = board.b[2][1][0];
            board.b[1][1][1] = board.b[2][0][0];
            board.b[2][0][0] = board.b[3][1][1];
            board.b[2][1][0] = board.b[3][0][1];
            board.b[3][0][1] = temp1;
            board.b[3][1][1] = temp2;
        }
        if (move == 2){
            char temp1 = board.b[1][1][0];
            char temp2 = board.b[1][1][1];
            board.b[1][1][0] = board.b[4][0][0];
            board.b[1][1][1] = board.b[4][1][0];
            board.b[4][0][0] = board.b[3][0][1];
            board.b[4][1][0] = board.b[3][0][0];
            board.b[3][0][1] = board.b[5][1][1];
            board.b[3][0][0] = board.b[5][0][1];
            board.b[5][1][1] = temp1;
            board.b[5][0][1] = temp2;
        }
    }
    if (rot == 1){
        if (move == 0){
            int faces[] = {0,5,2,4};
            int x1 = 1;
            int y1 = 0;
            int x2 = 1;
            int y2 = 1;
            char temp1 = board.b[faces[0]][x1][y1];
            char temp2 = board.b[faces[0]][x2][y2];
            for (int i = 0; i < 3; i++){
                board.b[faces[i]][x1][y1] = board.b[faces[i+1]][x1][y1]; 
                board.b[faces[i]][x2][y2] = board.b[faces[i+1]][x2][y2];
            }
            board.b[faces[3]][x1][y1] = temp1;
            board.b[faces[3]][x2][y2] = temp2;
        }
        if(move == 1){
            char temp1 = board.b[0][0][1];
            char temp2 = board.b[0][1][1];
            board.b[0][0][1] = board.b[3][0][1];
            board.b[0][1][1] = board.b[3][1][1];
            board.b[3][0][1] = board.b[2][1][0];
            board.b[3][1][1] = board.b[2][0][0];
            board.b[2][0][0] = board.b[1][1][1];
            board.b[2][1][0] = board.b[1][0][1];
            board.b[1][0][1] = temp1;
            board.b[1][1][1] = temp2;
        }
        if (move == 2){
            char temp1 = board.b[1][1][0];
            char temp2 = board.b[1][1][1];
            board.b[1][1][0] = board.b[5][1][1];
            board.b[1][1][1] = board.b[5][0][1];
            board.b[5][0][1] = board.b[3][0][0];
            board.b[5][1][1] = board.b[3][0][1];
            board.b[3][0][1] = board.b[4][0][0];
            board.b[3][0][0] = board.b[4][1][0];
            board.b[4][0][0] = temp1;
            board.b[4][1][0] = temp2;
        }
    }
    if (rot == 2){
        board = make_move(board,move,1);
        board = make_move(board,move,1);
    }
    return board;
}

void init_board(board_t* board){

    *board = (board_t){
        {
            {
                {'r', 'r'},
                {'r', 'r'}
            },
            {
                {'b', 'b'},
                {'b', 'b'}
            },
            {
                {'o', 'o'},
                {'o', 'o'}
            },
            {
                {'g', 'g'},
                {'g', 'g'}
            },
            {
                {'y', 'y'},
                {'y', 'y'}
            },
            {
                {'w', 'w'},
                {'w', 'w'}
            }
        }
    };
}

void print_move(int move, int rot){
    char* move_names[3][3] = {{"Rp","R1","R2"},{"Up","U1","U2"},{"Fp","F1","F2"}};
    printf("%s ",move_names[move][rot]);
}

typedef struct{
    int axis;
    int rot;
    struct Node* next;
}Node;

typedef struct{
    Node* head;
}Moves;

Moves* init_moves(){
    Moves* moves = (Moves*)malloc(sizeof(Moves));
    moves->head = NULL;
    return moves;
}

board_t twist(board_t board){
    char temp = board.b[0][0][1];
    board.b[0][0][1] = board.b[1][1][1];
    board.b[1][1][1] = board.b[4][0][0];
    board.b[4][0][0] = temp;
    return board;
}

Moves* solve(board_t board, board_t board_final) {
    Moves* moves = init_moves();
    int solving_moves[15];
    for (int i = 0; i < 15; i++){
        solving_moves[i] = -1;
    }
    int** visited = (int**)malloc(40320 * sizeof(int*));
    for (int i = 0; i < 40320; i++) {
        visited[i] = (int*)malloc(6561 * sizeof(int));
    }

    board_t** parent = (board_t**)malloc(40320 * sizeof(board_t*));
    for (int i = 0; i < 40320; i++) {
        parent[i] = (board_t*)malloc(6561 * sizeof(board_t));
    }
    Queue* q = init_queue();
    int* o = ord(board);
    visited[o[0]][o[1]] = 1;
    enqueue(q, board);
    board_t start = board;
    while (!is_empty_queue(q)) {
        board_t v = dequeue(q);
        if (is_same(v, board_final)) {
            board_t c = v;
            int* par = ord(c);
            int a = 0;
            while (!is_same(c, board)) {
                Node* n = (Node*)malloc(sizeof(Node));
                n->axis = visited[par[0]][par[1]]/3;
                n->rot = visited[par[0]][par[1]]%3;
                n->next = moves->head;
                moves->head = n;
                c = parent[par[0]][par[1]];
                par = ord(c);
                a++;
            }
            return moves;
        }
        for (int axis = 0; axis < 3; axis++){
            for (int rot = 0 ; rot < 3; rot ++){
                board_t u = make_move(v, axis, rot);
                if (is_valid(u)) {
                    int* or = ord(u);
                    if (!visited[or[0]][or[1]]){
                        enqueue(q, u);
                        parent[or[0]][or[1]] = v;
                        visited[or[0]][or[1]] = 3*(axis) + rot;
                    }
                }
            }
        }    
    }
    return moves;
}

board_t scramble_board(board_t board){
    board = make_move(board, 0, 1);
    board = make_move(board, 1, 1);
    board = make_move(board, 2, 1);
    board = make_move(board, 0, 1);
    board = make_move(board, 0, 1);
    board = make_move(board, 1, 1);
    board = make_move(board, 2, 1);
    board = make_move(board, 0, 1);
    board = make_move(board, 1, 0);
    board = make_move(board, 2, 0);
    board = make_move(board, 0, 0);
    return board;
}

board_t random_scramble_board(){
    board_t board;
    init_board(&board);
    for (int i = 0; i < 20; i++){
        int a = (rand())%3;
        int b = (rand())%3;
        board = make_move(board, a,b);
    }
    return board;
}

int main(){
    board_t board;
    board_t board_final;
    init_board(&board);
    init_board(&board_final);
    board = scramble_board(board);
    // board = twist(board);
    int mode;
    printf("Choose the Input Mode : \n");
    printf("0 : Standard Initial Input\n");
    printf("1 : For giving your input\n");
    printf("2 : Random Input\n");
    printf("3 : Unsolvable Input\n");
    scanf("%d",&mode);
    if (mode == 0){
        init_board(&board);
        board = scramble_board(board);
    }
    else if (mode == 1){
        printf("Update the board in init_board function\n");
        return 0;
    }
    else if (mode == 2){
        board = random_scramble_board();
    }
    else if (mode == 3){
        init_board(&board);
        board = twist(board);
    }
    printf("Initial Configuration : \n");
    print_board(board);
    int * i = ord(board);
    if (i[0] == -1){
        printf("\nUnsolvable.");
        return 0;
    }

    Moves* moves = solve(board,board_final); 
    if (moves->head == NULL){
        printf("Unsolvable.");
        return 0;
    }
    printf("\n Moves to solve : \n");
    Node* current = moves->head;
    while (current){
        Node* next_current = current->next; 
        if (current->next && current->axis == next_current->axis) {
            int rot_1 = current->rot;
            int rot_2 = next_current->rot;
            if (rot_1 == 0){
                rot_1 = -1;
            }
            if (rot_2 == 0){
                rot_2 = -1;
            }
            int net_rot = (rot_1 + rot_2);
            if (net_rot == -1){
                net_rot = 0;
            }
            else if (net_rot == 1){
                net_rot = 1;
            }
            else if (net_rot == 2){
                net_rot = 2;
            }
            else if (net_rot == 3){
                net_rot = 0;
            }
            print_move(current->axis, net_rot);
            board = make_move(board, current->axis, net_rot);
            current = current->next;
            current = current->next;
            continue;
        }
        print_move(current->axis, current->rot);
        board = make_move(board, current->axis, current->rot);
        current = current->next;
    }
    printf("\n\nSolved :\n");
    print_board(board);
    return 0;
}

// Cube