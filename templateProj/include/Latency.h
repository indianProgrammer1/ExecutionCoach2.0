#pragma once
#include <vector>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <chrono>

class LatencyStats {
private:
    std::vector<double> samples;
    std::mutex mu;
public:
    static LatencyStats& getInstance() {
        static LatencyStats instance;
        return instance;
    }

    void record(double microseconds) {
        std::lock_guard<std::mutex> lock(mu);
        samples.push_back(microseconds);
    }

    void printSummary() {
        //sort the samples vector and calculate its average
        //takes the median value,95th precentile and 99th precentile
        std::lock_guard<std::mutex> lock(mu);
        if (samples.empty()) return;

        std::sort(samples.begin(), samples.end());

        double avg = 0;
        for (double x : samples) avg += x;
        avg /= samples.size();

        
        auto p50 = samples[samples.size() * 50 / 100];
        auto p95 = samples[samples.size() * 95 / 100];
        auto p99 = samples[samples.size() * 99 / 100];

        std::cout << "\n=== Latency (Milliseconds) ===\n";
        std::cout << "Samples: " << samples.size() << "\n";
        std::cout << "Average: " << avg << "\n";
        std::cout << "p50:     " << p50 << "\n";
        std::cout << "p95:     " << p95 << "\n";
        std::cout << "p99:     " << p99 << "\n";
        std::cout << "==============================\n";
    }


};
