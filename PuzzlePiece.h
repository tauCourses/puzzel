#ifndef PUZZLE_PUZZLE_PIECE_H
#define PUZZLE_PUZZLE_PIECE_H


//#include <cstdnt>
#include <cstdint>

#define Piece_t uint8_t
#define nullPiece (Piece_t) 0xFF //0b11111111

class PuzzlePiece {

public:
    static bool isPieceRespectConstrain(Piece_t p, Piece_t c);

    static bool isValidPiece(Piece_t piece);

    PuzzlePiece(int index, int left, int up, int right, int down);

    explicit PuzzlePiece(const PuzzlePiece *copy);

    PuzzlePiece(const PuzzlePiece &copy);

    const int index;
    const int left, up, right, down;

    Piece_t representor();
};


#endif //PUZZLE_PUZZLE_PIECE_H
