#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// ---------------------------------------------------------
// 1. DATA STRUCTURE
// ---------------------------------------------------------
struct Record {
    long long number;
    string text;
};

// ---------------------------------------------------------
// 2. PRIME NUMBER MATH (Dynamic Scaling)
// ---------------------------------------------------------
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; (long long)i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int getOptimalPrime(int datasetSize) {
    for (int i = datasetSize; i > 1; i--) {
        if (isPrime(i)) return i;
    }
    return 997;
}

int primeDivisor = 0;

int hashFunction(long long key) {
    return (int)(key % primeDivisor);
}

int main() {
    // ==========================================
    // 1. CONFIGURATION
    // ==========================================
    string inputSize = "1000";
    string inputFilename = "dataset_" + inputSize + ".csv";

    // ==========================================
    // 2. TARGET SELECTION
    // Use a value copied from the dataset to trace a FOUND search,
    // or a value outside the valid range / not present in the
    // dataset to trace a NOT-FOUND search.
    // ==========================================
    long long target = 7548885293,ifzxn;

    // ==========================================
    // 3. READ THE DATASET
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
        if (getline(ss, numStr, ',') && getline(ss, textStr)) {
            Record rec;
            rec.number = stoll(numStr);
            rec.text = textStr;
            dataset.push_back(rec);
        }
    }
    inFile.close();

    int n = (int)dataset.size();
    if (n == 0) {
        cerr << "Error: File is empty.\n";
        return 1;
    }

    // ==========================================
    // 4. BUILD THE HASH TABLE (array/vector based chaining)
    // ==========================================
    primeDivisor = getOptimalPrime(n);
    vector<vector<Record>> hashTable(primeDivisor);

    for (int i = 0; i < n; i++) {
        int idx = hashFunction(dataset[i].number);
        hashTable[idx].push_back(dataset[i]);
    }

    // ==========================================
    // 5. SEARCH STEP TRACE
    //    - compute the bucket index from the target
    //    - walk the chain in insertion order
    //    - print every comparison made ("!=" for a miss, "=" for a hit)
    //    - if the chain is empty, or exhausted without a hit, print the
    //      sentinel line "-1 != target" to mark the search as failed
    // ==========================================
    string outputFilename = inputSize + "_dataset" + "_hash_table_search_step_" + to_string(target) + ".txt";
    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create " << outputFilename << "\n";
        return 1;
    }

    int index = hashFunction(target);
    bool found = false;

    for (size_t i = 0; i < hashTable[index].size(); i++) {
        const Record& rec = hashTable[index][i];
        if (rec.number == target) {
            outFile << rec.number << " = " << target << "/" << rec.text << "\n";
            cout    << rec.number << " = " << target << "/" << rec.text << "\n";
            found = true;
            break;
        } else {
            outFile << rec.number << " != " << target << "\n";
            cout    << rec.number << " != " << target << "\n";
        }
    }

    if (!found) {
        outFile << -1 << " != " << target << "\n";
        cout    << -1 << " != " << target << "\n";
    }

    outFile.close();

    cout << "\nTrace complete! Bucket index = " << index
         << " (table size = " << primeDivisor << ").\n"
         << "Results written to " << outputFilename << "\n";

    return 0;
}