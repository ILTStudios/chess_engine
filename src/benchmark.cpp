#include <iostream>
#include <chrono>
#include "board.h"
#include "validator.h"

using namespace std;
using namespace chrono;

int main() {

    Board board;
    Validator validator;

    board.initialise("std");
    validator.initialise();

    uint64_t checksum = 0;

    const int iterations = 100000;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; i++) {
        for (int from = 0; from < 64; from++) {
            for (int to = 0; to < 64; to++) {
                checksum += validator.is_valid(
                    1, from, to, board
                );
            }
        }
    }

    auto end = high_resolution_clock::now();
    double seconds = duration<double>(end - start).count();
    double positions_per_second = (iterations * 64.0 * 64.0) / seconds;

    cout << "Time: " << seconds << " seconds\n";
    cout << "Candidate moves checked per second: "
         << positions_per_second << "\n";

    cout << "Checksum: " << checksum << endl;
}