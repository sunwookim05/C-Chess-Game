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

void chessMain(Chess *chess){

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
    chess.setColor = setColor;
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