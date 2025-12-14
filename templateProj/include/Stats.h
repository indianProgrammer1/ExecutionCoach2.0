#pragma once
#include <atomic>
#include <iostream>
//atomic variables to protect its value
// when multiple threads access it simultaneously
struct Stats {
    static inline std::atomic<int> processed_lines = 0;
    static inline std::atomic<int> parser_errors = 0;
    static inline std::atomic<int> db_errors = 0;
    static inline std::atomic<int> ok_count = 0;
    static inline std::atomic<int> warn_count = 0;
    static inline std::atomic<int> adjust_count = 0;
    static inline std::atomic<int> block_count = 0;


    static inline void printSummary() {
        std::cout << "\n=== Execution Summary ===\n";
		std::cout << "Total trades: " << processed_lines.load()+ parser_errors.load() << "\n";
        std::cout << "Processed trades: " << processed_lines.load() << "\n";
        std::cout << "Parser errors:    " << parser_errors.load() << "\n";
        std::cout << "DB write errors:  " << db_errors.load() << "\n\n";
        std::cout << "Decisions:\n";
        std::cout << "OK          : " << ok_count.load() << "\n";
        std::cout << "WARN        : " << warn_count.load() << "\n";
        std::cout << "ADJUST_QTY  : " << adjust_count.load() << "\n";
        std::cout << "BLOCK       : " << block_count.load() << "\n";
        std::cout << "==========================\n";
    }
};
