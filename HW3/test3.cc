#include "vector.hh"

int main(int argc, char **argv) {
    /* Value pushed for testing sizes. */
    void *test_val = NULL;

    /* Just want to push something to a vector of some primitive type. */
    Vector<int *> v1;
    v1.push_back((int *) test_val);

    /* Creating something with a different primitive type. */
    Vector<char *> v2;
    v2.push_back((char *) test_val);

    /* Want a third vector of a third different primitive type. */
    Vector<float *> v3;
    v3.push_back((float *) test_val);

    return 0;
}
