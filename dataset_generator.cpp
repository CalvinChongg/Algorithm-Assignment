// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC5L
// Tutorial Class: T17L
// Trimester: 2610
// Member_1: 242UC244KX | YAP CHI YI | yap.chi.yi@student.mmu.edu.my | 0182694514
// Member_2: 242UC244JT | CHONG MENG HANG | chong.meng.hang@student.mmu.edu.my | 0168711296
// Member_3: 242UC244S6 | THAM MEI TING | tham.mei.ting@student.mmu.edu.my | 0173268006
// Member_4: 242UC244PF | TAN YONG XIN | tan.yong.xin@student.mmu.edu.my | 0126556505
// *********************************************************
// Task Distribution
// Member_1: Radix sort
// Member_2: Hash Table Search
// Member_3: Heap Sort
// Member_4: Dataset Generator
// ***************************************************** 

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

using namespace std;

int main() {
    // ID 242UC244JT
    unsigned long long seed = 2421324400ULL; 
    
    // Initialize the random number generator
    mt19937_64 rng(seed);
    
    long long targetSize = 3000; 


    // Basic validation
    if (targetSize <= 0) {
        cerr << "Error: Dataset size must be greater than 0.\n";
        return 1;
    }

    // Define the ranges for generation
    uniform_int_distribution<long long> intDist(1000000000LL, 9999999999LL); // 10-digit range
    uniform_int_distribution<int> charDist(0, 25); // a-z range

    string filename = "dataset_" + to_string(targetSize) + ".csv";
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cerr << "Error creating file: " << filename << "\n";
        return 1;
    }

    cout << "Generating " << filename << " with " << targetSize << " rows..." << flush;

    unordered_set<long long> unique_numbers;
    
    // Ensure the hash table has enough buckets beforehand to speed up massive generations
    unique_numbers.reserve(targetSize); 

    // 3. GENERATION LOOP
    while (unique_numbers.size() < targetSize) {
        long long randomNum = intDist(rng);
        
        // If the number is NOT found in the set, it's unique
        if (unique_numbers.find(randomNum) == unique_numbers.end()) {
            unique_numbers.insert(randomNum); // Add to our unique checklist
            
            // Generate 5-letter string
            string randomStr = "";
            for(int j = 0; j < 5; j++) {
                randomStr += (char)('a' + charDist(rng));
            }
            
            // Write to CSV format: 1000000038,uoren
            outFile << randomNum << "," << randomStr << "\n";
        }
    }
    
    outFile.close();
    cout << " Done!\n";

    return 0;
}