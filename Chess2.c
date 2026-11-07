#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

class Chess
{
private:
    unsigned int turncount;
    string player_input;
    vector<string> input_list;

    // all bit boards for the pieces
    // all white pieces
    uint64_t white_pawns;
    uint64_t white_rooks;
    uint64_t white_knights;
    uint64_t white_bishops;
    uint64_t white_king;
    uint64_t white_queen;

    // all black pieces
    uint64_t black_pawns;
    uint64_t black_rooks;
    uint64_t black_knights;
    uint64_t black_bishops;
    uint64_t black_king;
    uint64_t black_queen;

    // supplementary bitboards
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t occupied_spaces;
    uint64_t empty_squares;

    //validation bitboards
    uint64_t knight_moves[64];
    void init_knight_moves();
    uint64_t w_pawn_moves[64];
    uint64_t b_pawn_moves[64];
    uint64_t w_pawn_captures[64];
    uint64_t b_pawn_captures[64];
    void init_pawn_moves();
    uint64_t king_moves[64];
    void init_king_moves();
    uint64_t rook_moves[64];
    void init_rook_moves();
    uint64_t bishop_moves[64];
    void init_bishop_moves();
    uint64_t queen_moves[64];
    
    // special move validation
    int enpassen_square;
    bool white_castle_ks;
    bool white_castle_qs;
    bool black_castle_ks;
    bool black_castle_qs;

public:
    Chess(); ~Chess();
    void display_board();
    void get_input();
    void change_board(string move);
    int validate_move(string play);
    void authenticate_win();
    char what_piece(string square);
    bool is_valid_piece(string square);
    bool validate_piece_move(string from_square, string to_square);
    bool has_ended;
};

// all init piece bitboard moves
void Chess::init_knight_moves(){    
    int rank_offsets[] = { 2,  1, -1, -2, -2, -1,  1,  2 };
    int file_offsets[] = { 1,  2,  2,  1, -1, -2, -2, -1 };

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 8; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            if(new_rank >= 0 && new_rank < 8  && new_file >= 0 && new_file < 8){
                int target_square = (new_rank * 8) + new_file;
                moves |= (1ULL << target_square);
            }
        }
        knight_moves[square] = moves;
    }
    cout << "Successfully generated all knight moves!" << endl;
}

void Chess::init_king_moves(){
    int rank_offsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int file_offsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 8; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            if(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                int target_square = (new_rank * 8) + new_file;
                moves |= (1ULL << target_square);
            }
        }

        king_moves[square] = moves;
    }
    cout << "Successfully generated all king moves!" << endl;
}

void Chess::init_pawn_moves(){
    for(int square = 0; square < 64; square += 1){
        uint64_t current_square = (1ULL << square);
        uint64_t w_moves = 0;
        uint64_t b_moves = 0;
        
        if(square >= 8 && square < 15){
            w_moves |= (current_square << 16);
        }
        if(square >= 48 && square < 56){
            b_moves |= (current_square >> 16);
        }

        w_moves |= (current_square << 8);
        uint64_t w_captures = 0;
        if ((current_square & 0xFEFEFEFEFEFEFEFEULL) != 0) w_captures |= (current_square << 7);
        if ((current_square & 0x7F7F7F7F7F7F7F7FULL) != 0) w_captures |= (current_square << 9);
        w_pawn_captures[square] = w_captures;
        w_pawn_moves[square] = w_moves;

        b_moves |= (current_square >> 8);
        uint64_t b_captures = 0;
        if ((current_square & 0xFEFEFEFEFEFEFEFEULL) != 0) b_captures |= (current_square >> 9);
        if ((current_square & 0x7F7F7F7F7F7F7F7FULL) != 0) b_captures |= (current_square >> 7);
        b_pawn_captures[square] = b_captures;
        b_pawn_moves[square] = b_moves;
    }
    cout << "Successfully generated all pawns moves!" << endl;
}

void Chess::init_rook_moves(){
    int rank_offsets[] = {1, -1, 0, 0};
    int file_offsets[] = {0, 0, 1, -1};

    for (int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }

        rook_moves[square] = moves;
    }
    cout << "Successfully generated all rook moves!" << endl;
}

void Chess::init_bishop_moves(){
    int rank_offsets[] = {1, 1, -1, -1};
    int file_offsets[] = {1, -1, 1, -1};

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }
        bishop_moves[square] = moves;
    }

    cout << "Sucessfully generated all bishop moves!" << endl;
}

