// *********************************************************
// Program: heap_sort.cpp
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
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

struct Record {
    long long number;
    string text;
};

// Heapify down to maintain max-heap property
// n = current heap size, i = index to heapify from
void heapifyDown(vector<Record>& arr, int n, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    if (left  < n && arr[left].number  > arr[largest].number) largest = left;
    if (right < n && arr[right].number > arr[largest].number) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapifyDown(arr, n, largest);
    }
}

int main() {
    // ==========================================
    // 1. CONFIGURATION
    // ==========================================
    string inputSize = "1000";
    string inputFilename  = "dataset_"      + inputSize + ".csv";
    string outputFilename = "heap_sort_dataset_" + inputSize + ".csv";

    // ==========================================
    // 2. READ THE DATASET (NOT TIMED)
    // ==========================================
    cout << "Reading data from " << inputFilename << "...\n";

    ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open " << inputFilename << "\n";
        return 1;
    }

    vector<Record> dataset;
    string line;

    // Read every single row in the file
    while (getline(inFile, line)) {
        stringstream ss(line);
        string numStr, textStr;
        if (getline(ss, numStr, ',') && getline(ss, textStr)) {
            Record rec;
            rec.number = stoll(numStr);
            rec.text   = textStr;
            dataset.push_back(rec);
        }
    }
    inFile.close();

    int n = dataset.size();
    if (n == 0) {
        cerr << "Error: No data loaded.\n";
        return 1;
    }

    // ==========================================
    // 3. HEAP SORT LOGIC (TIMED)
    // ==========================================
    cout << "Starting Heap Sort on " << n << " elements...\n";

    auto start_time = chrono::high_resolution_clock::now();

    // Step 1: Build max-heap bottom-up
    // Start from the last non-leaf node and heapify down to root
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDown(dataset, n, i);
    }

    // Step 2: Extract max one by one
    // Swap root (largest) with last element, shrink heap, heapify again
    for (int i = n - 1; i >= 1; i--) {
        swap(dataset[0], dataset[i]);
        heapifyDown(dataset, i, 0);
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_taken = end_time - start_time;

    // ==========================================
    // 4. WRITE THE SORTED DATASET (NOT TIMED)
    // ==========================================
    cout << "Writing sorted data to " << outputFilename << "...\n";

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create output file.\n";
        return 1;
    }

    for (int i = 0; i < n; i++) {
        outFile << dataset[i].number << "/" << dataset[i].text << "\n";
    }
    outFile.close();

    // ==========================================
    // 5. PRINT RESULTS FOR DOCUMENTATION
    // ==========================================
    cout << "--------------------------------------\n";
    cout << "Algorithm Runtime: " << time_taken.count() << " seconds\n";
    cout << "--------------------------------------\n";

    return 0;
}