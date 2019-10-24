#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

int **readmatrices(int s1[2], int s2[2]);

vector<string> splitInput(string str);


int main() {
    cout << "Hello, World!" << endl;
    int m1size[2], m2size[2];
    auto matrices = readmatrices(m1size, m2size);
    return 0;
}

int **readmatrices(int s1[2], int s2[2]) {

    ifstream myfile("matrices.txt");
    string line;
    vector<string> args;

    if (myfile.is_open()) {

        getline(myfile, line);
        args = splitInput(line);

        //matrix 1 size
        s1[0] = stoi(args[0]);
        s1[1] = stoi(args[1]);

        //create matrix 1
        int matrix1[s1[0]][s1[1]];
        for (int i = 0; i < s1[0]; ++i) {

            getline(myfile, line);
            args = splitInput(line);

            for (int j = 0; j < s1[1]; ++j)
                matrix1[i][j] = stoi(args[j]);

        }

        //matrix 2 size
        s2[0] = stoi(args[0]);
        s2[1] = stoi(args[1]);

        //create matrix 1
        int matrix2[s2[0]][s2[1]];
        for (int i = 0; i < s2[0]; ++i) {

            getline(myfile, line);
            args = splitInput(line);

            for (int j = 0; j < s2[1]; ++j)
                matrix2[i][j] = stoi(args[j]);

        }

        myfile.close();


    }
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
