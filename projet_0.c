#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ANSWER_LEN 300
// TODO : prevent buffer overflow

#define MAX_HEDGEHOGS 302
#define MAX_LINE 6
#define MAX_ROW 9
#define MAX_SLICE 4
#define MAX_PLAYERS 4
#define NB_HEDGEHOGS 4 // nb of hedgehogs per player

const int INVALID_X[6] = {0,1,2,3,4,5};
const int INVALID_Y[6] = {2,6,4,5,3,7};
const int CASE_VIDE = -1;
const int ERROR = -2;
const char IMPOSSIBLE = '!';

typedef struct{
    int x,y;
} coord_t;

typedef struct{
    int size;
    char stackHedgehog[MAX_HEDGEHOGS];
} cell_t;

typedef struct{
    cell_t board[MAX_LINE][MAX_ROW];
} board_t;

void display_message_error(){
    printf("Erreur : operation invalide\n");
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
    if (crt_siz >= MAX_HEDGEHOGS){
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

int dice(){
	return rand() % 6 + 1;
}

void swap(char* hedgehogs, int a, int b){
    char c = hedgehogs[a];
    hedgehogs[a] = hedgehogs[b];
    hedgehogs[b] = c;
}

void random_shuffle(char* hedgehogs, int nbHedgehogs){
    for (int i=nbHedgehogs-1; i>=0; --i){
        int idx = rand() % (i+1);
        swap(hedgehogs,i,idx);
    }
}

void playerTurn(board_t* b, char player){
    printf("C'est au joueur %c de jouer.\n", toupper(player));
    int ligne = dice();
    if (ligne == 1){
        printf("Tu dois jouer sur la 1ere ligne\n");
    }
    else{
        printf("Tu dois jouer sur la %dieme ligne\n",ligne);
    }
    coord_t possibleCells[MAX_LINE];
    int nextCell = 0;
    for (int iLig=0; iLig<MAX_LINE; ++iLig){
        for (int iRow=0; iRow<MAX_ROW; ++iRow){
            if (board_height(b,iLig,iRow) <= 0)
                continue;
            char top = board_top(b,iLig,iRow);
            if (top == player){
                possibleCells[nextCell] = (coord_t){iLig,iRow};
                ++nextCell;
                break;
            }
        }
    }
    if (nextCell == 0){
        printf("Tu ne peux pas jouer ! :(\n");
        return;
    }
    printf("Veux-tu deplacer un de tes herissons d'une ligne ? [Oui/Non]\n");
    char answer[MAX_ANSWER_LEN];
    bool skipNextStep = false;
    while (scanf("%s",answer)){ // scanf_s ?
        if (strcmp(answer,"Non") == 0){
            skipNextStep = true;
            break;
        }
        if (strcmp(answer,"Oui") == 0){
            skipNextStep = false;
            break;
        }
        printf("Saisie invalide : Veuillez rentrer Oui ou Non \n\n");
    }
    if (!skipNextStep){
        printf("Choisissez une case parmi :\n\n");
        for (int iCell=0; iCell<nextCell; ++iCell){
            coord_t cell = possibleCells[iCell];
            printf("%c%d",'a'+cell.y,cell.x+1);
            if (iCell != nextCell-1)
                printf(", ");
            else
                printf(".\n\n");
        }
    }
}

board_t create_board(){
	board_t b = {0};
	char hedgehogs[MAX_HEDGEHOGS];
	for (int i=0; i<MAX_PLAYERS; ++i){
        for (int j=0; j<NB_HEDGEHOGS; ++j){
            int idx = i*NB_HEDGEHOGS+j;
            hedgehogs[idx] = 'a'+i;
        }
	}
	int total_hedgehogs = MAX_PLAYERS*NB_HEDGEHOGS;
	random_shuffle(hedgehogs,total_hedgehogs);
    for (int i=0; i<total_hedgehogs; ++i){
        int rng = dice();
        board_push(&b,rng-1,0,hedgehogs[i]);
    }
	return b;
}

int main(int argc,char **argv){
	srand(time(NULL));
    board_t b = create_board();
    board_print(&b, 1);
    playerTurn(&b, 'a');
    return 0;
}
