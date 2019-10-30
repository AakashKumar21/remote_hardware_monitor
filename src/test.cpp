#include <bits/stdc++.h>
using namespace std;

int countWords(string str)
{
    // breaking input into word using string stream
    stringstream s(str); // Used for breaking words
    int x;               // to store individual words

    int count = 0;
    while (s >> x)
    {
        count++;
        cout<< x;
    }
    return count;
}

// Driver code
int main()
{
    string s = "geeks for 34453 343";
    cout << " Number of words are: " << countWords(s);
    return 0;
}