#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;

// 1. DATA STRUCTURE
struct Record {
    long long number;
    string text;
};

// 2. PRIME NUMBER MATH (Dynamic Scaling)
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int getOptimalPrime(int datasetSize) {
    for (int i = datasetSize; i > 1; i--) {
        if (isPrime(i)) {
            return i;
        }
    }
    return 997; 
}

int primeDivisor = 0;

int hashFunction(long long key) {
    return key % primeDivisor;
}

int main() {
    // ==========================================
    // 1. CONFIGURATION
    // Change inputSize to test different dataset amounts
    // ==========================================
    string inputSize = "10000000"; 
    string inputFilename = "dataset_" + inputSize + ".csv"; 
    string outputFilename = "hash_table_search_dataset_" + inputSize + ".txt";

    // ==========================================
    // 2. READ THE DATASET
    // ==========================================
    cout << "Reading data from " << inputFilename << "...\n";
    ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open " << inputFilename << " (Did you run dataset_generator.cpp?)\n";
        return 1;
    }

    vector<Record> dataset;
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string numStr, textStr;
        // Handles standard comma-separated format
        if (getline(ss, numStr, ',') && getline(ss, textStr)) {
            Record rec;
            rec.number = stoll(numStr);
            rec.text = textStr;
            dataset.push_back(rec);
        }
    }
    inFile.close();

    int n = dataset.size();
    if (n == 0) {
        cerr << "Error: File is empty.\n";
        return 1;
    }

    // ==========================================
    // 3. BUILD THE HASH TABLE
    // ==========================================
    primeDivisor = getOptimalPrime(n);
    vector<vector<Record>> hashTable(primeDivisor); 

    for (int i = 0; i < n; i++) {
        int index = hashFunction(dataset[i].number);
        hashTable[index].push_back(dataset[i]);
    }

    // ==========================================
    // 4. FIND BEST AND WORST CASE TARGETS
    // ==========================================
    long long bestCaseTarget = -1;
    long long worstCaseTarget = -1;
    int maxChainIndex = 0;
    size_t maxChainSize = 0;

    // Find Best Case: The very first item parked in the very first populated bucket
    for (int i = 0; i < primeDivisor; i++) {
        if (!hashTable[i].empty()) {
            bestCaseTarget = hashTable[i][0].number;
            break;
        }
    }

    // Find Worst Case: Find the most crowded bucket, then calculate a "fake" 
    // number that forces the search to check the entire chain
    for (int i = 0; i < primeDivisor; i++) {
        if (hashTable[i].size() > maxChainSize) {
            maxChainSize = hashTable[i].size();
            maxChainIndex = i;
        }
    }
    worstCaseTarget = maxChainIndex + (100000000LL * primeDivisor); 

    // ==========================================
    // 5. RUN THE TIMING TESTS
    // Document Requirement: Perform 'n' searches where n is the dataset size.
    // ==========================================
    
    // --- BEST CASE TEST ---
    auto start_best = chrono::high_resolution_clock::now();
    for (int r = 0; r < n; r++) {
        int index = hashFunction(bestCaseTarget);
        for (size_t i = 0; i < hashTable[index].size(); i++) {
            if (hashTable[index][i].number == bestCaseTarget) break;
        }
    }
    auto end_best = chrono::high_resolution_clock::now();
    double bestTime = chrono::duration<double>(end_best - start_best).count() / n;

    // --- WORST CASE TEST ---
    auto start_worst = chrono::high_resolution_clock::now();
    for (int r = 0; r < n; r++) {
        int index = hashFunction(worstCaseTarget);
        for (size_t i = 0; i < hashTable[index].size(); i++) {
            if (hashTable[index][i].number == worstCaseTarget) break;
        }
    }
    auto end_worst = chrono::high_resolution_clock::now();
    double worstTime = chrono::duration<double>(end_worst - start_worst).count() / n;

    // --- AVERAGE CASE TEST ---
    // Search for every single item in the original dataset once (exactly 'n' searches)
    auto start_avg = chrono::high_resolution_clock::now();
    for (int r = 0; r < n; r++) {
        long long target = dataset[r].number;
        int index = hashFunction(target);
        for (size_t i = 0; i < hashTable[index].size(); i++) {
            if (hashTable[index][i].number == target) break;
        }
    }
    auto end_avg = chrono::high_resolution_clock::now();
    double averageTime = chrono::duration<double>(end_avg - start_avg).count() / n;

    // ==========================================
    // 6. OUTPUT RESULTS
    // Document Requirement: Print in file and command prompt window.
    // ==========================================
    
    // Print to the Command Prompt Window
    cout << "\n--- SEARCH RESULTS (n = " << n << ") ---\n";
    cout << fixed << setprecision(9);
    cout << "Best case time: " << bestTime << " seconds\n";
    cout << "Average case time: " << averageTime << " seconds\n";
    cout << "Worst case time: " << worstTime << " seconds\n";
    cout << "----------------------------------\n";

    // Write to the output .txt file
    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create " << outputFilename << "\n";
        return 1;
    }
    outFile << fixed << setprecision(9);
    outFile << "Best case time: " << bestTime << " seconds\n";
    outFile << "Average case time: " << averageTime << " seconds\n";
    outFile << "Worst case time: " << worstTime << " seconds\n";
    outFile.close();
    
    cout << "\nTesting complete! Results written to " << outputFilename << "\n";

    return 0;
}