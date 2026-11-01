#include "ff9.h"

void game_newGame(void) {
    item_initNew();
    chr_newGameStub();
    party_initNewGame();
}
