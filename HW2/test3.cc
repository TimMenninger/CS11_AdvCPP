#include "vector.hh"

int main(int argc, char **argv) {
    /* Just want to push something to a vector of some primitive type. */
    Vector<int> v1;
    v1.push_back(22);

    /* Creating something with a different primitive type. */
    Vector<char> v2;
    v2.push_back(22);

    /* Want a third vector of a third different primitive type. */
    Vector<float> v3;
    v3.push_back(22);

    return 0;
}
