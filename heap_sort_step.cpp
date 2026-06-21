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
    string inputFilename = "dataset_10000.csv";
    int startRow = 1;
    int endRow   = 10; 

    // ==========================================
    // 2. READ THE DATASET
    // ==========================================
    ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open " << inputFilename << "\n";
        return 1;
    }

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
                rec.text   = textStr;
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
    // PREPARE OUTPUT FILE
    // ==========================================
    // Output filename format: dataset_1000_heap_sorted_step_1_7.txt
    string baseName      = inputFilename.substr(0, inputFilename.find(".csv"));
    string outputFilename = baseName + "_heap_sorted_step_"
                          + to_string(startRow) + "_" + to_string(endRow) + ".txt";

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot create output file.\n";
        return 1;
    }

    // ==========================================
    // HEAP SORT LOGIC (using max-heap)
    // ==========================================
    int n = dataset.size();

    // Step 1: Build max-heap bottom-up (no output during this phase)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDown(dataset, n, i);
    }

    // Print the initial max-heap state (before any extraction)
    printState(outFile, dataset, "initial");

    // Step 2: Extract max one by one and print after each extraction
    for (int i = n - 1; i >= 1; i--) {
        // Move current root (largest) to the end
        swap(dataset[0], dataset[i]);
        // Heapify the reduced heap
        heapifyDown(dataset, i, 0);
        // Print current state, labelled i = X (matching the assignment sample)
        printState(outFile, dataset, "i = " + to_string(i));
    }

    outFile.close();
    cout << "Step-by-step sorting complete! Saved to " << outputFilename << "\n";

    return 0;
}