#include <iostream>
#include <random>
#include <ctime>
#include "AVLTree.h"
using namespace std;
int main(){
    srand(time(0));
    AVLTree<int> a;
    for(int i = 0; i < 10; i++){
        int x = rand() % 48 + 1;
        a.insert(x);
        cout << x << " ";
    }
    cout << "\n";
    a.view();
    return 0;
}
