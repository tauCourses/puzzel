//
// Created by private on 12/12/17.
//

#include <algorithm>
#include "RotatablePieceManager.h"

vector<AbstractPieceManager::Shape> RotatablePieceManager::getAllPossiblePuzzleShapes() {

    vector<AbstractPieceManager::Shape> shapes;
    auto numberOfPieces = static_cast<int>(pieces.size());
    for (int i = 1; i * i <= numberOfPieces; i++) {
        if (numberOfPieces % i != 0)
            continue;
        Shape s;
        s.width = i;
        s.height = (numberOfPieces / i);
        if (this->isPuzzleShapePossible(s))
            shapes.emplace_back(s);
    }
    return shapes;
}

void RotatablePieceManager::addPiece(unique_ptr<PuzzlePiece> piece) {
    this->pieces.emplace_back(piece.get());
    this->addPieceToRepository(piece->representor());
}

bool RotatablePieceManager::hasASumOfZero() {
    int sum = 0;
    for (auto piece : this->pieces)
        sum += piece.right + piece.left + piece.down + piece.up;
    return sum == 0;
}

bool RotatablePieceManager::hasAllCorners() {
    return this->numberOfCorners() >= 1; // todo implement a better solution
}

int RotatablePieceManager::numberOfCorners() {
    int corners = 0;
    for (auto piece : this->pieces)
        if ((piece.up == 0 || piece.down == 0) && (piece.left == 0 || piece.right == 0))
            ++corners;
    return corners;
}

void RotatablePieceManager::printMissingCorners(ofstream &fout) {
    auto corners = this->numberOfCorners();
    if (corners >= 1)// todo implement a better solution
        return;

    fout << "There are only " << corners << " corners" << endl;
}

RotatablePieceManager::RotatablePieceManager() {
    this->initialLookupTable();
}

void RotatablePieceManager::initialLookupTable() {
    for (int l = 0; l <= nullPiece; l++) //for each piece_t
    {
        for (int t = 0;; t++) //look for the first permutation
        {
            if (this->isPermutation(l, t)) {
                this->lookupTable[l] = t; //set as representor
                break;
            }
        }
    }
}

bool RotatablePieceManager::isPermutation(Piece_t p1, Piece_t p2) {
    for (int i = 0; i < 4; i++) {
        if (p1 == p2)
            return true;
        p1 = rotatePieceCounterClockWise(p1); //roll one left
    }
    return false;
}

int RotatablePieceManager::getPermutationDegree(Piece_t current, Piece_t origin) {
    for (int i = 0; i < 4; i++) {
        if (current == origin)
            return i * 90;
        current = rotatePieceCounterClockWise(current); //roll one left
    }
    cerr << "Error: Couldn't get permutation degree";
    return 0;
}

void RotatablePieceManager::removePieceFromRepository(Piece_t piece) {
    if (--this->pieceRepository[lookupTable[piece]] == 0)
        this->removeFromConstrainRepository(piece);

}

void RotatablePieceManager::addPieceToRepository(Piece_t piece) {
    if (this->pieceRepository[lookupTable[piece]]++ == 0)
        this->addToConstrainRepository(piece);
}

int RotatablePieceManager::numOfOptionsForConstrain(Piece_t constrain) {
    return this->constrainRepository[constrain];
}

void RotatablePieceManager::removeFromConstrainRepository(Piece_t piece) {
    changeConstrains(piece, -1);
}

void RotatablePieceManager::addToConstrainRepository(Piece_t piece) {
    changeConstrains(piece, 1);
}

void RotatablePieceManager::changeConstrains(Piece_t piece, const int delta) {
    Piece_t p1 = piece;
    for (Piece_t maskOption : this->maskOptions)
        this->constrainRepository[p1 | maskOption] += delta;

    Piece_t p2 = rotatePieceCounterClockWise(p1);
    if (p1 == p2)
        return; // piece completely symmetric

    // else - add it:
    for (Piece_t maskOption : this->maskOptions)
        this->constrainRepository[p2 | maskOption] += delta;

    Piece_t p3 = rotatePieceCounterClockWise(p2);
    if (p1 == p3)
        return; // piece is symmetric to 180 degree turns

    Piece_t p4 = rotatePieceCounterClockWise(p3);
    for (Piece_t maskOption : this->maskOptions) {
        this->constrainRepository[p3 | maskOption] += delta;
        this->constrainRepository[p4 | maskOption] += delta;
    }
}

bool RotatablePieceManager::isPuzzleShapePossible(AbstractPieceManager::Shape shape) {
    if (shape.width == 1) {
        auto straightTest = [](PuzzlePiece c) { return (c.up == 0 && c.down == 0) || (c.right == 0 && c.left == 0); };
        return std::all_of(this->pieces.begin(), this->pieces.end(), straightTest);
    }
    int numberOfStraightNeeded = shape.height * 2 + shape.width * 2 - 4;
    for (auto piece : this->pieces) {
        if (piece.left == 0 || piece.right == 0 ||
            piece.down == 0 || piece.up == 0) {
            if (--numberOfStraightNeeded == 0)
                return true;
        }
    }
    return false;
}

void RotatablePieceManager::printPiece(Piece_t piece, ofstream &out) {
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if (isPermutation(it->representor(), piece) && !it->wasUsedInSolution) {
            it->wasUsedInSolution = true;
            out << it->index;
            int degree = getPermutationDegree(piece, it->representor());
            if (degree != 0)
                out << " [" << degree << "]";
            return;
        }
    }
    cerr << "Error: couldn't find piece: " << int(piece) << endl;
}

inline bool RotatablePieceManager::pieceExistInRepository(Piece_t piece) {
    return this->pieceRepository[lookupTable[piece]] > 0;
}

inline Piece_t RotatablePieceManager::rotatePieceCounterClockWise(Piece_t piece) {
    return (piece << 2) | (piece >> 6); //roll one left
}