// Constructor for the game, dont really know if i should keep this but whatever
Chess::Chess(){
    turncount = 1; player_input = ""; has_ended = false;

    // special rule
    enpassen_square = -1;
    white_castle_ks = true;
    white_castle_qs = true;
    black_castle_ks = true;
    black_castle_qs = true;

    // setting all piece bitboards for standard game
    white_rooks = (1ULL << 0) | (1ULL << 7);
    white_knights = (1ULL << 1) | (1ULL << 6);
    white_bishops = (1ULL << 2) | (1ULL << 5);
    white_queen = (1ULL << 3);
    white_king = (1ULL << 4);
    white_pawns = (0xFFULL << 8);

    // setting all piece bitboards for standard game
    black_rooks = (1ULL << 63) | (1ULL << 56);
    black_knights = (1ULL << 62) | (1ULL << 57);
    black_bishops = (1ULL << 61) | (1ULL << 58);
    black_queen = (1ULL << 59);
    black_king = (1ULL << 60);
    black_pawns = (0xFFULL << 48);

    // defining all the other supplementary bitboards
    white_pieces = white_rooks | white_knights | white_bishops | white_queen | white_king | white_pawns;
    black_pieces = black_rooks | black_knights | black_bishops | black_queen | black_king | black_pawns;
    occupied_spaces = white_pieces | black_pieces;
    empty_squares = ~occupied_spaces;

    // defining all the validation bitboards
    init_knight_moves();
    init_king_moves();
    init_pawn_moves();
    init_rook_moves();
    init_bishop_moves();

    for(int square = 0; square < 64; square += 1){
        uint64_t rook_mask = rook_moves[square];
        uint64_t bishop_mask = bishop_moves[square];

        queen_moves[square] = (rook_mask | bishop_mask);
    }

}

// Destructor for when the game ends, pretty useful
Chess::~Chess(){
    cout << "\n\nThe game has ended! " << ((turncount == 1) ? "White " : "Black ") << "has won! \n";
}

// changes inputs like e2 into readable bitboard code
int algebraic_to_index(string square){
    int file = square[0] - 'a';
    int rank = square[1] - '1';

    return (rank * 8) + file;
}

// validates if a square is within the bounds of our game
bool is_valid_square(string square){
    if (square.length() != 2) return false;

    char file = tolower(square[0]);
    char rank = square[1];

    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
}

bool Chess::is_valid_piece(string square){
    if (square.length() != 2) return false;

    int index = algebraic_to_index(square);
    uint64_t mask = (1ULL << index);
    
    if (turncount == 1) {
        return (white_pieces & mask) != 0;
    } else {
        return (black_pieces & mask) != 0;
    }
}

char Chess::what_piece(string square){
    int index = algebraic_to_index(square);
    uint64_t mask = (1ULL << index);

    if      (white_pawns & mask)   return 'P';
    else if (white_knights & mask) return 'N';
    else if (white_bishops & mask) return 'B';
    else if (white_rooks & mask)   return 'R';
    else if (white_queen & mask)   return 'Q';
    else if (white_king & mask)    return 'K';
    
    else if (black_pawns & mask)   return 'p';
    else if (black_knights & mask) return 'n';
    else if (black_bishops & mask) return 'b';
    else if (black_rooks & mask)   return 'r';
    else if (black_queen & mask)   return 'q';
    else if (black_king & mask)    return 'k';

    return '.'; // Empty square
}

bool Chess::validate_piece_move(string from_square, string to_square){
    char from_piece = toupper(what_piece(from_square));
    char og_from_piece = what_piece(from_square);
    int from_index = algebraic_to_index(from_square);
    int to_index = algebraic_to_index(to_square);
    uint64_t to_mask = (1ULL << to_index);

    if(og_from_piece == 'p'){
        return(b_pawn_moves[from_index] & to_mask || b_pawn_captures[from_index] & to_mask) != 0;
    }
    if(og_from_piece == 'P'){
        return (w_pawn_moves[from_index] & to_mask || w_pawn_captures[from_index] & to_mask) != 0;
    }
    if(from_piece == 'N'){
        return (knight_moves[from_index] & to_mask) != 0;
    }
    if(from_piece == 'B'){
        return (bishop_moves[from_index] & to_mask) != 0;
    }
    if(from_piece == 'R'){
        return (rook_moves[from_index] & to_mask) != 0;
    }
    if(from_piece == 'Q'){
        return (queen_moves[from_index] & to_mask) != 0;
    }
    if(from_piece == 'K'){
        return (king_moves[from_index] & to_mask) != 0;
    }
    if(from_piece == '.'){
        return 0;
    }

    return 0;
}

