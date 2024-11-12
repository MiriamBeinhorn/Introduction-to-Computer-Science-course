/**********
 Name: Miriam Beinhorn
 ID: 212340442
 Assignment: 4
**********/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PLAYERS 10
#define LONGEST_NAME 20
#define LONGEST_OBJECT 7
#define MAX_BOARD_DIM 30

typedef struct {
    char name[LONGEST_OBJECT];
    int isExposed;
} card;
typedef struct {
    char name[LONGEST_NAME];
    int score;
} player;


int namesCheck(char str[], int num);

void buildPlayersNum(player arr[MAX_PLAYERS], char *str, int num);

void instructions(int boardDim, char *argv[], int OBJECTS);

void printCard(char str[]);

void printBoard(int boardDim, card board[][MAX_BOARD_DIM]);

int makeMove(int boardDim, card board[][MAX_BOARD_DIM]);

void printScores(int playersNum, player arr[]);

void printWinner(player playersArr[], int playersNum);

int main(int argc, char *argv[]) {
    const int DIM = 1, PLAYERS_NUM = 2, NAMES_LIST = 3, OBJECTS = 4; //the order of the arguments
    const int MIN_ARG = 10; //minimal amount of arguments
    if (argc < MIN_ARG) { //the program must get at least 10 arguments
        printf("INVALID INPUT");
        return 1;
    }
    char *dimStr = argv[DIM]; //a pointer to the dimensions of the board
    int boardDim = atoi(dimStr); //gets the number of dimensions as an integer
    char *names = argv[PLAYERS_NUM]; //a pointer to the number of players
    int playersNum = atoi(names); //gets the number of players as an integer
    int pairsNum = (boardDim * boardDim / 2); //the amount of different pairs we got
    //valid input contains exactly this many arguments:
    const int ARG_NUM = ((boardDim * boardDim / 2) + (boardDim * boardDim) + 4);
    //INPUT CHECKS:
    if (boardDim % 2 != 0) { //the board dimensions must be an even number
        printf("INVALID INPUT");
        return 1;
    }
    if (argc != ARG_NUM) { //makes sure we got the exact amount of arguments we need
        printf("INVALID INPUT");
        return 1;
    }
    for (int i = 0; i < (boardDim * boardDim); i++) { //now we check if we got every coordinate we need exactly 1 time
        int found = 0;
        for (int j = OBJECTS + pairsNum;
             j < OBJECTS + pairsNum + (boardDim * boardDim); j++) { //check every single coordinate we got
            if ((atoi(argv[j])) == i) { //count how many times we got 'i'
                found++;
            }
        }
        if (found != 1) { //every number from 0 to (boardDim * boardDim - 1) must appear exactly 1 time
            printf("INVALID INPUT");
            return 1;
        }
    }
    if (namesCheck(argv[NAMES_LIST], playersNum)) { //makes sure the names string is valid
        printf("INVALID INPUT");
        return 1;
    }
    player playersArr[MAX_PLAYERS]; //build an array of the players
    buildPlayersNum(playersArr, argv[NAMES_LIST], playersNum); //enter values to the player's array
    card board[MAX_BOARD_DIM][MAX_BOARD_DIM]; //creating the game's board
    //entering the cards values into the board's array:
    int objectCount = 0, position;
    for (int i = OBJECTS + pairsNum; i < argc; i++) { //every 2 positions represent a single object
        position = atoi(argv[i]);
        strcpy(board[position / boardDim][position % boardDim].name, argv[OBJECTS + objectCount]);
        board[position / boardDim][position % boardDim].isExposed = 0;
        //exposed: as long as the card is not exposed this value will remain 0
        i++;
        position = atoi(argv[i]);
        strcpy(board[position / boardDim][position % boardDim].name, argv[OBJECTS + objectCount]);
        board[position / boardDim][position % boardDim].isExposed = 0;
        //exposed: as long as the card is not exposed this value will remain 0
        objectCount++;
    }
    instructions(boardDim, argv, OBJECTS); //printing the instructions
    //starting the game:
    int matches = 0, turn = 0;
    while (matches < pairsNum) { //play until all cards have been matched
        printBoard(boardDim, board); //printing current state of the board
        printf("\nIt is %s's turn.\n", playersArr[turn].name);
        if (makeMove(boardDim, board)) { //making a move. receives 1 if got a match, 0 if didn't
            playersArr[turn].score = (playersArr[turn].score) + 1; //if got a match, add 1 to the players score
            matches++; //add 1 to the pairs that were already matched
            turn--;//we are going to move to the next player,
            // so if this player got a mach we need a take a step back first
        }
        if (turn < (playersNum - 1))
            turn++; //move to the next player
        else //if it was the last player
            turn = 0; //return to the first player
        printScores(playersNum, playersArr);
    }
    printWinner(playersArr, playersNum); //end of the game. check and print who is the winner
    return 0;
}

int namesCheck(char str[], int num) { //gets the names string and the wanted players number
    int count = 0;
    int len = strlen(str);
    if (str[0] == '#' || str[len - 1] == '#') { //check if we got '#' as the first or the last char
        return 1;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] == '#') {
            if ((i < len - 1) && (str[i + 1] == '#')) {
                return 1; //we can't get 2 '#' in a row
            }
            count++; //count the '#'
        }
    }
    if (count == num - 1) //the number of '#' suppose to be 1 less than the number of names we get
        return 0;
    return 1;
}

