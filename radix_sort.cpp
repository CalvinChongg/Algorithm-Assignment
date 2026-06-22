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
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;

struct Record {
    long long number;
    string text;
};

int main() {
    // ==========================================
    // 1. CONFIGURATION
    // Change this to test your different sizes (1000, 1000000, etc.)
    // ==========================================
    string inputSize = "1000"; 
    string inputFilename = "dataset_" + inputSize + ".csv";
    string outputFilename = "radix_sorted_dataset_" + inputSize + ".csv";

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
            rec.text = textStr;
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
    // 3. RADIX SORT LOGIC (STRICTLY TIMED)
    // ==========================================
    cout << "Starting Radix Sort on " << n << " elements...\n";

    auto start_time = chrono::high_resolution_clock::now();

    // The identical sorting logic from the step version, just without printing
    long long divisor = 1;
    vector<Record> output(n);

    for (int pass = 1; pass <= 10; pass++) {
        int count[10] = {0};

        // put each digits into their own array to know how many of the same digit there is
        for (int i = 0; i < n; i++) {
            int digit = (dataset[i].number / divisor) % 10;
            count[digit]++;
        }

        // cummulative sum
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // step 1 : get last digit of numbers being read
        // step 2 : "count[digit]" to get the cum num of that digit, then we -1 for human reading
        // we then assign the array "output" of THAT slot with numbers being read "dataset[i]"
        // step 3 : then decrease the count of that EXACT digit
        
        //example we already give them range like digit 0 has slot 0 to 4 
        //then digit 1 has slot 5 to 6, even if count[0]-- it will not affect count[1]

        for (int i = n - 1; i >= 0; i--) {
            int digit = (dataset[i].number / divisor) % 10;
            output[count[digit] - 1] = dataset[i];
            count[digit]--;
        }

        // replace each lines with new sorted lines
        // repeat with next divisor
        for (int i = 0; i < n; i++) {
            dataset[i] = output[i];
        }
        divisor *= 10;
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