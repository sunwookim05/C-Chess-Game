#include "main.h"
#include "System.h"
#include "algorithm.h"
#include "Scanner.h"
#include "chess.h"

import SYSTEM System;

int main(void){ 
    Chess chess = new_Chess();

    chess.chessMain(&chess);
    
    // chess.baseTable(&chess);
    // chess.makeTable(&chess);
    
    return 0;
}