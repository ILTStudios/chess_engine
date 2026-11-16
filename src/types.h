#pragma once
#include <cstdint>

enum class Color : uint8_t {White, Black};
enum class PieceType : uint8_t {Pawn, Knight, Bishop, Rook, Queen, King, None};

enum class MoveType : uint8_t {
    Normal,
    DoublePush,
    Capture,
    EnPassant,
    KingSideCastle,
    QueenSideCastle,
    Promotion,
    PromotionCapture
};

struct Move{
    int from = 0;
    int to = 0;
    MoveType type = MoveType::Normal;
    PieceType promotion  = PieceType::None;
};

inline Color opposite(Color c){
    return (c == Color::White) ? Color::Black : Color::White;
}