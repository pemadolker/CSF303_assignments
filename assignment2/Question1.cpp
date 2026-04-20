#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cout << "Enter N (number of integers): " << flush;
    cin >> N;

    vector<int> arr(N);
    cout << "Enter " << N << " integers: " << flush;
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    int K;
    cout << "Enter K (divisor): " << flush;
    cin >> K;

    int totalSubsets = (1 << N);
    int count = 0;

    cout << "\n--- All Subsets and Their Sums ---\n";

    for (int mask = 0; mask < totalSubsets; mask++) {
        vector<int> subset;
        int sum = 0;

        for (int i = 0; i < N; i++) {
            if (mask & (1 << i)) {
                subset.push_back(arr[i]);
                sum += arr[i];
            }
        }

        cout << "{ ";
        if (subset.empty()) {
            cout << "empty";
        } else {
            for (int i = 0; i < (int)subset.size(); i++) {
                cout << subset[i];
                if (i + 1 < (int)subset.size()) cout << ", ";
            }
        }
        cout << " }  ->  sum = " << sum;

        if (sum % K == 0) {
            cout << "  [divisible by " << K << "]";
            count++;
        }
        cout << "\n";
    }

    cout << "\n--- Result ---\n";
    cout << "Total subsets: " << totalSubsets << "\n";
    cout << "Subsets with sum divisible by " << K << ": " << count << "\n";

    return 0;
}