//we use this function to build an array of the players
void buildPlayersNum(player arr[MAX_PLAYERS], char *str, int num) {
    char hashtag[] = "#"; //the players are seperated by '#'
    char *token = strtok(str, hashtag);
    int i = 0;
    while (token != NULL && i < num) {
        arr[i].score = 0; //all players start with 0 points
        strcpy(arr[i].name, token);
        token = strtok(NULL, hashtag);
        i++;
    }
}

void instructions(int boardDim, char *argv[], int OBJECTS) { //printing the instructions
    printf("Welcome to the Memory Card Game!\nThe game will be played on the following board:\n\n | ");
    for (int i = 0; i < boardDim; i++) //print the board
        printf("%d | ", i);
    printf("\n");
    for (int i = 0; i < (strlen("0 |") + strlen("(0,0)|") * boardDim); i++)
        printf("-");
    printf("\n");
    for (int line = 0; line < boardDim; line++) {
        printf("%d |", line);
        for (int column = 0; column < boardDim; column++) {
            printf("(%d,%d)|", line, column);
        }
        printf("\n");
        for (int i = 0; i < (strlen("0 |") + strlen("(0,0)|") * boardDim); i++)
            printf("-");
        printf("\n");
    }
    printf("\nAnd the following objects:\n\n"); //print a list of the objects
    for (int i = 0; i < (boardDim * boardDim / 2); i++)
        printf("%d. %s\n", i + 1, argv[OBJECTS + i]);
    printf("\nLet the game begin!\n\n");
}

void printCard(char str[]) {
    const int WIDTH = 15; //amount of chars we need to print for every card
    int strLength = strlen(str);
    int rightPadding = (WIDTH - strLength) / 2; //we want the right side to be rounded down
    int leftPadding = WIDTH - strLength - rightPadding;
    for (int i = 0; i < leftPadding; i++) {
        printf(" ");
    }
    printf("%s", str);
    for (int i = 0; i < rightPadding; i++) {
        printf(" ");
    }
}

void printBoard(int boardDim, card board[][MAX_BOARD_DIM]) {
    const int CHARS_IN_CARD = 15; //every card is supposed to be printed with 15 chars
    printf("Current board state:\n");
    for (int i = 0; i < (boardDim * CHARS_IN_CARD + boardDim + strlen("|")); i++)
        printf("-");
    printf("\n");
    for (int line = 0; line < boardDim; line++) {
        for (int column = 0; column < boardDim; column++) {
            printf("|");
            if (board[line][column].isExposed) //if the card is already exposed, we print its name
                printCard(board[line][column].name);
            else
                printCard("X"); //if the card is not exposed yet, we print 'X'
        }
        printf("|\n");
        for (int i = 0; i < (boardDim * CHARS_IN_CARD + boardDim + strlen("|")); i++)
            printf("-");
        printf("\n");
    }
}

int makeMove(int boardDim, card board[][MAX_BOARD_DIM]) {
    const int EXPOSE = 1; //to expose a card we change the value in its isExposed to 1
    int row1, col1, row2, col2;
    printf("Enter the coordinates of the first card (row col): ");
    scanf("%d %d", &row1, &col1);
    printf("Enter the coordinates of the second card (row col): ");
    scanf("%d %d", &row2, &col2);
    //input check:
    if ((row1 >= boardDim) || (col1 >= boardDim) || (row2 >= boardDim) || (col2 >= boardDim) ||
        (row1 < 0) || (col1 < 0) || (row2 < 0) || (col2 < 0)) { //the card must be in the board's range
        printf("Invalid move. Try again.\n");
        return makeMove(boardDim, board); //make a new move
    }
    if ((row1 == row2) && (col1 == col2)) { //must guss 2 different cards
        printf("Invalid move. Try again.\n");
        return makeMove(boardDim, board); //make a new move
    }
    if (board[row1][col1].isExposed || board[row2][col2].isExposed) { //the player can't choose an exposed card
        printf("Invalid move. Try again.\n");
        return makeMove(boardDim, board); //make a new move
    }
    if (!(strcmp(board[row1][col1].name, board[row2][col2].name))) { //strcmp returns 0 if the strings are identical
        printf("Match!\n\n");
        board[row1][col1].isExposed = EXPOSE; //expose the card
        board[row2][col2].isExposed = EXPOSE; //expose the card
        return 1; //return 1 if we got a match
    }
    printf("No match. Try again.\n\n");
    return 0; //return 0 if we didn't get a match
}

void printScores(int playersNum, player arr[]) { //after avery round, we want to see the scores board
    printf("The scores are:\n");
    for (int i = 0; i < playersNum - 1; i++) { //print evey player's score (except the last one)
        printf("%s: %d, ", arr[i].name, arr[i].score);
    }
    //now print the last player (because it's in a different form of printing)
    printf("%s: %d\n", arr[playersNum - 1].name, arr[playersNum - 1].score);
}

void printWinner(player playersArr[], int playersNum) {
    int bestScore = 0, count = 0, winner;
    for (int i = 0; i < playersNum; i++) {
        if (playersArr[i].score > bestScore) { //checks what is the highest score
            bestScore = playersArr[i].score;
            winner = i; //which player is it that got the highest score
            count = 1;
        } else if (playersArr[i].score == bestScore) //check if more than 1 player got the highest score
            count++;
    }
    if (count > 1)
        printf("It's a tie!");
    else
        printf("Congratulations %s! You won!", playersArr[winner].name);
}