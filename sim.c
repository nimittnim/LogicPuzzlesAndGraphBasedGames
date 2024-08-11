#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "test.h"

enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

typedef char board_t[30][20];
typedef char player_t;
typedef char edges_t[6][6]; 
typedef struct {
    int vertex_1;
    int vertex_2;
    int score;
}move_t;


#define MAX_ORD (14348907)

uint16_t computed_moves[MAX_ORD+1];

uint16_t encode_move(move_t m){
    uint16_t b = 0;

    assert(0 <= m.vertex_1 && m.vertex_2 <= 5);
    b |= m.vertex_1;

    assert(0 <= m.vertex_2 && m.vertex_2 <= 5);
    b |= m.vertex_2 << 3;

    switch (m.score) {
    case -1: b |= 1 << 8; break;
    case 0: b |= 1 << 7; break;
    case 1: b |= 1 << 6; break;
    }

    return b;
}

move_t decode_move(uint16_t encoded){
    move_t m;
    m.vertex_1 = encoded & 0x7;
    m.vertex_2 = (encoded >> 3) & 0x7;
    m.score = (encoded >> 6) & 0x7;
    if (m.score == 0b001)
        m.score = -1;
    else if (m.score == 0b010)
        m.score = 0;
    else if (m.score == 0b100)
        m.score = 1;
    return m;
}


int ord(edges_t edges){
    int p = 1;
    int i = 0;
    int d;

    for (int vertex_1 = 0; vertex_1 < 6; vertex_1++) {
        for (int vertex_2 = vertex_1 + 1; vertex_2 < 6; vertex_2++) {
            switch (edges[vertex_1][vertex_2]) {
            case 'R': d = 1; break;
            case 'B': d = 2; break;
            case '.': d = 0; break;
            }
            i += d * p;
            p *= 3;
        }
    }
    return i;
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


int valid_move(edges_t edges, int vertex_1, int vertex_2){
    if (vertex_1 > 5 || vertex_2 > 5){
        return 0;
    }
    if (vertex_1 == vertex_2){
        return 0;
    }
    if (vertex_1 > vertex_2){
        int temp = vertex_1;
        vertex_1 = vertex_2;
        vertex_2 = temp;
    }
    if (edges[vertex_1][vertex_2] != '.' || edges[vertex_2][vertex_1] != '.'){
        return 0;
    }
    return 1;
}

int has_lost(edges_t edges, player_t player){

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            if (edges[i][j] == player){
                    for (int j_1 = 0; j_1 < 6; j_1++){
                        if (j_1 == i){
                            continue;
                        }
                        if (edges[j][j_1] == player){
                            if(edges[i][j_1] == player){
                                return 1;
                            }
                        }
                    }
            }
        } 
    }
    return 0;
}

int is_full(edges_t edges){
    for (int i = 0; i < 6; i++){
        for (int j = i + 1; j < 6; j++){
            if (edges[i][j] == '.'){
                return 0;
            }
        }
    }
    return 1;
}

void print_edges(edges_t edges){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            printf("%c ",edges[i][j]);
        }
        printf("\n");
    }
}

void make_move(edges_t edges, int vertex_1, int vertex_2, player_t player){
    if (vertex_1 > vertex_2){
        int temp = vertex_1;
        vertex_1 = vertex_2;
        vertex_2 = temp;
    }
    edges[vertex_1][vertex_2] = player;
    edges[vertex_2][vertex_1] = player;
}

void undo_move(edges_t edges, int vertex_1, int vertex_2, player_t player){
    if (vertex_1 > vertex_2){
        int temp = vertex_1;
        vertex_1 = vertex_2;
        vertex_2 = temp;
    }
    edges[vertex_1][vertex_2] = '.';
    edges[vertex_2][vertex_1] = '.';
}


move_t best_move(edges_t edges, player_t player){
    move_t candidate;
    move_t response;
    int no_candidate = 1;


    int o = ord(edges);
    if (computed_moves[o]) {
        return decode_move(computed_moves[o]);
    }
    
    printf("\n");


    // Finding best move based on other player's response
    for (int vertex_1 = 0; vertex_1 < 6; vertex_1++){
        for (int vertex_2 = 0; vertex_2 < 6; vertex_2++){
            if (valid_move(edges, vertex_1, vertex_2)){
                make_move(edges, vertex_1, vertex_2, player);
                // printf("\ncheck\n");
                
                if (has_lost(edges, player)){
                    undo_move(edges, vertex_1, vertex_2, player);
                    continue;
                }
                // undo_move(edges, vertex_1, vertex_2, player);

                if (is_full(edges)) {
                        undo_move(edges, vertex_1, vertex_2, player);
                        int scor;
                        if (has_lost(edges, player)){
                            scor = -1;
                        }
                        else{
                            scor = 1;
                        }
                        computed_moves[o] = encode_move(candidate = (move_t) {
                        .vertex_1 = vertex_1,
                        .vertex_2 = vertex_2,
                        .score = scor
                        });
                        return candidate;
                }
                response = best_move(edges, other_player(player));
                undo_move(edges, vertex_1, vertex_2, player);
                if (response.score == -1){
                    computed_moves[o] = encode_move(candidate = (move_t) {
                        .vertex_1 = vertex_1,
                        .vertex_2 = vertex_2,
                        .score = 1
                    });
                    return candidate;
                }
                else{ // response.score == 1
                    if (no_candidate){
                        computed_moves[o] = encode_move(candidate = (move_t) {
                        .vertex_1 = vertex_1,
                        .vertex_2 = vertex_2,
                        .score = -1
                        });
                        no_candidate = 0;
                    }
                }
            }
        }
    }
    computed_moves[o] = encode_move(candidate);
    return candidate;
}

