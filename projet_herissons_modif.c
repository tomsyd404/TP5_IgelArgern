#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ANSWER_LEN 300
// TODO : prevent buffer overflow

#define MAX_HEDGEHOGS 302
#define MAX_LINE 6 // MAX_LINE > 1
#define MAX_ROW 9 // MAX_ROW > 1
#define MAX_SLICE 4
#define MAX_PLAYERS 4
#define NB_HEDGEHOGS 4 // nb of hedgehogs per player >= 3
#define COND_VICTOIRE (NB_HEDGEHOGS-1)

const int INVALID_X[6] = {0,1,2,3,4,5};
const int INVALID_Y[6] = {2,6,4,5,3,7};
const int CASE_VIDE = -1;
const int ERROR = -2;
const char IMPOSSIBLE = '!';

typedef struct{
    int x,y;
} coord_t;

typedef struct{
    int playerID,score;
} playerScore_t;

typedef struct{
    int size;
    char stackHedgehog[MAX_HEDGEHOGS];
} cell_t;

typedef struct{
    cell_t board[MAX_LINE][MAX_ROW];
    int scorePlayers[MAX_PLAYERS];
} board_t;

void display_message_error(){
    printf("Erreur : operation invalide\n");
}

bool verify_coords(int line, int row){
    return line >= 0 && line < MAX_LINE && row >= 0 && row < MAX_ROW;
}

void board_push(board_t* b, int line, int row, char team){
    int crt_siz = b->board[line][row].size;
    if (!verify_coords(line,row) || crt_siz >= MAX_HEDGEHOGS){
        display_message_error();
        return;
    }
    b->board[line][row].stackHedgehog[crt_siz] = team;
    b->board[line][row].size++;
    if (row == MAX_ROW-1){
        b->scorePlayers[team-'a']++;
    }
}

