// *********************************************************
// Program: radix_sort_step.cpp
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

using namespace std;

// Structure to hold the data row
struct Record {
    long long number;
    string text;
};

// Helper function to print the current state of the array exactly as the sample requires
void printState(ofstream& outFile, const vector<Record>& arr, string stepLabel) {
    outFile << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        outFile << arr[i].number << "/" << arr[i].text;
        if (i < arr.size() - 1) {
            outFile << ", ";
        }
    }
    outFile << "] " << stepLabel << "\n";
}

int main() {

    string inputFilename = "dataset_1000.csv"; 
    int startRow = 1; // 1-indexed row number
    int endRow = 7;   // 1-indexed row number

    ifstream inFile(inputFilename);

    vector<Record> dataset;
    string line;
    int currentRow = 1;

    // Read only the rows from startRow to endRow
    while (getline(inFile, line) && currentRow <= endRow) {
        if (currentRow >= startRow) {
            stringstream ss(line);
            string numStr, textStr;
            
            // Split by comma
            if (getline(ss, numStr, ',') && getline(ss, textStr)) {
                Record rec;
                rec.number = stoll(numStr);
                rec.text = textStr;
                dataset.push_back(rec);
            }
        }
        currentRow++;
    }
    inFile.close();

    if (dataset.empty()) {
        cerr << "Error: No data loaded. Check row indices.\n";
        return 1;
    }

    // ==========================================
    // 3. PREPARE OUTPUT FILE
    // ==========================================
    // Output filename format: dataset_1000_radix_sorted_step_1_7.txt
    string baseName = inputFilename.substr(0, inputFilename.find(".csv"));
    string outputFilename = baseName + "_radix_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
    
    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create output file.\n";
        return 1;
    }

    outFile << "(processing from the rightmost character)\n";
    printState(outFile, dataset, "original");

    // ==========================================
    // 4. RADIX SORT LOGIC (LSD)
    // ==========================================
    long long divisor = 1;
    int n = dataset.size();
    vector<Record> output(n);

    // 10 digits in our integers, so 10 passes
    for (int pass = 1; pass <= 10; pass++) {
        int count[10] = {0};

        // Store count of occurrences for the current digit
        for (int i = 0; i < n; i++) {
            int digit = (dataset[i].number / divisor) % 10;
            count[digit]++;
        }

        // Change count[i] so it contains the actual position of this digit in output[]
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Build the output array (working backwards maintains stability)
        for (int i = n - 1; i >= 0; i--) {
            int digit = (dataset[i].number / divisor) % 10;
            output[count[digit] - 1] = dataset[i];
            count[digit]--;
        }

        // Copy the sorted elements back to original array
        for (int i = 0; i < n; i++) {
            dataset[i] = output[i];
        }

        // The document labels step 1 as $d=10$, step 2 as $d=9$, down to $d=1$
        int d_label = 11 - pass; 
        printState(outFile, dataset, "$d=" + to_string(d_label) + "$");

        // Move to the next digit to the left
        divisor *= 10;
    }

    outFile.close();
    cout << "Step-by-step sorting complete! Saved to " << outputFilename << "\n";

    return 0;
}