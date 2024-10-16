#include "chess.h"
#include "System.h"
#include "Scanner.h"

import SYSTEM System;

void setColor(ColorType color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    System.out.printf("%s", color);
#endif
}

void resetConsole(){
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    printf("\033[H\033[J");
    #else
    printf("\033[H\033[J");
    #endif
}

void clearConsole(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifndef _WIN32
int kbhit() {
    struct termios oldt, newt;
    int oldf;
    char ch;
    int byteswaiting;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    byteswaiting = read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (byteswaiting == 1) {
        return ch;
    }
    return 0; // No input
}
#endif

void colorPrintln(boolean color, const string str){
    setColor(color ? YELLOW : RESET);
    System.out.println(str);
    setColor(RESET);
}

// boolean canMove(Chess *chess, ChessPiece piece, Position newPos){

// }

void uiMain(uint8_t i){
    clearConsole();
    System.out.println("\t\t\tChess");
    System.out.println("");
    setColor(GRAY);
    System.out.println("\t\tSingle Play (to be continue)");
    colorPrintln(i == 0,"\t\tMulty Play");
    colorPrintln(i == 1,"\t\tHow to Play");
    colorPrintln(i == 2,"\t\tEXIT");
    System.out.println("\n");
    setColor(GRAY);
    System.out.println("Select \'W\'&\'S\'\nExecution \'Enter\'");
    setColor(RESET);
}

void howPlay(Chess *chess){
    clearConsole();
    setColor(GRAY);
    System.out.printf("=== Chess Move Command Format ===\n");
    System.out.printf("1. Basic move command: The format is [starting position][destination position] for moving pieces.\n");
    System.out.printf("   Example: e2e4 -> Moves the pawn from e2 to e4\n");
    System.out.printf("   Format: [starting position][destination position] (expressed as letter+number)\n");
    System.out.printf("\n=== Castling Explanation ===\n");
    System.out.printf("1. Castling is only possible when the king and rook have not moved from their initial positions.\n");
    System.out.printf("2. Kingside castling (short castling): O-O format where the king moves two squares, and the rook jumps over the king.\n");
    System.out.printf("   Example: O-O\n");
    System.out.printf("3. Queenside castling (long castling): O-O-O format where the king moves two squares to the left, and the rook jumps over the king.\n");
    System.out.printf("   Example: O-O-O\n");
    System.out.printf("\n=== Promotion Explanation ===\n");
    System.out.printf("1. Promotion occurs when a pawn reaches the opponent's back rank (8th rank).\n");
    System.out.printf("2. The pawn can be promoted to a queen, rook, bishop, or knight.\n");
    System.out.printf("   Format: [starting position][destination position]=[promoted piece]\n");
    System.out.printf("   Example: e7e8=Q -> The pawn moves from e7 to e8 and promotes to a queen\n");
    System.out.printf("\n=== En Passant Explanation ===\n");
    System.out.printf("1. En passant is possible only immediately after an opponent's pawn moves two squares forward from its starting position.\n");
    System.out.printf("2. When an opponent's pawn is adjacent, you can capture it as if it had only moved one square forward.\n");
    System.out.printf("   Format: [captured pawn position][destination position].e\n");
    System.out.printf("   Example: e4d5.e -> The pawn on e4 moves to d5, capturing the opponent's pawn en passant\n\n");
    System.out.printf("Press any key to return...");
    getchar();
    setColor(RESET);
    clearConsole();
    chess->mode = MAIN;
    return;
}

void chessMain(Chess *chess){
    uint8_t i = 0;
    char input;
    uiMain(i);
    while(true){
        if(kbhit()){
            #ifdef _WIN32
            input = getch();
            #else
            input = kbhit();
            #endif
            if(input == '\r' || input == '\n'){
                clearConsole();
                if(!i) chess->mode = TWO_PLAYER;
                if(i == 1) chess->mode = HOW_PLAY;
                if(i == 2) break;
            }else if(!((input | 0x20) ^ 'w')){
                if(!(chess->mode) & (i > 0)) i--;
            }else if(!((input | 0x20) ^ 's')){
                if(!(chess->mode) & (i < 2)) i++;
            }else if(!((input | 0x20) ^ 'a')){

            }else if(!((input | 0x20) ^ 'd')){

            }
            switch (chess->mode){
            case MAIN: uiMain(i); break;
            case SINGLE_PLAYER: break;
            case HOW_PLAY: howPlay(chess); break;
            default: uiMain(i);break;
            }
            uiMain(i); 
        }
    }
}

void baseTable(Chess *chess) {
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            if (i == 0 || i == 9) *(*(chess->table + i) + j) = (j == 0) ? ' ' : (j == 9) ? ' ' : 'A' + (j - 1);
            else if (j == 0 || j == 9) *(*(chess->table + i) + j) = (i == 9) ? ' ' : '0' + i;
            else if ((i == 9 && j == 0) || (i == 9 && j == 9)) *(*(chess->table + i) + j) = ' '; 
            else {
                for (size_t k = 0; k < 32; k++) {
                    if ((*(chess->piece + k)).position.row == i && (*(chess->piece + k)).position.col == j) {
                        switch ((*(chess->piece + k)).type) {
                            case QUEEN: *(*(chess->table + i) + j) = 'Q'; break;
                            case KING: *(*(chess->table + i) + j) = 'K'; break;
                            case ROOK: *(*(chess->table + i) + j) = 'R'; break;
                            case PAWN: *(*(chess->table + i) + j) = 'P'; break;
                            case BISHOP: *(*(chess->table + i) + j) = 'B'; break;
                            case KNIGHT: *(*(chess->table + i) + j) = 'N'; break;
                            default: break;
                        }
                        break;
                    } else {
                        *(*(chess->table + i) + j) = '#';
                    }
                }
                if (*(*(chess->table + i) + j) == 'I') {
                    *(*(chess->table + i) + j) = ' ';
                }
            }
        }
    }
}