char board_pop(board_t* b, int line, int row){
    if (!verify_coords(line,row)){
        display_message_error();
        return IMPOSSIBLE;
    }
    int crt_siz = b->board[line][row].size;
    if (crt_siz <= 0 || b->board[line][row].stackHedgehog[crt_siz-1] == CASE_VIDE){
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
    if (crt_siz <= 0 || b->board[line][row].stackHedgehog[crt_siz-1] == CASE_VIDE){
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
    if (crt_siz <= 0 || crt_siz < pos+1 || b->board[line][row].stackHedgehog[crt_siz-1-pos] == CASE_VIDE){
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

bool can_move(board_t* b, int line, int row){
	int nb_hedgehogs = 0;
	for (int previous_row = 0; previous_row<row; ++previous_row){
		nb_hedgehogs = nb_hedgehogs + board_height(b, line, previous_row);
	}
	if (nb_hedgehogs == 0){
		return true;
	}
	else{
		return false;
	}
}

int verticalMove(board_t* b, char player){
    int diceResult = dice();
    board_print(b,diceResult-1);
    printf("C'est au joueur %c de jouer.\n", toupper(player));
    if (diceResult == 1){
        printf("Tu dois jouer sur la 1ere ligne\n");
    }
    else{
        printf("Tu dois jouer sur la %dieme ligne\n",diceResult);
    }
    coord_t possibleCells[MAX_LINE];
    int nextCell = 0;
    for (int iLig=0; iLig<MAX_LINE; ++iLig){
        for (int iRow=0; iRow<MAX_ROW; ++iRow){
            if (board_height(b,iLig,iRow) <= 0)
                continue;
            char top = board_top(b,iLig,iRow);
            // Verify trapped
            if (top == player){
				if (!is_trapped(iLig,iRow)){
					possibleCells[nextCell] = (coord_t){iLig,iRow};
					++nextCell;
				}
				else{
					if (can_move(b, iLig, iRow)){
						possibleCells[nextCell] = (coord_t){iLig,iRow};
						++nextCell;
					}
				}
            }
        }
    }
    if (nextCell == 0){
        printf("Tu ne peux pas jouer ! :(\n");
        return diceResult;
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
        bool coherent = false;
        char chosenRow = getchar();
        // ignores \n from the previous buffer
        int chosenLine;
        while (!coherent){
            chosenRow = getchar();
            scanf("%d",&chosenLine);
            for (int iCell=0; iCell<nextCell; ++iCell){
                coord_t cell = possibleCells[iCell];
                if (cell.x == chosenLine-1 && cell.y == chosenRow-'a'){
                    coherent = true;
                    break;
                }
            }
            if (coherent)
                break;
            printf("Saisie invalide : Veuillez rentrer une case valide \n\n");
        }
        char team = board_pop(b,chosenLine-1,chosenRow-'a');
        // Testing edge cases : if on border then automatically move
        if (chosenLine == 1){
            board_push(b,1,chosenRow-'a',team);
        }
        else if (chosenLine == MAX_LINE){
            board_push(b,MAX_LINE-2,chosenRow-'a',team);
        }
        else{
            printf("Voulez-vous deplacer vers le haut ou vers le bas : [Haut/Bas]\n\n");
            char buffer[MAX_ANSWER_LEN];
            while (scanf("%s",buffer)){
                if (strcmp(buffer,"Haut") == 0){
                    board_push(b,chosenLine-2,chosenRow-'a',team);
                    break;
                }
                if (strcmp(buffer,"Bas") == 0){
                    board_push(b,chosenLine,chosenRow-'a',team);
                    break;
                }
                printf("Saisie invalide : Veuillez rentrer Haut ou Bas \n\n");
            }
            getchar(); // <- just to ignore the \n
        }
    }
    else getchar(); // <- just to ignore an extra \n
    board_print(b, diceResult-1);
    return diceResult;
}

void playerTurn(board_t* b, char player){
    int tarLine = verticalMove(b,player); // dice result is 1-indexing
    int possibleRows[MAX_LINE];
    int nextCell = 0;
    for (int iRow=0; iRow<MAX_ROW-1; ++iRow){
        if (board_height(b,tarLine-1,iRow) <= 0)
            continue;
		if (!is_trapped(tarLine-1,iRow)){
			possibleRows[nextCell] = iRow;
			++nextCell;
		}
		else if (can_move(b, tarLine-1, iRow)){
			possibleRows[nextCell] = iRow;
			++nextCell;
        }
    }
    if (nextCell == 0){
        printf("Tu ne peux pas jouer ! :(\n");
        return;
    }
    printf("Tu dois avancer un herisson\n\n");
    printf("Choisis parmi : \n");
    for (int iCell=0; iCell<nextCell; ++iCell){
        printf("%c",(char)('a'+possibleRows[iCell]));
        if (iCell != nextCell-1)
            printf(", ");
        else
            printf(".\n\n");
    }
    bool coherent = false;
    char chosenRow;// = getchar();
//    printf("-%c-",chosenRow);
    while (!coherent){
        scanf("%c",&chosenRow); // attention si utilisateur fait nimp
        for (int iCell=0; iCell<nextCell; ++iCell){
            int iRow = possibleRows[iCell];
            if (iRow == chosenRow-'a'){
                coherent = true;
                break;
            }
        }
        if (coherent)
            break;
        printf("Saisie invalide : Veuillez rentrer une case valide \n\n");
    }
    char team = board_pop(b,tarLine-1,chosenRow-'a');
    board_push(b,tarLine-1,chosenRow-'a'+1,team);
    //board_print(b,tarLine-1);
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

int compTriage(const void* player1, const void* player2){
    playerScore_t* p1 = (playerScore_t*) player1;
    playerScore_t* p2 = (playerScore_t*) player2;
    return p2->score - p1->score;
}

bool victoire(board_t* b){
    bool is_finished = false;
    for (int iPlayer=0; iPlayer<MAX_PLAYERS; ++iPlayer){
        if (b->scorePlayers[iPlayer] >= COND_VICTOIRE){
            is_finished = true;
            break;
        }
    }
    if (!is_finished)
        return false;
    // ranking
    playerScore_t classement[MAX_PLAYERS];
    for (int iPlayer=0; iPlayer<MAX_PLAYERS; ++iPlayer){
        classement[iPlayer] = (playerScore_t){iPlayer,b->scorePlayers[iPlayer]};
    }
    qsort(classement,MAX_PLAYERS,sizeof(playerScore_t),compTriage);
    int rank = 1;
    bool printing = true;
    for (int iPlayer=0; iPlayer<MAX_PLAYERS; ++iPlayer){
        if (iPlayer != 0 && classement[iPlayer].score < classement[iPlayer-1].score){
            ++rank;
            printing = true;
        }
        if (printing){
            printf("\n- Place #%d : équipe %c",rank,'A'+classement[iPlayer].playerID);
        }
        else{
            printf(", équipe %c",'A'+classement[iPlayer].playerID);
        }
        printing = false;
    }
    printf("\n\n");
    return true;
}

int main(){
	srand(time(NULL));
    board_t b = create_board();
    //board_print(&b, 0);
    while (1){
        for (int iPlayer=0; iPlayer<MAX_PLAYERS; ++iPlayer){
            playerTurn(&b, 'a'+iPlayer);
        }
        if (victoire(&b)){
            board_print(&b,-1);
            printf("THE END\n\n");
            break;
        }
    }
    return 0;
}
