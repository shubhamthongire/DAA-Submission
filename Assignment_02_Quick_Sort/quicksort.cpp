#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Structure to represent each movie record
struct Movie {
    string title;
    float rating;
    int release_year;
    int popularity;

    void display() const {
        cout << title << " | Rating: " << rating
             << " | Year: " << release_year
             << " | Popularity: " << popularity << endl;
    }
};


// Comparator Functions (Ascending Order)
bool compareByRating(const Movie &a, const Movie &b) {
    return a.rating <= b.rating;
}

bool compareByYear(const Movie &a, const Movie &b) {
    return a.release_year <= b.release_year;
}

bool compareByPopularity(const Movie &a, const Movie &b) {
    return a.popularity <= b.popularity;
}

// QuickSort Implementation (Using your partition logic)

int partition(vector<Movie> &arr, int si, int ei, bool (*compare)(const Movie &, const Movie &)) {
    int i = si - 1;
    Movie pivot = arr[ei]; // Taking last element as pivot

    for (int j = si; j < ei; j++) {
        if (compare(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    i++;
    swap(arr[i], arr[ei]);
    return i; // Returns the pivot index
}

void quickSort(vector<Movie> &arr, int si, int ei, bool (*compare)(const Movie &, const Movie &)) {
    if (si >= ei) {
        return;
    }

    int pivotIdx = partition(arr, si, ei, compare);

    quickSort(arr, si, pivotIdx - 1, compare); // Left half
    quickSort(arr, pivotIdx + 1, ei, compare);  // Right half
}


// Data Loaders: CSV Parser & Synthetic Generator

vector<Movie> loadMoviesFromCSV(const string &filename) {
    vector<Movie> movies;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filename << "'\n";
        return movies;
    }

    string line;
    getline(file, line); // Skip CSV header row

    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string title, ratingStr, yearStr, popStr;

        getline(ss, title, ',');
        getline(ss, ratingStr, ',');
        getline(ss, yearStr, ',');
        getline(ss, popStr, ',');

        try {
            Movie m;
            m.title = title;
            m.rating = stof(ratingStr);
            m.release_year = stoi(yearStr);
            m.popularity = stoi(popStr);
            movies.push_back(m);
        } catch (...) {
            // Skips malformed rows if any
            continue;
        }
    }
    return movies;
}

vector<Movie> generateMovies(int n) {
    vector<Movie> movies;
    movies.reserve(n);
    for (int i = 0; i < n; i++) {
        Movie m;
        m.title = "Movie_" + to_string(i + 1);
        m.rating = static_cast<float>(rand() % 90 + 10) / 10.0f; // 1.0 to 10.0
        m.release_year = rand() % 45 + 1980;                   // 1980 to 2024
        m.popularity = rand() % 1000000 + 1000;                // 1,000 to 1,000,000
        movies.push_back(m);
    }
    return movies;
}


// Main Function
int main() {
    srand(time(0));
    vector<Movie> movies;

    cout << "==========================================\n";
    cout << "        Movie QuickSort Benchmark         \n";
    cout << "==========================================\n";
    cout << "Select Data Source:\n";
    cout << "  1. Load from CSV file\n";
    cout << "  2. Generate Random Dataset\n";
    cout << "Enter choice (1/2): ";
    
    int sourceChoice;
    cin >> sourceChoice;

    if (sourceChoice == 1) {
        cout << "Enter CSV filename (e.g., movies.csv or movies_real_titles.csv): ";
        string filename;
        cin >> filename;
        movies = loadMoviesFromCSV(filename);
    } else {
        int n;
        cout << "Enter number of movies to generate: ";
        cin >> n;
        movies = generateMovies(n);
    }

    if (movies.empty()) {
        cout << "No data available to sort. Exiting.\n";
        return 1;
    }

    cout << "\nSort by (rating / year / popularity): ";
    string criteria;
    cin >> criteria;

    bool (*compare)(const Movie &, const Movie &) = nullptr;

    if (criteria == "rating") {
        compare = compareByRating;
    } else if (criteria == "year") {
        compare = compareByYear;
    } else if (criteria == "popularity") {
        compare = compareByPopularity;
    } else {
        cout << "Invalid choice. Defaulting to 'rating'.\n";
        compare = compareByRating;
        criteria = "rating";
    }

    // Benchmark QuickSort
    auto start = high_resolution_clock::now();
    quickSort(movies, 0, static_cast<int>(movies.size()) - 1, compare);
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    // Display Results
    int total = movies.size();
    int topK = min(10, total);

    cout << "\n--- Top " << topK << " Movies (Highest " << criteria << ") ---\n";
    for (int i = total - 1; i >= total - topK; --i) {
        movies[i].display();
    }

    cout << "\nSorted " << total << " records in " << diff.count() << " seconds.\n";

    return 0;
}