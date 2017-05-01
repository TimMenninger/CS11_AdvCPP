#include "iostream"
#include "vector.hh"

int main(int argc, char **argv) {
    Vector<bool> v1(1);

    Vector<bool> v(1);
    v.push_back(true);
    std::cout << v.at(0) << " " << v.at(1) << std::endl;
    v[1] = v1[0];
    std::cout << v.at(0) << " " << v.at(1) << " " << (v[0] == v[1]) << std::endl;
    v[1] = true;
    std::cout << v.at(0) << " " << v.at(1) << " " << (v[0] == v[1]) << std::endl;

    Vector<bool>::iterator it = v.begin();
    std::cout << *it << std::endl;
    it = ++it;
    std::cout << *it << std::endl;
    it = it--;
    std::cout << *it << std::endl;
    it = v.begin();
    it = it + 5;
    it += 4;
    it -= 4;
    it = it - 4;
    std::cout << *it << std::endl;
    it = v.begin();
    std::cout << (it == it) << " " << (it != it) << std::endl;
    std::cout << (it == (it+1)) << " " << (it != (it+1)) << std::endl;
    it++;

    std::cout << v.capacity() << std::endl;
    v.reserve(33);
    std::cout << v.capacity() << std::endl;

    v.push_back(true);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v.size() << std::endl;
    v.resize(5);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " " << v.size() << std::endl;
    v[0] = true;
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " " << v.size() << std::endl;
    v.resize(1);
    std::cout << v[0] << " " << v.size() << std::endl;

    it = v.begin();
    v.resize(0);
    v.push_back(true);
    v.push_back(true);
    std::cout << v[0] << " " << v[1] << " " << v.size() << std::endl;
    v.insert(it, false);
    v.insert(it, true);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " <<  v.size() << std::endl;
    v.erase(it);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v.size() << std::endl;

    return 0;
}
