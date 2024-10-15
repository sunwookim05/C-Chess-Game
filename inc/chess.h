#include "main.h"

#ifdef _WIN32
#include <windows.h>
typedef int32_t ColorType;
#define BLACK    0
#define GRAY     8
#define RED     12
#define YELLOW  14
#define WHITE   15
#define RESET    7
#else
#include <unistd.h>
typedef const char* ColorType;
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"
#define RESET   "\033[0m"
#endif

#ifndef __CHESS_H
#define __CHESS_H

typedef enum {
    BLACKP,
    WHITEP
} Color;

typedef enum {
    MAIN,
    SINGLE_PLAYER,
    TWO_PLAYER
} GameMode;

typedef enum {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
} PieceType;

#pragma pack(push, 1)
typedef struct Position{
    uint8_t row;
    uint8_t col;
}Position;

#pragma pack(pop)

#pragma pack(push, 1)
typedef struct EnPassant {
    boolean isActive;
    Position position;
} EnPassant;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct piece {
    PieceType type;
    Color color;
    Position position; 
    boolean isCaptured;
    boolean hasMoved;
} ChessPiece;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Chess{
    GameMode mode;
    boolean isBlackTurn;
    EnPassant enPassant;
    ChessPiece *piece;
    string *table;
    void (*setColor)(ColorType);
    void (*chessMain)(struct Chess*);
    void (*baseTable)(struct Chess*);
    void (*makeTable)(struct Chess*);
    void (*updateTable)(struct Chess*);
}Chess;
#pragma pack(pop)

Chess new_Chess(void);

#endif