#include "iostream"
#include "vector.hh"

int main(int argc, char **argv) {
    Vector<bool> v(5);
    std::cout << v.size() << " " << v.capacity() << std::endl;
    std::cout << v.at(1) << " " << v[1] << std::endl;

    v.resize(3);
    v.reserve(20);
    std::cout << v.size() << " " << v.capacity() << std::endl;

    v.reserve(35);
    std::cout << v.size() << " " << v.capacity() << std::endl;

    v.shrink_to_fit();
    std::cout << v.size() << " " << v.capacity() << std::endl;

    v.clear();
    std::cout << v.size() << " " << v.capacity() << std::endl;

    Vector<bool> v1(1);
    v1.push_back(true);
    std::cout << v1.at(0) << " " << v1.at(1) << std::endl;

    return 0;
}
