#include "vector.hh"
#include "common.hh"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    Vector<int> hi(0);
    hi.push_back(3);
    hi.push_back(3);
    hi.push_back(3);
    Vector<int>::iterator it = hi.begin()+1;
    cout << hi[0] << " " << hi[1] << " " << hi[2] << " " << hi[3] << " " << hi[4] << endl;
    hi.insert(it, 5);
    cout << hi[0] << " " << hi[1] << " " << hi[2] << " " << hi[3] << " " << hi[4] << endl;
    cout << smallestPow2(-1) << " " << smallestPow2(4) << " " << smallestPow2(65000) << endl;
}
