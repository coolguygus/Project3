#include <iostream>
#include <fstream>
#include "queue.h"
#include "linearlist.h"

using namespace std;

struct words{
    int index = -1;
    string word;
    LinearList<int> adj;
};

const int TABLE_SIZE = 5757;

words wordList[TABLE_SIZE];

int hammingDist (string, string);
int seqSearch (words[], string, int);
void genLadder (int, int);
void fillList ();
void findHam1 ();

int main() {
    string inp1;
    string inp2;

    fillList();

    findHam1();

    cout << "Please enter word 1" << endl;
    cin >> inp1;

    cout << "Please enter word 2" << endl;
    cin >> inp2;

    genLadder(seqSearch(wordList, inp1, TABLE_SIZE), seqSearch(wordList, inp2, TABLE_SIZE));

    return 0;  
}

int hammingDist (string a, string b) {
    int i = 0;
    int dist = 0;

    while (a[i]) {
        if (a[i] != b[i]) {
            dist++;
        }
        i++;
    }
    return dist;
}

int seqSearch (words array[], string key, int length) {
    for (int i = 0; i < length; i++)
        if (array[i].word == key)
            return i;

    return -1;
}

void fillList() {
    ifstream file;
    string word;

    file.open("sgb-words.txt");

    if(!file.is_open()) {
        cout << "ERROR: File was not opened" << endl;
        exit(1);
    }

    int i = 0;
    while (getline(file, word)) {
        wordList[i].index = i;
        wordList[i].word = word;

        i++;
    }
    file.close();
}

void findHam1() {
    int x,y;
    for (x = 0; x < 5757; x++) {
        for (y = 0; y < 5757; y++){
            if (hammingDist(wordList[x].word,wordList[y].word) == 1){
                wordList[x].adj.insert(0, y);
                wordList[y].adj.insert(0, x);
            }
        }
    }
}

void genLadder (int w1, int w2){
    Queue<int> Q;
    int v;
    int w;

    Q.enqueue(w2);

    while(!Q.isEmpty()){
        w = Q.dequeue();
        
        for (int i = 0; i < wordList[w].adj.size(); i++){
            v = wordList[w].adj[i];

            if (wordList[v].index == -1 && v != w2){
                wordList[v].index = w;
                Q.enqueue(v);
            }
        }
    }

    if (wordList[w1].index != -1){
        cout << wordList[w1].word << endl; //prints out word at index/address w1
        w = wordList[w1].index;

        while (w = -1){
            cout << wordList[w].word << endl;
            w = wordList[w].index;
        }
    }
    else{
        cout << "No Ladder Exists" << endl;
    }
}