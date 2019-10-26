#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <time.h>

using namespace std;

struct mythreadsingle {
    int x;
    int y;
    int m;
    int n;
    int l;
    int *row;
    int *column;
    int **ans;
};

struct mythreadrow {
    int index;
    int l;
    int n;
    int *row;
    int **matrix;
    int **ans;
};

struct mythreadsort {
    int l;
    int r;
    int *arr;
};

int **readarray();

void *mergesort(void *threadarg);

void merge_sort(int *arr, int l, int r);

int ***readmatrices(int s1[2], int s2[2]);

void *multiplysingle(void *threadarg);

void *multiplyrow(void *threadarg);

vector<string> splitInput(string str);

void combine(int *arr, int l, int m, int r);


int main() {

    int m1size[2], m2size[2];
    auto matrices = readmatrices(m1size, m2size);
    int m = m1size[0], n = m2size[1], l = m1size[1];

    int **ans = new int *[m];
    for (int k = 0; k < m; ++k) {
        ans[k] = new int[n];
    }


    clock_t clc;
    int mode = 3;
    for (; mode < 4; ++mode)
        if (mode == 1) {
            cout << "mode 1: " << endl;
            clc = clock();
            pthread_t threads[m * n];
            pthread_attr_t attr;
            void *status;
            struct mythreadsingle t[m * n];
            int rc;

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            for (int i = 0; i < m * n; i++) {

                int column = i % n, row = i / n;

                t[i].x = row;
                t[i].y = column;
                t[i].m = m;
                t[i].n = n;
                t[i].row = matrices[0][row];
                t[i].column = new int[n];
                t[i].ans = ans;
                t[i].l = l;
                for (int j = 0; j < l; ++j) {
                    t[i].column[j] = matrices[1][j][column];
                }

                rc = pthread_create(&threads[i], &attr, multiplysingle,
                                    (void *) &t[i]);
                if (rc) {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }

            }

            pthread_attr_destroy(&attr);
            for (int i = 0; i < n * m; i++) {
                rc = pthread_join(threads[i], &status);
                if (rc) {
                    cout << "Error:unable to join," << rc << endl;
                    exit(-1);
                }
            }

            for (int v = 0; v < m; ++v) {
                for (int vv = 0; vv < n; ++vv) {
                    cout << ans[v][vv] << " ";
                }
                cout << endl;
            }
            clc = clock() - clc;
            cout << "total time: " << (float) (clc) / CLOCKS_PER_SEC << " secs"
                 << endl;

        } else if (mode == 2) {
            cout << "mode 2: " << endl;
            clc = clock();
            pthread_t threads[m];
            pthread_attr_t attr;
            void *status;
            struct mythreadrow t[m];
            int rc;

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            for (int i = 0; i < m; i++) {

                t[i].index = i;
                t[i].row = matrices[0][i];
                t[i].matrix = matrices[1];
                t[i].ans = ans;
                t[i].l = l;
                t[i].n = n;

                rc = pthread_create(&threads[i], &attr, multiplyrow,
                                    (void *) &t[i]);
                if (rc) {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }

            }

            pthread_attr_destroy(&attr);
            for (int i = 0; i < m; i++) {
                rc = pthread_join(threads[i], &status);

                if (rc) {
                    cout << "Error:unable to join," << rc << endl;
                    exit(-1);
                }
            }

            for (int v = 0; v < m; ++v) {
                for (int vv = 0; vv < n; ++vv) {
                    cout << ans[v][vv] << " ";
                }
                cout << endl;
            }
            clc = clock() - clc;
            cout << "total time: " << (float) (clc) / CLOCKS_PER_SEC << " secs"
                 << endl;
        } else if (mode == 3) {
            cout << "=========================================" << endl;
            int **arrsize = readarray();
            merge_sort(arrsize[1], 0, arrsize[0][0]-1);
            for (int i = 0; i < arrsize[0][0]; ++i) {
                cout << arrsize[1][i] << " ";
            }
        }

    return 0;
}


void *multiplysingle(void *threadarg) {
    struct mythreadsingle *t;
    t = (struct mythreadsingle *) threadarg;
    int **ans = t->ans;
    int n = t->n, m = t->m;
    int *c = t->column, *r = t->row;

    int sum = 0;

    for (int i = 0; i < t->l; ++i) {
        sum += c[i] * r[i];
    }

    ans[t->x][t->y] = sum;

    pthread_exit(NULL);
}