void make_diagonal_left_to_bottom_1(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    int i = 0;
    while (start_i < end_i){
        board[start_i][start_j] = player;
        start_i = start_i + 2;
        start_j++;
        i++;
    }
    printf("\n%d\n",i);
}

void make_diagonal_left_to_top_1(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_i < end_i && start_j > end_j){
        make_diagonal_left_to_top_1(board, end_i, end_j, start_i, start_j,player);
        return;
    }
    else if(start_i < end_i && start_j < end_j){
        make_diagonal_left_to_bottom_1(board, start_i, start_j, end_i, end_j,player);
        return;
    }
    else if ((start_i > end_i && start_j > end_j)){
        make_diagonal_left_to_bottom_1(board, end_i, end_j, start_i, start_j,player);
        return;
    }
    // start_i > end_i && start_j < end_j
    else{
    while (start_i >= end_i){
        board[start_i][start_j] = player;
        start_i = start_i - 2;
        start_j++;
    }
    }
}

void make_diagonal_left_to_bottom_2(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    while (start_i <= end_i){
        board[start_i][start_j] = player;
        start_i++;
        start_j = start_j+2;
    }
}

void make_diagonal_left_to_top_2(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_i < end_i && start_j > end_j){
        make_diagonal_left_to_top_2(board, end_i, end_j, start_i, start_j,player);
    }
    else if(start_i < end_i && start_j < end_j){
        make_diagonal_left_to_bottom_2(board, start_i, start_j, end_i, end_j,player);
    }
    else if ((start_i > end_i && start_j > end_j)){
        make_diagonal_left_to_bottom_2(board, end_i, end_j, start_i, start_j,player);
    }
    // start_i > end_i && start_j < end_j
    else{
    while (start_i > end_i){
        board[start_i][start_j] = player;
        start_i--;
        start_j = start_j+2;
    }
    }
}

void make_diagonal_left_to_bottom(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    while (start_i <= end_i){
        board[start_i][start_j] = player;
        start_i++;
        start_j = start_j+1;
    }
}

void make_diagonal_left_to_top(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_i < end_i && start_j > end_j){
        make_diagonal_left_to_top(board, end_i, end_j, start_i, start_j,player);
    }
    else if(start_i < end_i && start_j < end_j){
        make_diagonal_left_to_bottom(board, start_i, start_j, end_i, end_j,player);
    }
    else if ((start_i > end_i && start_j > end_j)){
        make_diagonal_left_to_bottom(board, end_i, end_j, start_i, start_j,player);
    }
    // start_i > end_i && start_j < end_j
    else{
    while (start_i > end_i){
        board[start_i][start_j] = player;
        start_i--;
        start_j = start_j+1;
    }
    }
}

void make_vertical_line(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_i > end_i){
        make_vertical_line(board, end_i, end_j, start_i, start_j,player);
    }
    else{
    while (start_i <= end_i){
        board[start_i][start_j] = player;
        start_i++;
    }
    }
}

void make_horizontal_line(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_j > end_j){
        make_horizontal_line(board, end_i, end_j, start_i, start_j,player);
    }
    else{
    while (start_j <= end_j){
        board[start_i][start_j] = player;
        start_j++;
    }
    }
}

void make_line(board_t board, int start_i, int start_j, int end_i, int end_j,player_t player){
    if (start_i == end_i){
        make_horizontal_line(board, start_i, start_j, end_i, end_j,player);
    }
    else if (start_j == end_j){
        make_vertical_line(board, start_i, start_j, end_i, end_j,player);
    }
}



