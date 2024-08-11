#include<stdio.h>
#include<stdlib.h>

typedef struct{
    char faces[6];
}cube_t;

typedef struct{
    cube_t filled[8];
    int empty;
}board_t;

// Queue

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

// Queue

// Up it Up

int* ord(board_t board){
    int* ind = (int*)malloc(2*sizeof(int));
    int p = 1;
    int a = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            if (board.filled[i].faces[j] == 'c'){
                a = a + p*j;
                p = p*6;
                break;
            }
        }
    }
    ind[0] = a;
    ind[1] = board.empty;
    return ind;
}

void print_board(board_t board){
    char b[9];
    for (int i = 0; i < 9; i++){
        b[i] = 'n';
    }
    b[board.empty] = 'e';

    int i = 0;
    int j = 0;
    while( i < 9){
        if (b[i] == 'e'){
            i++;
            continue;
        }
        b[i] = board.filled[j].faces[0];
        i++;
        j++;
    }
    for (int k = 0; k < 9; k++){
        if ( k == 3 || k == 6){
            printf("\n");
        }
        printf("%c",b[k]);
    }
    printf("\n\n");
}

int is_won(board_t board){
    if (board.empty != 4){
        return 0;
    }
    for (int i = 0; i < board.empty; i++){
        if (board.filled[i].faces[0] != 'g'){
            return 0;
        }
    }
    for (int j = board.empty; j < 8; j++){
        if (board.filled[j].faces[0] != 'g'){
            return 0;
        }
    }
    return 1;
}

board_t up(const board_t b)
{
    int e = b.empty;
    int up = e-3;
    if (up < 0 || up > 5) {return b;}
    board_t o = b;
    o.empty = up;
    cube_t temp = o.filled[e-1];
    o.filled[e-1].faces[0] = b.filled[up].faces[3];
    o.filled[e-1].faces[1] = b.filled[up].faces[0];
    o.filled[e-1].faces[2] = b.filled[up].faces[2];
    o.filled[e-1].faces[3] = b.filled[up].faces[5];
    o.filled[e-1].faces[4] = b.filled[up].faces[4];
    o.filled[e-1].faces[5] = b.filled[up].faces[1];

    o.filled[e-3] = o.filled[e-2];
    o.filled[e-2] = temp;
    return o;
}

board_t down(const board_t b)
{
    int e = b.empty;
    if (e+3 > 8) {return b;}
    board_t o = b;
    o.empty = e+3;
    cube_t temp = o.filled[e];
    o.filled[e].faces[0] = b.filled[e+2].faces[1];
    o.filled[e].faces[1] = b.filled[e+2].faces[5];
    o.filled[e].faces[2] = b.filled[e+2].faces[2];
    o.filled[e].faces[3] = b.filled[e+2].faces[0];
    o.filled[e].faces[4] = b.filled[e+2].faces[4];
    o.filled[e].faces[5] = b.filled[e+2].faces[3];

    o.filled[e+2] = o.filled[e+1];
    o.filled[e+1] = temp;
    return o;
}

board_t left(const board_t b)
{
    int e = b.empty;
    int left = e-1;
    if (left == 2 || left == 5 || left == 8 || left < 0 ) {return b;}
    board_t o = b;
    o.empty = left;
    cube_t temp = o.filled[e-1];
    o.filled[e-1].faces[0] = b.filled[left].faces[2];
    o.filled[e-1].faces[1] = b.filled[left].faces[1];
    o.filled[e-1].faces[2] = b.filled[left].faces[5];
    o.filled[e-1].faces[3] = b.filled[left].faces[3];
    o.filled[e-1].faces[4] = b.filled[left].faces[0];
    o.filled[e-1].faces[5] = b.filled[left].faces[4];

    return o;
}

board_t right(const board_t b)
{
    int e = b.empty;
    int right = e+1;
    if (right > 8 || right == 0 || right == 3 || right == 6) {return b;}
    board_t o = b;
    o.empty = right;
    o.filled[e].faces[0] = b.filled[e].faces[4];
    o.filled[e].faces[1] = b.filled[e].faces[1];
    o.filled[e].faces[2] = b.filled[e].faces[0];
    o.filled[e].faces[3] = b.filled[e].faces[3];
    o.filled[e].faces[4] = b.filled[e].faces[5];
    o.filled[e].faces[5] = b.filled[e].faces[2];

    return o;
}

int is_same(board_t b1, board_t b2){
    if (b1.empty != b2.empty){
        return 0;
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j< 6; j++){
            if (b1.filled[i].faces[j] != b2.filled[i].faces[j]){
                return 0;
            }
        }
    }
    return 1;
}

