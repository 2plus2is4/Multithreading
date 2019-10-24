#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

int **readmatrices(int pInt[2], int pInt1[2]);

vector<string> splitInput(string str);


int main() {
    cout << "Hello, World!" << endl;
    ifstream myfile("matrices.txt");
    int m1size[2], m2size[2];
    auto matrices = readmatrices(m1size, m2size);
    return 0;
}

int **readmatrices(int pInt[2], int pInt1[2]) {
    return nullptr;
}

vector<string> splitInput(string str) {

    vector<string> args;
    string word = "";
    for (auto x : str) {
        if (x == ' ') {
            args.push_back(word);
            word = "";
        } else {
            word = word + x;
        }
    }
    args.push_back(word);
    return args;
}
