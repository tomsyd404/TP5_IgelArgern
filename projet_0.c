#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MAX_HEDGEHOG = 302;
const int MAX_LINE = 6;
const int MAX_ROW = 9;
const int MAX_SLICE = 4;
const int MAX_PLAYERS = 4;

const int INVALID_X[6] = {0,1,2,3,4,5};
const int INVALID_Y[6] = {2,6,4,5,3,7};
const int CASE_VIDE = -1;
const int ERROR = -2;
const char IMPOSSIBLE = '!';

typedef struct{
    int size;
    char stackHedgehog[MAX_HEDGEHOG];
} cell_t;

typedef struct{
    cell_t board[MAX_LINE][MAX_ROW];
} board_t;

void display_message_error(){
    printf("Erreur : operation invalide");
}

bool verify_coords(int line, int row){
    return line >= 0 && line < MAX_LINE && row >= 0 && row < MAX_ROW;
}

void board_push(board_t* b, int line, int row, char team){
    if (!verify_coords(line,row)){
        display_message_error();
        return;
    }
    int crt_siz = b->board[line][row].size;
    b->board[line][row].stackHedgehog[crt_siz] = team;
    b->board[line][row].size++;
    if (crt_siz >= MAX_HEDGEHOG){
        display_message_error();
    }
}

char board_pop(board_t* b, int line, int row){
    if (!verify_coords(line,row)){
        display_message_error();
        return IMPOSSIBLE;
    }
    int crt_siz = b->board[line][row].size;
    if (crt_siz <= 0 || !b->board[line][row].stackHedgehog[crt_siz]){
        // TO VERIFY -> check if character is empty
        display_message_error();
        return IMPOSSIBLE;
    }
    b->board[line][row].size--;
    char team = b->board[line][row].stackHedgehog[crt_siz-1];
    b->board[line][row].stackHedgehog[crt_siz-1] = CASE_VIDE;
    return team;
}

int board_height(board_t* b, int line, int row){
    if (!verify_coords(line,row)){
        display_message_error();
        return ERROR;
    }
    int crt_siz = b->board[line][row].size;
    return crt_siz;
}

char board_top(board_t* b, int line, int row){
    if (!verify_coords(line,row)){
        display_message_error();
        return IMPOSSIBLE;
    }
    int crt_siz = b->board[line][row].size;
    if (crt_siz <= 0 || !b->board[line][row].stackHedgehog[crt_siz-1]){
        // TO VERIFY -> check if character is empty
        display_message_error();
        return IMPOSSIBLE;
    }
    char team = b->board[line][row].stackHedgehog[crt_siz-1];
    return team;
}

char board_peek(board_t* b, int line, int row, int pos){
    if (!verify_coords(line,row)){
        display_message_error();
        return IMPOSSIBLE;
    }
    int crt_siz = b->board[line][row].size;
    if (crt_siz <= 0 || crt_siz < pos+1 || !b->board[line][row].stackHedgehog[crt_siz-1-pos]){
        // TO VERIFY -> check if character is empty
        display_message_error();
        return IMPOSSIBLE;
    }
    char team = b->board[line][row].stackHedgehog[crt_siz-1-pos];
    return team;
}

bool is_trapped(int line, int row){
    for (int i=0; i<MAX_LINE; ++i){
        if (INVALID_X[i] == line && INVALID_Y[i] == row){
            return true;
        }
    }
    return false;
}

void cell_print(board_t* b, int line, int row, int slice){
    bool trap = is_trapped(line, row); // TODO : trap
    int height;
    switch (slice){
        case 0 :
            if (trap){
                printf(" vvv ");
            }
            else{
                printf(" --- ");
            }
            break;
        case 1 :
            height = board_height(b, line, row);
            if (height == 0){
				if (trap){
					printf(">   <");
				}
				else{
					printf("|   |");
				}
            }
            else if (height >= 1){
                char team = toupper(board_top(b, line, row));
                if (trap){
					printf(">%c%c%c<", team, team, team);
				}
				else{
					printf("|%c%c%c|", team, team, team);
				}
            }
            else{
                display_message_error();
                return;
            }
            break;
        case 2 :
            height = board_height(b, line, row);
            if (height == 0){
				if (trap){
					printf(">   <");
				}
				else{
					printf("|   |");
				}
            }
            else if (height == 1){
                char team = toupper(board_top(b, line, row));
                if (trap){
					printf(">%c%c%c<", team, team, team);
				}
				else{
					printf("|%c%c%c|", team, team, team);
				}
            }
            else if (height == 2){
                char team = board_peek(b, line, row, 1);
                if (trap){
					printf(">%c%c%c<", team, team, team);
				}
				else{
					printf("|%c%c%c|", team, team, team);
				}
            }
            else if (height == 3){
                char team1 = board_peek(b, line, row, 1);
                char team2 = board_peek(b, line, row, 2);
                if (trap){
					printf(">%c %c<", team1, team2);
				}
				else{
					printf("|%c %c|", team1, team2);
				}
            }
            else if (height >= MAX_SLICE){
                char team1 = board_peek(b, line, row, 1);
                char team2 = board_peek(b, line, row, 2);
                char team3 = board_peek(b, line, row, 3);
                if (trap){
					printf(">%c%c%c<", team1, team2, team3);
				}
				else{
					printf("|%c%c%c|", team1, team2, team3);
				}
            }
            else{
                display_message_error();
                return;
            }
            break;
        case 3 :
            height = board_height(b, line, row);
            if (height == 0 || height == 1){
				if (trap){
					printf(" ^^^ ");
				}
				else{
					printf(" --- ");
				}
            }
            else if (height >= 2){
				if (trap){
					printf(" ^%d^ ", height);
				}
				else{
					printf(" -%d- ", height);
				}
            }
            else{
                display_message_error();
                return;
            }
            break;
        default :
            display_message_error();
    }
}

void board_print(board_t* b, int highlighted_line){
    for (int line=0; line<MAX_LINE; ++line){
        for (int slice=0; slice<MAX_SLICE; ++slice){
            if (line == highlighted_line){
                printf("> ");
            }
            else{
                printf("  ");
            }
            for (int row=0; row<MAX_ROW; ++row){
                cell_print(b, line, row, slice);
                printf("  ");
            }
            printf("\n");
        }
    }
}

int de(){
	return rand() % 6 + 1;
}

board_t create_board(){
	board_t b = {0};
	return b;
}

int main(int argc,char **argv){
	srand(time(NULL));
    board_t b = create_board();
    board_print(&b, 1);
    return 0;
}