void update_board(board_t board, int vertex_1, int vertex_2, player_t player){
    if (vertex_1 > vertex_2){
        int temp = vertex_1;
        vertex_1 = vertex_2;
        vertex_2 = temp;
    }
    if (vertex_1 == 0 && vertex_2 == 1){make_diagonal_left_to_top(board,10,19,0,10,player);}
    else if (vertex_1 == 0 && vertex_2 == 2){make_diagonal_left_to_top_1(board,20,29,0,10,player);}
    else if (vertex_1 == 0 && vertex_2 == 3){make_line(board,0,10,29,10,player);}
    else if (vertex_1 == 0 && vertex_2 == 4){make_diagonal_left_to_top_1(board,20,0,0,10,player);}
    else if (vertex_1 == 0 && vertex_2 == 5){make_diagonal_left_to_top(board,10,0,0,10,player);}
    else if (vertex_1 == 1 && vertex_2 == 2){make_line(board,10,19,20,19,player);}
    else if (vertex_1 == 1 && vertex_2 == 3){ make_diagonal_left_to_top_1(board,10,29,29,10,player);}
    else if (vertex_1 == 1 && vertex_2 == 4){make_diagonal_left_to_top_2(board,20,0,10,29,player);}
    else if (vertex_1 == 1 && vertex_2 == 5){make_line(board,10,0,10,19,player);}
    else if (vertex_1 == 2 && vertex_2 == 3){ make_diagonal_left_to_top(board,29,10,20,19,player);}
    else if (vertex_1 == 2 && vertex_2 == 4){make_line(board,20,0,20,19,player);}
    else if (vertex_1 == 2 && vertex_2 == 5){make_diagonal_left_to_top_2(board,10,0,20,29,player);}
    else if (vertex_1 == 3 && vertex_2 == 4){make_diagonal_left_to_top(board,20,0,29,10,player);}
    else if (vertex_1 == 3 && vertex_2 == 5){make_diagonal_left_to_top_1(board,11,0,29,10,player);}
    else if (vertex_1 == 4 && vertex_2 == 5){make_line(board,10,0,19,0,player);}
}


void init_board(board_t board){
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 20; j++){
            board[i][j] = ' ';
        }
    }
    player_t player = '.';
    make_diagonal_left_to_top(board,10,19,0,10,player);
    make_line(board,10,19,20,19,player);
    make_diagonal_left_to_top(board,29,10,20,19,player);
    make_diagonal_left_to_top(board,20,0,29,10,player);
    make_line(board,10,0,19,0,player);
    make_diagonal_left_to_top(board,10,0,0,10,player);
}

void print_board(board_t board){
    printf("--------------------------------------------");
    printf("\n");
    for (int i = 0; i < 30; ++i) {
        for (int j = 0; j < 20; j++){
            printf("%2c",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("--------------------------------------------");
}

void init_edges(edges_t edges){
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            edges[i][j] = '.';
        }
    }
}

void print_key(){
    printf("\n 0,1,2,3,4 and 5 represent vertices of hexagon clockwise\n");
}


void user_vs_user(){
    int vertex_1,vertex_2;
    player_t current = 'B';
    edges_t edges;
    init_edges(edges);
    board_t board;
    init_board(board);
    while(1){
        print_key();
        printf("Enter your move : \n vertex 1 :\n");
        scanf("%d",&vertex_1);
        printf("vertex 2 :\n");
        scanf("%d",&vertex_2);
        printf("%d %d",vertex_1,vertex_2);
        if (valid_move(edges, vertex_1, vertex_2)){
            make_move(edges, vertex_1,vertex_2,current);
            print_edges(edges);
            update_board(board, vertex_1, vertex_2, current);
        if (has_lost(edges, other_player(current))){
            printf("\n %c won\n",current);
            print_edges(edges);
            break;
        }
        }    
        current = other_player(current);  
    }
}


void user_vs_computer(){
    int vertex_1,vertex_2;
    player_t current = 'B';
    board_t board;
    init_board(board);
    edges_t edges;
    init_edges(edges);
    move_t response;
    player_t player;
    printf("Choose player : \n R : play second \n B : play first\n");
    scanf("%s",&player);
   
    while(1){
        if (current == player){
            while (1){
            print_board(board);
            // print_edges(edges);
            print_key();
            printf("Enter your move : \n vertex 1 :\n");
            scanf("%d",&vertex_1);
            printf("vertex 2 :\n");
            scanf("%d",&vertex_2);
            if (valid_move(edges, vertex_1, vertex_2)){
                make_move(edges, vertex_1,vertex_2,current);
                update_board(board, vertex_1, vertex_2, current);
                break;
            }    
            else{
                printf("%s","\nIncorrect Move, please renter correct move \n");
            }
            }
        }
        else {
            response = best_move(edges,current);
            if (valid_move(edges, response.vertex_1, response.vertex_2)){
            make_move(edges, response.vertex_1, response.vertex_2, current);
            update_board(board, response.vertex_1, response.vertex_2,current);
            }
        }
        if (has_lost(edges, other_player(current))){
            print_board(board);
            printf("\nPlayer %c won",current);
            break;
        }
        if (is_full(edges)){
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
    scanf("%d",&mode);
    if (mode == 0){
        user_vs_computer();
    }
    else if (mode == 1){
        user_vs_user();
    }
    return 0;
}