typedef struct{
    char m;
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

Moves* solve(board_t board) {
    Moves* moves = init_moves();
    char** visited = (char**)malloc(1679616 * sizeof(char*));
    for (int i = 0; i < 1679616; i++) {
        visited[i] = (char*)malloc(9 * sizeof(char));
    }

    board_t** parent = (board_t**)malloc(1679616 * sizeof(board_t*));
    for (int i = 0; i < 1679616; i++) {
        parent[i] = (board_t*)malloc(9 * sizeof(board_t));
    }


    Queue* q = init_queue();
    int* o = ord(board);
    visited[o[0]][o[1]] = 1;
    enqueue(q, board);
    while (!is_empty_queue(q)) {
        board_t v = dequeue(q);
        if (is_won(v)) {
            board_t c = v;
            int* par = ord(c);
            int a = 0;
            while (!is_same(c, board)) {
                Node* n = (Node*)malloc(sizeof(Node));
                n->m = visited[par[0]][par[1]];
                n->next = moves->head;
                moves->head = n;
                c = parent[par[0]][par[1]];
                par = ord(c);
            }
                for (int i = 0; i < 1679616; i++) {
                    free(visited[i]);
                }
                free(visited);
                for (int i = 0; i < 1679616; i++) {
                    free(parent[i]);
                }
                free(parent);
           
            return moves;
        }   
        board_t a = up(v);
        board_t b = down(v);
        board_t c = left(v);
        board_t d = right(v);


        int* aord = ord(a);
        int* bord = ord(b);
        int* cord = ord(c);
        int* dord = ord(d);

        if (!visited[aord[0]][aord[1]] ) {
            visited[aord[0]][aord[1]] = 'U';
            parent[aord[0]][aord[1]] = v;
            enqueue(q, a);
        }
        if (!visited[bord[0]][bord[1]]) {
            visited[bord[0]][bord[1]] ='D';
            parent[bord[0]][bord[1]] = v;
            enqueue(q, b);
        }
        if (!visited[cord[0]][cord[1]]) {
            visited[cord[0]][cord[1]] = 'L';
            parent[cord[0]][cord[1]] = v;
            enqueue(q, c);
        }
        if (!visited[dord[0]][dord[1]]) {
            visited[dord[0]][dord[1]] = 'R';
            parent[dord[0]][dord[1]] = v;
            enqueue(q, d);
        }
    }
    for (int i = 0; i < 1679616; i++) {
    free(visited[i]);
    }
    free(visited);

    for (int i = 0; i < 1679616; i++) {
        free(parent[i]);
    }
    free(parent);
    return moves;
}

void init_board(board_t* board){
    // printf("check");
    for (int i = 0; i < 8; i++){
        // printf("check");
        cube_t c = (cube_t){
            {'c','b','b','b','b','g'}
        };
        board->filled[i] = c;
    }
    board->empty = 4;
}

board_t custom_init_board(int emp, int pos[8]){
    board_t board;
    board.empty = emp;
    for (int i = 0; i < 8; i++){
        cube_t cube;
        cube.faces[pos[i]] = 'c';
        int j_ = pos[i];
        int k;
        if (j_ == 0){
            k = 5;
            cube.faces[k] = 'g';
        }
        else if (j_ == 1){
            k = 3;
            cube.faces[k] = 'g';
        }
        else if (j_ == 2){
            k = 4;
            cube.faces[k] = 'g';
        }
        else if (j_ == 3){
            k = 1;
            cube.faces[k] = 'g';
        }
        else if (j_ == 4){
            k = 2;
            cube.faces[k] = 'g';
        }
        else if (j_ == 5){
            k = 2;
            cube.faces[k] = 'g';
        }
        for (int j = 0; j < 6; j++){
            if (j != k && j!=j_){
                cube.faces[j] = 'b';
            }
        }
        board.filled[i] = cube;
    }
    return board;
}

board_t random_init_board(){
    int empty = (rand())%9;
    int pos[8];
    for (int i = 0; i < 8; i++){
        pos[i] = (rand())%6;
    }
    board_t board = custom_init_board(empty, pos);
    return board;
}

void print_key(){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("%d",3*i+j);
        }
        printf("\n");
    }
    printf("\n");
}
// Up it Up

int main(){
    board_t board;
    int mode;
    printf("Choose the Input Mode : \n");
    printf("0 : Standard Initial Input\n");
    printf("1 : For giving your input\n");
    printf("2 : Random Input\n");
    scanf("%d",&mode);
    if (mode == 0){
        init_board(&board);
    }
    else if (mode == 1){
        int empty;
        print_key();
        printf("Enter the index of empty cell\n");
        scanf("%d",&empty);
        int pos[8];
        for (int i = 0; i < 8; i ++){
            printf("Enter the position of the c face on the cube %d",i);
            scanf("%d",&pos[i]);
        }
        board = custom_init_board(empty, pos);
    }
    else if (mode == 2){
        board = random_init_board();
    }
    printf("Initial Board :\n");
    print_board(board);
    Moves* moves = solve(board);
    if (moves->head == NULL){
        printf("Unsolvable.");
        return 0;
    }
    printf("Moves to solve : \n");
    Node* current = moves->head;
    while (current){
        printf("%c ",current->m);
        if (current-> m == 'U'){
            board = up(board);
        }
        else if(current-> m == 'D'){
            board = down(board);
        }
        else if (current->m == 'L'){
            board = left(board);
        }
        else{
            board = right(board);
        }
        current = current->next;
    }
    printf("\n\nSolved\n");
    print_board(board);
    return 0;
}