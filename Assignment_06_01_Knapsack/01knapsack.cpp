
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 1. Recursive Approach
// Time Complexity: O(2^N)
int knapsackRec(vector<int>& wt, vector<int>& val,
                int W, int n) {
    if (n == 0 || W == 0)
        return 0;

    if (wt[n - 1] <= W) {
        int include = val[n - 1] +
            knapsackRec(wt, val, W - wt[n - 1], n - 1);

        int exclude = knapsackRec(wt, val, W, n - 1);

        return max(include, exclude);
    }

    return knapsackRec(wt, val, W, n - 1);
}

// 2. Memoization Approach
// Time Complexity: O(N * W)
// Space Complexity: O(N * W)
int knapsackMem(vector<int>& wt, vector<int>& val,
                int W, int n, vector<vector<int>>& dp) {
    if (n == 0 || W == 0)
        return 0;

    if (dp[n][W] != -1)
        return dp[n][W];

    if (wt[n - 1] <= W) {
        int include = val[n - 1] +
            knapsackMem(wt, val, W - wt[n - 1],
                        n - 1, dp);

        int exclude = knapsackMem(wt, val, W, n - 1, dp);

        dp[n][W] = max(include, exclude);
    } else {
        dp[n][W] = knapsackMem(wt, val, W, n - 1, dp);
    }

    return dp[n][W];
}

// 3. Tabulation Approach
// Time Complexity: O(N * W)
// Space Complexity: O(N * W)
int knapsackTab(vector<int>& wt, vector<int>& val,
                int W, int n, vector<string>& names) {
    vector<vector<int>> dp(
        n + 1, vector<int>(W + 1, 0)
    );

    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            // Exclude current item
            dp[i][w] = dp[i - 1][w];

            // Include current item if it fits
            if (wt[i - 1] <= w) {
                int include = val[i - 1]
                    + dp[i - 1][w - wt[i - 1]];

                dp[i][w] = max(dp[i][w], include);
            }
        }
    }

    // Display DP table
    cout << "\nDynamic Programming Table:\n";
    cout << "Items\\Capacity\t";
    for (int w = 0; w <= W; w++)
        cout << w << "\t";
    cout << '\n';

    for (int i = 0; i <= n; i++) {
        cout << i << "\t\t";
        for (int w = 0; w <= W; w++)
            cout << dp[i][w] << "\t";
        cout << '\n';
    }

    // Trace back to find selected items
    vector<int> selected;
    int remaining = W;

    for (int i = n; i > 0; i--) {
        if (dp[i][remaining] != dp[i - 1][remaining]) {
            selected.push_back(i - 1);
            remaining -= wt[i - 1];
        }
    }

    reverse(selected.begin(), selected.end());

    // Display selected relief items
    cout << "\nSelected Relief Items:\n";
    int totalWeight = 0;

    if (selected.empty())
        cout << "No items selected.\n";

    for (int index : selected) {
        cout << names[index]
             << " | Weight: " << wt[index]
             << " kg | Utility: " << val[index] << '\n';

        totalWeight += wt[index];
    }

    cout << "\nMaximum Total Utility: " << dp[n][W] << '\n';
    cout << "Total Weight: " << totalWeight << " kg\n";
    cout << "Remaining Capacity: "
         << W - totalWeight << " kg\n";

    return dp[n][W];
}

int main() {
    int n, W;

    cout << "========================================\n";
    cout << " DISASTER RELIEF RESOURCE ALLOCATION\n";
    cout << "========================================\n";

    cout << "Enter number of relief items: ";
    cin >> n;

    if (n <= 0) {
        cout << "Error: Number of items must be positive.\n";
        return 1;
    }

    vector<string> names(n);
    vector<int> wt(n), val(n);

    cout << "\nEnter item name, weight (kg), and utility.\n";
    cout << "Use names without spaces, e.g., FirstAidKit.\n";

    for (int i = 0; i < n; i++) {
        cout << "\nItem " << i + 1 << " name: ";
        cin >> names[i];

        cout << "Weight (kg): ";
        cin >> wt[i];

        cout << "Utility value: ";
        cin >> val[i];

        if (wt[i] <= 0 || val[i] < 0) {
            cout << "Error: Weight must be positive and "
                 << "utility must be non-negative.\n";
            return 1;
        }
    }

    cout << "\nEnter truck capacity (kg): ";
    cin >> W;

    if (W < 0) {
        cout << "Error: Capacity cannot be negative.\n";
        return 1;
    }

    // Recursive result
    cout << "\n--- Recursive Approach ---\n";
    cout << "Maximum Utility: "
         << knapsackRec(wt, val, W, n) << '\n';

    // Memoization result
    vector<vector<int>> memo(
        n + 1, vector<int>(W + 1, -1)
    );

    cout << "\n--- Memoization Approach ---\n";
    cout << "Maximum Utility: "
         << knapsackMem(wt, val, W, n, memo) << '\n';

    // Tabulation result and selected items
    cout << "\n--- Tabulation Approach ---\n";
    int result = knapsackTab(wt, val, W, n, names);

    cout << "\n========== COMPLEXITY ANALYSIS ==========\n";
    cout << "Recursive Time: O(2^N)\n";
    cout << "Memoization Time: O(N * W)\n";
    cout << "Tabulation Time: O(N * W)\n";
    cout << "DP Space Complexity: O(N * W)\n";

    cout << "\nFinal Optimal Utility: " << result << '\n';

    return 0;
}