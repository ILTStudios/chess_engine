#pragma once
#include <cstdint>

class Board;

class Validator{
    public:
        bool is_valid(int player_turn, int from_square, int to_square, const Board & board); // returns 0 if not valid move, returns 1 if vaild
        void initialise(); // initialises all validation data
        void update_castleing_rights(int from_square, int to_square);
        bool is_in_check(const Board &board, int player_turn);
        bool has_legal_moves(const Board &board, int player_turn);
        int enpassen_square;

    private:
        // general piece move validation biboards
        uint64_t knight_moves[64];
        uint64_t w_pawn_moves[64];
        uint64_t w_pawn_captures[64];
        uint64_t b_pawn_moves[64];
        uint64_t b_pawn_captures[64];
        uint64_t king_moves[64];
        uint64_t rook_moves[64];
        uint64_t bishop_moves[64];
        uint64_t queen_moves[64];
        //

        // magic bitboard nonsense
            // magic numbers from Leela Chess Code https://github.com/LeelaChessZero/lc0/blob/master/src/chess/board.cc
        static constexpr uint64_t rookMagicNumbers[64] = {
            0x088000102088C001ULL, 0x10C0200040001000ULL, 0x83001041000B2000ULL,
            0x0680280080041000ULL, 0x488004000A080080ULL, 0x0100180400010002ULL,
            0x040001C401021008ULL, 0x02000C04A980C302ULL, 0x0000800040082084ULL,
            0x5020C00820025000ULL, 0x0001002001044012ULL, 0x0402001020400A00ULL,
            0x00C0800800040080ULL, 0x4028800200040080ULL, 0x00A0804200802500ULL,
            0x8004800040802100ULL, 0x0080004000200040ULL, 0x1082810020400100ULL,
            0x0020004010080040ULL, 0x2004818010042800ULL, 0x0601010008005004ULL,
            0x4600808002001400ULL, 0x0010040009180210ULL, 0x020412000406C091ULL,
            0x040084228000C000ULL, 0x8000810100204000ULL, 0x0084110100402000ULL,
            0x0046001A00204210ULL, 0x2001040080080081ULL, 0x0144020080800400ULL,
            0x0840108400080229ULL, 0x0480308A0000410CULL, 0x0460324002800081ULL,
            0x620080A001804000ULL, 0x2800802000801006ULL, 0x0002809000800800ULL,
            0x4C09040080802800ULL, 0x4808800C00800200ULL, 0x0200311004001802ULL,
            0x0400008402002141ULL, 0x0410800140008020ULL, 0x000080C001050020ULL,
            0x004080204A020010ULL, 0x0224201001010038ULL, 0x0109001108010004ULL,
            0x0282004844020010ULL, 0x8228180110040082ULL, 0x0001000080C10002ULL,
            0x024000C120801080ULL, 0x0001406481060200ULL, 0x0101243200418600ULL,
            0x0108800800100080ULL, 0x4022080100100D00ULL, 0x0000843040600801ULL,
            0x8301000200CC0500ULL, 0x1000004500840200ULL, 0x1100104100800069ULL,
            0x2001008440001021ULL, 0x2002008830204082ULL, 0x0010145000082101ULL,
            0x01A2001004200842ULL, 0x1007000608040041ULL, 0x000A08100203028CULL,
            0x02D4048040290402ULL};
        static constexpr uint64_t bishopMagicNumbers[64] = {
            0x0008201802242020ULL, 0x0021040424806220ULL, 0x4006360602013080ULL,
            0x0004410020408002ULL, 0x2102021009001140ULL, 0x08C2021004000001ULL,
            0x6001031120200820ULL, 0x1018310402201410ULL, 0x401CE00210820484ULL,
            0x001029D001004100ULL, 0x2C00101080810032ULL, 0x0000082581000010ULL,
            0x10000A0210110020ULL, 0x200002016C202000ULL, 0x0201018821901000ULL,
            0x006A0300420A2100ULL, 0x0010014005450400ULL, 0x1008C12008028280ULL,
            0x00010010004A0040ULL, 0x3000820802044020ULL, 0x0000800405A02820ULL,
            0x8042004300420240ULL, 0x10060801210D2000ULL, 0x0210840500511061ULL,
            0x0008142118509020ULL, 0x0021109460040104ULL, 0x00A1480090019030ULL,
            0x0102008808008020ULL, 0x884084000880E001ULL, 0x040041020A030100ULL,
            0x3000810104110805ULL, 0x04040A2006808440ULL, 0x0044040404C01100ULL,
            0x4122B80800245004ULL, 0x0044020502380046ULL, 0x0100400888020200ULL,
            0x01C0002060020080ULL, 0x4008811100021001ULL, 0x8208450441040609ULL,
            0x0408004900008088ULL, 0x0294212051220882ULL, 0x000041080810E062ULL,
            0x10480A018E005000ULL, 0x80400A0204201600ULL, 0x2800200204100682ULL,
            0x0020200400204441ULL, 0x0A500600A5002400ULL, 0x801602004A010100ULL,
            0x0801841008040880ULL, 0x10010880C4200028ULL, 0x0400004424040000ULL,
            0x0401000142022100ULL, 0x00A00010020A0002ULL, 0x1010400204010810ULL,
            0x0829910400840000ULL, 0x0004235204010080ULL, 0x1002008143082000ULL,
            0x11840044440C2080ULL, 0x2802A02104030440ULL, 0x6100000900840401ULL,
            0x1C20A15A90420200ULL, 0x0088414004480280ULL, 0x0000204242881100ULL,
            0x0240080802809010ULL};
            //
        uint64_t rook_masks[64];
        uint64_t bishop_masks[64];
        uint64_t set_occupancy(int index, int bits_in_mask, uint64_t mask);
        //

        // special piece move validation data
        bool white_castle_ks;
        bool white_castle_qs;
        bool black_castle_ks;
        bool black_castle_qs;
        //

        // move bitboard functions
        void init_knight_moves();
        void init_pawn_moves();
        void init_king_moves();
        void init_rook_moves();
        void init_bishop_moves();
        //

        // validation functions
        bool is_valid_move(int from_mask, int to_mask, int player_turn, const Board &board);
        bool is_path_clear(int from_index, int to_index, const Board &board);
        bool is_valid_castle(int from_square, int to_square, int player_turn, const Board &board);
        bool is_square_attacked(int square, int player_turn, const Board &board);
        bool is_valid_pawn_move(int from_mask, uint64_t to_mask, int player_turn, const Board &board);
        bool is_valid_knight_move(int from_mask, uint64_t to_mask);
        bool is_valid_bishop_move(int from_mask, uint64_t to_mask, const Board &board);
        bool is_valid_rook_move(int from_mask, uint64_t to_mask, const Board &board);
        bool is_valid_king_move(int from_mask, uint64_t to_mask);
        bool is_valid_queen_move(int from_mask, uint64_t to_mask, const Board &board);
};