void *multiplyrow(void *threadarg) {
    struct mythreadrow *t;
    t = (struct mythreadrow *) threadarg;

    for (int i = 0; i < t->n; ++i) {
        int sum = 0;
        for (int ii = 0; ii < t->l; ++ii) {
            sum += t->row[ii] * t->matrix[ii][i];
        }
        t->ans[t->index][i] = sum;
    }

    pthread_exit(NULL);
}

void *mergesort(void *threadarg) {

    struct mythreadsort *td;
    td = (struct mythreadsort *) threadarg;


    if (td->l >= td->r)
        pthread_exit(NULL);
    int m = td->l + (td->r - td->l) / 2;

    pthread_t threads[2];
    pthread_attr_t attr;
    void *status;
    struct mythreadsort t[2];

    t[0].arr = td->arr;
    t[0].l = td->l;
    t[0].r = m;
    t[1].arr = td->arr;
    t[1].l = m + 1;
    t[1].r = td->r;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc = pthread_create(&threads[0], &attr, mergesort,
                            (void *) &t[0]);
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    rc = pthread_create(&threads[1], &attr, mergesort,
                        (void *) &t[1]);
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }


    pthread_attr_destroy(&attr);
    rc = pthread_join(threads[0], &status);
    if (rc) {
        cout << "Error:unable to join," << rc << endl;
        exit(-1);
    }
    rc = pthread_join(threads[1], &status);
    if (rc) {
        cout << "Error:unable to join," << rc << endl;
        exit(-1);
    }


    combine(td->arr, td->l, m, td->r);
}

void merge_sort(int *arr, int l, int r) {
    if (l >= r)
        return;
    int m = l + (r - l) / 2;

    pthread_t threads[2];
    pthread_attr_t attr;
    void *status;
    struct mythreadsort t[2];

    t[0].arr = arr;
    t[0].l = l;
    t[0].r = m;
    t[1].arr = arr;
    t[1].l = m + 1;
    t[1].r = r;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc = pthread_create(&threads[0], &attr, mergesort,
                            (void *) &t[0]);
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    rc = pthread_create(&threads[1], &attr, mergesort,
                        (void *) &t[1]);
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    pthread_attr_destroy(&attr);
    rc = pthread_join(threads[0], &status);
    if (rc) {
        cout << "Error:unable to join," << rc << endl;
        exit(-1);
    }
    rc = pthread_join(threads[1], &status);
    if (rc) {
        cout << "Error:unable to join," << rc << endl;
        exit(-1);
    }

    combine(arr, l, m, r);
}

void combine(int *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

int **readarray() {
    ifstream myfile("array.txt");
    string line;
    vector<string> args;
    int *ans, size;
    if (myfile.is_open()) {

        getline(myfile, line);
        args = splitInput(line);

        size = stoi(args[0]);
        ans = new int[size];

        getline(myfile, line);
        args = splitInput(line);
        for (int i = 0; i < size; ++i) {
            ans[i] = stoi(args[i]);
        }

        myfile.close();

    }

    int **arrsize = new int *[2];
    arrsize[0] = new int[1];
    arrsize[0][0] = size;
    arrsize[1] = ans;

    return arrsize;
}


int ***readmatrices(int s1[2], int s2[2]) {

    ifstream myfile("matrices.txt");
    string line;
    vector<string> args;
    int ***ans = new int **[2];
    if (myfile.is_open()) {

        getline(myfile, line);
        args = splitInput(line);

        //matrix 1 size
        s1[0] = stoi(args[0]);
        s1[1] = stoi(args[1]);

        //create matrix 1

        int **matrix1 = new int *[s1[0]];
        for (int k = 0; k < s1[0]; ++k) {
            matrix1[k] = new int[s1[1]];
        }
        for (int i = 0; i < s1[0]; ++i) {

            getline(myfile, line);
            args = splitInput(line);

            for (int j = 0; j < s1[1]; ++j)
                matrix1[i][j] = stoi(args[j]);

        }

        getline(myfile, line);
        args = splitInput(line);

        //matrix 2 size
        s2[0] = stoi(args[0]);
        s2[1] = stoi(args[1]);

        //create matrix 1

        int **matrix2 = new int *[s2[0]];
        for (int k = 0; k < s2[0]; ++k) {
            matrix2[k] = new int[s2[1]];
        }

        for (int i = 0; i < s2[0]; ++i) {

            getline(myfile, line);
            args = splitInput(line);

            for (int j = 0; j < s2[1]; ++j)
                matrix2[i][j] = stoi(args[j]);

        }

        myfile.close();

        ans[0] = matrix1;
        ans[1] = matrix2;

    }


    return ans;
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