void makeTable(Chess *chess){
    resetConsole();
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            char piece = *(*(chess->table + i) + j);
            if (piece == ' ') setColor(BLACK);
            else if (piece == '#') setColor((i + j) % 2 == 0 ? GRAY : WHITE);
            else if ((i == 0 || i == 9) && piece >= 'A' && piece <= 'Z') setColor(YELLOW);
            else if (piece >= '0' && piece <= '9') setColor(RED);
            else {
                for (size_t k = 0; k < 32; k++) {
                    if ((*(chess->piece + k)).position.row == i && (*(chess->piece + k)).position.col == j) {
                        if ((*(chess->piece + k)).color == BLACKP) setColor(GRAY);
                        else if ((*(chess->piece + k)).color == WHITEP) setColor(WHITE);
                        break;
                    }
                }
            }
            System.out.printf(j == 0 || j == 8 ? "%c  " : "%c " , piece);
            setColor(RESET);
        }
        System.out.println("");
    }
    System.out.println("");
    setColor(chess->isBlackTurn ? GRAY : WHITE);
    System.out.printf(chess->isBlackTurn ? "Black turn" : "White turn");
}

void updateTable(Chess *chess){

}

Chess new_Chess(){
    Chess chess;
    chess.mode = MAIN;         
    chess.isBlackTurn = false;      
    chess.enPassant.isActive = false;
    chess.piece = (ChessPiece*)malloc(sizeof(ChessPiece) * 32);
    chess.table = (string*)malloc(sizeof(string) * 10);
    for(size_t i = 0; i < 9; i++) *(chess.table + i) = (string)malloc(sizeof(char) * 10);
    chess.baseTable = baseTable;
    chess.chessMain = chessMain;
    chess.makeTable = makeTable;
    chess.updateTable = updateTable;

    *(chess.piece + 0) = (ChessPiece){ROOK, BLACKP, {1, 1}, false, false};
    *(chess.piece + 1) = (ChessPiece){KNIGHT, BLACKP, {1, 2}, false, false};
    *(chess.piece + 2) = (ChessPiece){BISHOP, BLACKP, {1, 3}, false, false};
    *(chess.piece + 3) = (ChessPiece){QUEEN, BLACKP, {1, 4}, false, false};
    *(chess.piece + 4) = (ChessPiece){KING, BLACKP, {1, 5}, false, false};
    *(chess.piece + 5) = (ChessPiece){BISHOP, BLACKP, {1, 6}, false, false};
    *(chess.piece + 6) = (ChessPiece){KNIGHT, BLACKP, {1, 7}, false, false};
    *(chess.piece + 7) = (ChessPiece){ROOK, BLACKP, {1, 8}, false, false};
    for (size_t i = 0; i < 8; i++) *(chess.piece + 8 + i) = (ChessPiece){PAWN, BLACKP, {2, i + 1}, false, false};
    *(chess.piece + 16) = (ChessPiece){ROOK, WHITEP, {8, 1}, false, false};
    *(chess.piece + 17) = (ChessPiece){KNIGHT, WHITEP, {8, 2}, false, false};
    *(chess.piece + 18) = (ChessPiece){BISHOP, WHITEP, {8, 3}, false, false};
    *(chess.piece + 19) = (ChessPiece){QUEEN, WHITEP, {8, 4}, false, false};
    *(chess.piece + 20) = (ChessPiece){KING, WHITEP, {8, 5}, false, false};
    *(chess.piece + 21) = (ChessPiece){BISHOP, WHITEP, {8, 6}, false, false};
    *(chess.piece + 22) = (ChessPiece){KNIGHT, WHITEP, {8, 7}, false, false};
    *(chess.piece + 23) = (ChessPiece){ROOK, WHITEP, {8, 8}, false, false};
    for (size_t i = 0; i < 8; i++) *(chess.piece + 24 + i) = (ChessPiece){PAWN, WHITEP, {7, i + 1}, false, false};

    return chess;
}