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

        for (int i = 0; i < n; i++) {
            int digit = (dataset[i].number / divisor) % 10;
            count[digit]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            int digit = (dataset[i].number / divisor) % 10;
            output[count[digit] - 1] = dataset[i];
            count[digit]--;
        }

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