// validates the move
int Chess::validate_move(string move){
    // is that a valid input?
    if(move.length() != 4){
        cout << "Invalid Move! Please enter a move from square to square (eg: e2e4) \n";
        return 0;
    }

    string from_square = move.substr(0, 2);
    string to_square = move.substr(2,2);

    // is that a valid square?
    if(!is_valid_square(from_square) || !is_valid_square(to_square)){
        cout << "Out of Bounds! Please enter a move from the file a - h and rank 1 - 8 \n";
        return 0;        
    }

    // is that a piece and can you move it?
    if (!is_valid_piece(from_square))
    {
        cout << "Please choose a piece of your own, not an empty spot. \n";
        return 0;
    }
    
    // can that piece actually go there?
    // identify what type of piece it is
    if(!validate_piece_move(from_square, to_square)){
        cout << "That piece can not move there. \n";
        return 0;
    }

    if(from_square == "e1" && to_square == "g1" && white_castle_ks == true){
        white_castle_ks = false;
    }
    if(from_square == "e1" && to_square == "c1" && white_castle_qs == true){
        white_castle_qs = false;
    }
    if(from_square == "e8" && to_square == "g8" && black_castle_ks == true){
        black_castle_ks = false;
    }
    if(from_square == "e1" && to_square == "c8" && black_castle_qs == true){
        black_castle_qs = false;
    }

    change_board(move);
    return 1;
}

// Gets someone's user input
void Chess::get_input(){
    while (turncount <= 2)
    {
        cout << "Player " << ((turncount == 1) ? "White, " : "Black, ") << "Choose your move: ";
        cin >> player_input;
        turncount += validate_move(player_input);
    }
    turncount = 1;
    cout << endl;
}

// Change the game state of the board
void Chess::change_board(string move){
    string from_square = move.substr(0, 2);
    string to_square = move.substr(2, 2);

    int from_index = algebraic_to_index(from_square);
    int to_index = algebraic_to_index(to_square);

    uint64_t from_mask = 1ULL << from_index;
    uint64_t to_mask = 1ULL << to_index;

    if      (white_pawns & from_mask)   { white_pawns   = (white_pawns & ~from_mask) | to_mask; }
    else if (white_knights & from_mask) { white_knights = (white_knights & ~from_mask) | to_mask; }
    else if (white_bishops & from_mask) { white_bishops = (white_bishops & ~from_mask) | to_mask; }
    else if (white_rooks & from_mask)   { white_rooks   = (white_rooks & ~from_mask) | to_mask; }
    else if (white_queen & from_mask)   { white_queen   = (white_queen & ~from_mask) | to_mask; }
    else if (white_king & from_mask)    { white_king    = (white_king & ~from_mask) | to_mask; }
    
    else if (black_pawns & from_mask)   { black_pawns   = (black_pawns & ~from_mask) | to_mask; }
    else if (black_knights & from_mask) { black_knights = (black_knights & ~from_mask) | to_mask; }
    else if (black_bishops & from_mask) { black_bishops = (black_bishops & ~from_mask) | to_mask; }
    else if (black_rooks & from_mask)   { black_rooks   = (black_rooks & ~from_mask) | to_mask; }
    else if (black_queen & from_mask)   { black_queen   = (black_queen & ~from_mask) | to_mask; }
    else if (black_king & from_mask)    { black_king    = (black_king & ~from_mask) | to_mask; }
    else {
        cout << "No piece found on that starting square!\n";
        return;
    }

    white_pieces = white_rooks | white_knights | white_bishops | white_queen | white_king | white_pawns;
    black_pieces = black_rooks | black_knights | black_bishops | black_queen | black_king | black_pawns;
    occupied_spaces = white_pieces | black_pieces;
    empty_squares = ~occupied_spaces;

    display_board();
}

// Authenticate whether a win scenario has happened or not
void Chess::authenticate_win(){

}

// Displays the entire board
void Chess::display_board(){
    for(int rank = 7; rank >= 0; --rank){
        cout << (rank + 1) << " ";

        for(int file = 0; file < 8; ++file){
            int square = (rank * 8) + file;
            uint64_t mask = 1ULL << square;

            char symbol = '.';

            if      (white_pawns & mask)   symbol = 'P';
            else if (white_knights & mask) symbol = 'N';
            else if (white_bishops & mask) symbol = 'B';
            else if (white_rooks & mask)   symbol = 'R';
            else if (white_queen & mask)   symbol = 'Q';
            else if (white_king & mask)    symbol = 'K';
            
            else if (black_pawns & mask)   symbol = 'p';
            else if (black_knights & mask) symbol = 'n';
            else if (black_bishops & mask) symbol = 'b';
            else if (black_rooks & mask)   symbol = 'r';
            else if (black_queen & mask)   symbol = 'q';
            else if (black_king & mask)    symbol = 'k';

            cout << symbol << " ";
        }
        cout << endl;
    }
    cout << "  A B C D E F G H\n\n";
}

int main(){
    // Chess chess;

    // while(!chess.has_ended){
    //     chess.display_board();
    //     chess.get_input();
    // }

    // return 0;
}