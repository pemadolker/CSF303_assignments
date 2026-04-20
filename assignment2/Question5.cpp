#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> computeLPS(const string& pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);

    int len = 0; // length of previous longest prefix suffix
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Fall back using LPS — key KMP insight
                len = lps[len - 1];
                // Do NOT increment i here
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int main() {
    string pattern = "ABABCABAB";
    vector<int> lps = computeLPS(pattern);

    cout << "Pattern : ";
    for (char c : pattern) cout << c << " ";
    cout << "\nIndex   : ";
    for (int i = 0; i < (int)pattern.size(); i++) cout << i << " ";
    cout << "\nLPS     : ";
    for (int val : lps) cout << val << " ";
    cout << "\n";

    return 0;
}