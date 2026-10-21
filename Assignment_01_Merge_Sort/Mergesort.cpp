#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> // For std::count

using namespace std;

// 1. Using a 'class' instead of a 'struct'
class TransactionRow {
public: // We must specify public so the merge sort can access these variables
    string text;
    long long dateValue; 
    
    // Constructor to easily create a new row object
    TransactionRow(string t, long long d) {
        text = t;
        dateValue = d;
    }
};

// 2. Your EXACT merge sort logic, adapted for the class
void merge(vector<TransactionRow>& arr, int si, int mid, int ei) {
    vector<TransactionRow> temp;
    int i = si, j = mid + 1;
    
    while (i <= mid && j <= ei) {
        // Compare the numeric date values directly
        if (arr[i].dateValue < arr[j].dateValue) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }
    
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= ei) temp.push_back(arr[j++]);
    
    for (int idx = si, x = 0; idx <= ei; idx++) {
        arr[idx] = temp[x++];
    }
}

void mergeSort(vector<TransactionRow>& arr, int si, int ei) {
    if (si >= ei) return;
    int mid = si + (ei - si) / 2;
    mergeSort(arr, si, mid);
    mergeSort(arr, mid + 1, ei);
    merge(arr, si, mid, ei);
}

int main() {
    ifstream file("Retail_Transaction_Dataset.csv");
    vector<TransactionRow> dataset;
    string line, header;
    
    // Check if the file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }
    
    getline(file, header); // Skip the header row    
    // 3. Loop to read and parse the CSV
    while (getline(file, line)) {
        
        // Handle rows that break onto a new line because of quotes
        while (count(line.begin(), line.end(), '"') % 2 != 0) {
            string extra;
            getline(file, extra);
            line += "\n" + extra;
        }
        
        // Skip exactly 4 commas to find the start of the TransactionDate
        int pos = 0;
        for (int i = 0; i < 4; i++) {
            pos = line.find(',', pos) + 1;
        }
        
        // Quickly extract Month, Day, Year, Hour, and Minute
        int m, d, y, h, min;
        sscanf(line.c_str() + pos, "%d/%d/%d %d:%d", &m, &d, &y, &h, &min);
        
        // The Math Trick: Turn the date into a single large number
        long long dateVal = y * 100000000LL + m * 1000000LL + d * 10000LL + h * 100LL + min;
        
        // Use the class constructor to add the data to our vector
        dataset.push_back(TransactionRow(line, dateVal));
    }

    // 4. Sort the dataset
    if (dataset.size() > 0) {
        mergeSort(dataset, 0, dataset.size() - 1);
    }

    // Print the first two sorted rows to verify
    cout << "Sorting complete! Here are the earliest 2 transactions:\n\n";
    if (dataset.size() >= 2) {
        cout << dataset[0].text << "\n\n" << dataset[1].text << endl;
    }

    return 0;
}
