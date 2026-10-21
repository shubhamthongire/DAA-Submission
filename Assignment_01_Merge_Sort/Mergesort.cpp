#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstdio>

using namespace std;

class TransactionRow {
public:
    string text;
    long long dateValue;

    TransactionRow(const string& row, long long date) : text(row), dateValue(date) {}
};

void merge(vector<TransactionRow>& arr, int si, int mid, int ei) {
    vector<TransactionRow> temp;
    temp.reserve(ei - si + 1);

    int i = si;
    int j = mid + 1;

    while (i <= mid && j <= ei) {
        if (arr[i].dateValue <= arr[j].dateValue) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }

    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= ei) temp.push_back(arr[j++]);

    for (int idx = si, x = 0; idx <= ei; ++idx, ++x) {
        arr[idx] = temp[x];
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
    // Keep Retail_Transaction_Dataset.csv in the same folder as this program.
    ifstream file("Retail_Transaction_Dataset.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open Retail_Transaction_Dataset.csv.\n"
             << "Place the CSV file in the same folder as this program.\n";
        return 1;
    }

    vector<TransactionRow> dataset;
    string line;
    string header;

    if (!getline(file, header)) {
        cerr << "Error: The CSV file is empty.\n";
        return 1;
    }

    while (getline(file, line)) {
        // If a quoted CSV field spans multiple lines, read until its quotes close.
        while (count(line.begin(), line.end(), '"') % 2 != 0) {
            string extra;
            if (!getline(file, extra)) break;
            line += "\n" + extra;
        }

        if (count(line.begin(), line.end(), '"') % 2 != 0) {
            cerr << "Warning: Skipping a row with unmatched quotation marks.\n";
            continue;
        }

        // TransactionDate is the fifth CSV field, after four commas.
        size_t pos = 0;
        bool foundDateField = true;

        for (int i = 0; i < 4; ++i) {
            size_t comma = line.find(',', pos);
            if (comma == string::npos) {
                foundDateField = false;
                break;
            }
            pos = comma + 1;
        }

        if (!foundDateField || pos >= line.size()) {
            cerr << "Warning: Skipping a malformed CSV row.\n";
            continue;
        }

        // Accept DD-MM-YYYY HH:MM (the format shown in the dataset),
        // and also MM/DD/YYYY HH:MM if the dataset uses slashes.
        int first = 0, second = 0, year = 0, hour = 0, minute = 0;
        char sep1 = '\0', sep2 = '\0';

        int parsed = sscanf(line.c_str() + pos, "%d%c%d%c%d %d:%d",
                            &first, &sep1, &second, &sep2, &year, &hour, &minute);

        if (parsed != 7 || sep1 != sep2) {
            cerr << "Warning: Skipping a row with an unrecognized date: "
                 << line.substr(pos, line.find(',', pos) - pos) << "\n";
            continue;
        }

        int day = 0;
        int month = 0;

        if (sep1 == '-') {
            // Dates with hyphens are interpreted as DD-MM-YYYY.
            day = first;
            month = second;
        } else if (sep1 == '/') {
            // Dates with slashes are interpreted as MM/DD/YYYY.
            month = first;
            day = second;
        } else {
            cerr << "Warning: Skipping a row with an unsupported date separator.\n";
            continue;
        }

        if (year < 1 || month < 1 || month > 12 ||
            day < 1 || day > 31 || hour < 0 || hour > 23 ||
            minute < 0 || minute > 59) {
            cerr << "Warning: Skipping a row with an invalid date/time.\n";
            continue;
        }

        // Build a sortable YYYYMMDDHHMM numeric key.
        long long dateValue = static_cast<long long>(year) * 100000000LL
                            + static_cast<long long>(month) * 1000000LL
                            + static_cast<long long>(day) * 10000LL
                            + static_cast<long long>(hour) * 100LL
                            + minute;

        dataset.emplace_back(line, dateValue);
    }

    file.close();

    if (dataset.empty()) {
        cerr << "Error: No valid transaction rows were read. Check the CSV date format.\n";
        return 1;
    }

    mergeSort(dataset, 0, static_cast<int>(dataset.size()) - 1);

    cout << "Sorting complete! Total valid transactions: " << dataset.size() << "\n";
    cout << "Here are the earliest 2 transactions:\n\n";

    for (size_t i = 0; i < dataset.size() && i < 2; ++i) {
        cout << dataset[i].text << "\n\n";
    }

    return 0;
}
