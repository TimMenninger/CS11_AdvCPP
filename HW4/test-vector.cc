#include "testbase.hh"
#include "vector.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>


using namespace std;


/*===========================================================================
 * HELPER FUNCTIONS
 *
 * These are used by the test suite.
 */

string make_string(int num_chars) {
    assert(num_chars > 0);

    string s;

    for (int i = 0; i < num_chars; i++)
        s += (char) ('A' + rand() % 26);

    return s;
}

string make_string(int min_chars, int max_chars) {
    assert(min_chars > 0);
    assert(max_chars >= min_chars);
    return make_string(min_chars + (1 + max_chars - min_chars) % rand());
}





/*===========================================================================
 * TEST FUNCTIONS
 *
 * These are called by the main() function at the end of this file.
 */


Vector<int> makeTestVector(int n) {
    Vector<int> v;
    for (int i = 0; i < n; i++)
        v.push_back(i);

    return v;
}


/*! Test the Rational constructors. */
void test_constructors(TestContext &ctx) {
    ctx.DESC("Vector size/capacity constructor");

    Vector<int> v1;
    ctx.CHECK(v1.size() == 0);
    ctx.CHECK(v1.capacity() == 0);
    ctx.CHECK(v1.begin() == v1.end());

    Vector<float> v2(10);
    ctx.CHECK(v2.size() == 10);

    ctx.CHECK(v2.capacity() >= 10);
    ctx.CHECK(v2.begin() < v2.end());

    ctx.result();

    ctx.DESC("Vector copy constructor");

    Vector<int> vo;
    vo.push_back(3);
    vo.push_back(5);
    vo.push_back(4);

    Vector<int> v3(vo);

    ctx.CHECK(vo[0] == 3);
    ctx.CHECK(vo[1] == 5);
    ctx.CHECK(vo[2] == 4);

    ctx.CHECK(v3[0] == 3);
    ctx.CHECK(v3[1] == 5);
    ctx.CHECK(v3[2] == 4);

    v3[1] = -2;

    ctx.CHECK(vo[0] == 3);
    ctx.CHECK(vo[1] == 5);
    ctx.CHECK(vo[2] == 4);

    ctx.CHECK(v3[0] == 3);
    ctx.CHECK(v3[1] == -2);
    ctx.CHECK(v3[2] == 4);

    ctx.result();

    ctx.DESC("Vector move constructor");

    Vector<int> v4 = makeTestVector(30);
    ctx.CHECK(v4.size() == 30);
    for (int i = 0; i < 30; i++)
        ctx.CHECK(v4[i] == i);

    ctx.result();
}


void test_assignment(TestContext &ctx) {
    Vector<int> v1, v2;

    ctx.DESC("Vector assignment operator");

    v1 = makeTestVector(30);     // Move-assignment
    v2 = v1;                     // Copy-assignment
    ctx.CHECK(v2.size() == 30);
    for (int i = 0; i < 30; i++) {
        ctx.CHECK(v1[i] == i);
        ctx.CHECK(v2[i] == i);
    }
    v2[12] = -63;
    ctx.CHECK(v1[12] == 12 && v2[12] == -63);

    Vector<int> v3;
    for (int i = 0; i < 1000; i++)
        v3.push_back(i);

    v3 = v1;
    ctx.CHECK(v3.size() == 30);
    for (int i = 0; i < 30; i++) {
        ctx.CHECK(v3[i] == i);
        ctx.CHECK(v1[i] == i);
    }
    v3[12] = -63;
    ctx.CHECK(v1[12] == 12 && v3[12] == -63);
    ctx.result();
}


void test_push_back(TestContext &ctx) {
    ctx.DESC("push_back() stores values and grows Vector properly");

    Vector<int> v;
    int i;
    const int NUMVALS = 70000;

    // Put 20,000 ints into the vector.
    for (i = 0; i < NUMVALS; i++)
        v.push_back(i);

    // Verify that the values look correct through array indexing.
    for (i = 0; i < NUMVALS; i++)
        ctx.CHECK(v[i] == i);

    // Verify that the values look correct through the iterator.
    Vector<int>::iterator it = v.begin();
    i = 0;
    while (it != v.end()) {
        ctx.CHECK(*it == i);
        i++;
        it++;
    }
    ctx.CHECK(i == NUMVALS);

    ctx.result();
}


void test_reverse(TestContext &ctx) {
    ctx.DESC("std::reverse() works with Vector");

    Vector<int> v;
    int i;
    const int NUMVALS = 100;

    for (i = 0; i < NUMVALS; i++)
        v.push_back(i);

    std::reverse(v.begin(), v.end());

    for (i = 0; i < NUMVALS; i++)
        ctx.CHECK(v[i] == NUMVALS - i - 1);

    // Verify that the values look correct through the iterator.
    Vector<int>::iterator it = v.begin();
    i = 0;
    while (it != v.end()) {
        ctx.CHECK(*it == NUMVALS - i - 1);
        i++;
        it++;
    }
    ctx.CHECK(i == NUMVALS);

    std::reverse(v.begin(), v.end());

    for (i = 0; i < NUMVALS; i++)
        ctx.CHECK(v[i] == i);

    ctx.result();
}


void test_sort_ints(TestContext &ctx) {
    const int NUMVALS = 10000;
    int i, last_i;

    ctx.DESC("std::sort() works with Vector<int>");

    Vector<int> v;

    // Insert random numbers
    for (i = 0; i < NUMVALS; i++)
        v.push_back(rand() % NUMVALS);

    // Sort it!
    std::sort(v.begin(), v.end());

    // Check the order
    last_i = v[0];
    for (i = 1; i < NUMVALS; i++) {
        ctx.CHECK(last_i <= v[i]);
        last_i = v[i];
    }

    // Verify that the values look correct through the iterator.
    Vector<int>::iterator it = v.begin();

    last_i = *it;
    it++;

    i = 1;

    while (it != v.end()) {
        ctx.CHECK(last_i <= *it);

        last_i = *it;
        it++;

        i++;
    }
    ctx.CHECK(i == NUMVALS);

    ctx.result();
}


void test_sort_strings(TestContext &ctx) {
    const int NUMVALS = 10000;
    int i;
    string last_i;

    ctx.DESC("std::sort() works with Vector<string>");

    Vector<string> v;

    // Insert random numbers
    for (i = 0; i < NUMVALS; i++) {
        v.push_back(make_string(4, 12));
    }

    // Sort it!
    std::sort(v.begin(), v.end());

    // Check the order
    last_i = v[0];
    for (i = 1; i < NUMVALS; i++) {
        ctx.CHECK(last_i <= v[i]);
        last_i = v[i];
    }

    // Verify that the values look correct through the iterator.
    Vector<string>::iterator it = v.begin();

    last_i = *it;
    it++;

    i = 1;

    while (it != v.end()) {
        ctx.CHECK(last_i <= *it);

        last_i = *it;
        it++;

        i++;
    }
    ctx.CHECK(i == NUMVALS);

    ctx.result();
}


/*===========================================================================
 * NEW TEST FUNCTIONS 0
 *
 * These should always pass after Lab 2
 */

void test_size_manipulation(TestContext &ctx) {
    ctx.DESC("[LAB 2+] testing: resize, clear, reserve, shrink_to_fit");

    size_t len_init = 10;
    size_t len_resize = 100;
    size_t len_reserve = 1000;

    Vector<int> v = makeTestVector(len_init);

    ctx.CHECK(v.size() == len_init);

    size_t expected_cap = len_init == 0 ? 0 : 1;
    size_t len_buffer = len_init;
    while (len_buffer != 0) {
        expected_cap <<= 1;
        len_buffer >>= 1;
    }
    ctx.CHECK(v.capacity() >= expected_cap);

    for (int i = 0; i < (int) v.size(); i++) {
        ctx.CHECK(v.at(i) == i);
    }

    v.shrink_to_fit();

    ctx.CHECK(v.size() == len_init);
    ctx.CHECK(v.capacity() >= len_init);

    v.resize(len_resize);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_resize);

    for (int i = 0; i < (int) len_init; i++) {
        ctx.CHECK(v.at(i) == i);
    }

    for (size_t i = len_init; i < len_resize; i++) {
        ctx.CHECK(v.at(i) == 0);
    }

    v.reserve(len_reserve);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_reserve);

    v.clear();

    ctx.CHECK(v.size() == 0);
    ctx.CHECK(v.capacity() >= len_reserve);

    ctx.result();
}

void test_insert_and_erase(TestContext &ctx) {
    ctx.DESC("[LAB 2+] testing: insert and erase");

    size_t len = 10;
    int insert_val = 13;
    size_t insert_offset = 5;

    Vector<int> v = makeTestVector(len);

    for (int i = 0; i < (int) v.size(); i++) {
        ctx.CHECK(v.at(i) == i);
    }

    v.insert(v.begin(), insert_val);
    ctx.CHECK(v.size() == len + 1);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (int i = 0; i < (int) len; i++) {
        ctx.CHECK(v.at(i + 1) == i);
    }

    v.insert(v.begin() + insert_offset, insert_val);
    ctx.CHECK(v.at(insert_offset) == insert_val);
    ctx.CHECK(*(v.begin() + insert_offset) == insert_val);
    ctx.CHECK(v.size() == len + 2);
    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (int i = 0; i < (int) len; i++) {
        ctx.CHECK(v.at(i + (i + 1 >= (int) insert_offset ? 2 : 1)) == i);
    }

    v.erase(v.begin() + insert_offset);
    ctx.CHECK(v.size() == len + 1);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (int i = 0; i < (int) len; i++) {
        ctx.CHECK(v.at(i + 1) == i);
    }

    v.erase(v.begin() + insert_offset, v.end());
    ctx.CHECK(v.size() == insert_offset);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (int i = 0; i < (int) insert_offset - 1; i++) {
        ctx.CHECK(v.at(i + 1) == i);
    }

    ctx.result();
}


/*===========================================================================
 * NEW TEST FUNCTIONS 1
 *
 * These should always pass after Lab 3
 */

struct DummyStruct {
    //
};

void test_size_manipulation_with_pointers(TestContext &ctx) {
    ctx.DESC("[LAB 3+] testing: resize, clear, reserve, shrink_to_fit");

    size_t len_init = 10;
    size_t len_resize = 100;
    size_t len_reserve = 1000;

    DummyStruct *ptrs[10 /*len_init*/] = {
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
    };

    Vector<DummyStruct*> v;
    for (size_t i = 0; i < len_init; i++) {
        v.push_back(ptrs[i]);
    }

    ctx.CHECK(v.size() == len_init);

    size_t expected_cap = len_init == 0 ? 0 : 1;
    size_t len_buffer = len_init;
    while (len_buffer != 0) {
        expected_cap <<= 1;
        len_buffer >>= 1;
    }
    ctx.CHECK(v.capacity() >= expected_cap);

    for (int i = 0; i < (int) v.size(); i++) {
        ctx.CHECK(v.at(i) == ptrs[i]);
    }

    v.shrink_to_fit();

    ctx.CHECK(v.size() == len_init);
    ctx.CHECK(v.capacity() >= len_init);

    v.resize(len_resize);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_resize);

    for (int i = 0; i < (int) len_init; i++) {
        ctx.CHECK(v.at(i) == ptrs[i]);
    }

    for (size_t i = len_init; i < len_resize; i++) {
        ctx.CHECK(v.at(i) == nullptr);
    }

    v.reserve(len_reserve);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_reserve);

    v.clear();

    ctx.CHECK(v.size() == 0);
    ctx.CHECK(v.capacity() >= len_reserve);

    ctx.result();

    for (size_t i = 0; i < len_init; i++) {
        delete ptrs[i];
    }
}

void test_insert_and_erase_with_pointers(TestContext &ctx) {
    ctx.DESC("[LAB 3+] testing: insert and erase");

    size_t len = 10;
    DummyStruct *insert_val = new DummyStruct();
    size_t insert_offset = 5;

    DummyStruct *ptrs[10 /*len*/] = {
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
        new DummyStruct(),
    };

    Vector<DummyStruct*> v;
    for (size_t i = 0; i < len; i++) {
        v.push_back(ptrs[i]);
    }

    for (int i = 0; i < (int) v.size(); i++) {
        ctx.CHECK(v.at(i) == ptrs[i]);
    }

    v.insert(v.begin(), insert_val);
    ctx.CHECK(v.size() == len + 1);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i + 1) == ptrs[i]);
    }

    v.insert(v.begin() + insert_offset, insert_val);
    ctx.CHECK(v.at(insert_offset) == insert_val);
    ctx.CHECK(*(v.begin() + insert_offset) == insert_val);
    ctx.CHECK(v.size() == len + 2);
    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i + (i + 1 >= insert_offset ? 2 : 1)) == ptrs[i]);
    }

    v.erase(v.begin() + insert_offset);
    ctx.CHECK(v.size() == len + 1);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i + 1) == ptrs[i]);
    }

    v.erase(v.begin() + insert_offset, v.end());
    ctx.CHECK(v.size() == insert_offset);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < insert_offset - 1; i++) {
        ctx.CHECK(v.at(i + 1) == ptrs[i]);
    }

    ctx.result();
}


/*===========================================================================
 * NEW TEST FUNCTIONS 2
 *
 * These should always pass after Lab 3
 */

void test_size_manipulation_with_bools(TestContext &ctx) {
    ctx.DESC("[LAB 4+] testing: resize, clear, reserve, shrink_to_fit");

    size_t len_init = 10;
    size_t len_resize = 100;
    size_t len_reserve = 1000;

    const bool bools[10 /*len_init*/] = {
        true,
        false,
        true,
        false,
        false,
        false,
        true,
        false,
        true,
        true
    };

    Vector<bool> v;
    for (size_t i = 0; i < len_init; i++) {
        v.push_back(bools[i]);
    }

    ctx.CHECK(v.size() == len_init);

    size_t expected_cap = len_init == 0 ? 0 : 1;
    size_t len_buffer = len_init;
    while (len_buffer != 0) {
        expected_cap <<= 1;
        len_buffer >>= 1;
    }
    ctx.CHECK(v.capacity() >= expected_cap);

    for (size_t i = 0; i < v.size(); i++) {
        ctx.CHECK(v.at(i) == bools[i]);
    }

    v.shrink_to_fit();

    ctx.CHECK(v.size() == len_init);
    ctx.CHECK(v.capacity() >= len_init);

    v.resize(len_resize);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_resize);

    for (size_t i = 0; i < len_init; i++) {
        ctx.CHECK(v.at(i) == bools[i]);
    }

    for (size_t i = len_init; i < len_resize; i++) {
        ctx.CHECK(v.at(i) == false);
    }

    v.reserve(len_reserve);

    ctx.CHECK(v.size() == len_resize);
    ctx.CHECK(v.capacity() >= len_reserve);

    v.clear();

    ctx.CHECK(v.size() == 0);
    ctx.CHECK(v.capacity() >= len_reserve);

    ctx.result();
}

void test_insert_and_erase_with_bools(TestContext &ctx) {
    ctx.DESC("[LAB 4+] testing: insert and erase");

    size_t sub_len = 10;
    size_t page_size = 32;
    size_t full_page_count = 0;
    size_t len = sub_len + page_size * full_page_count;
    bool insert_val = true;
    size_t insert_offset = 5;

    const bool bools[10 /*sub_len*/] = {
        true,
        false,
        true,
        false,
        false,
        false,
        true,
        false,
        true,
        true
    };

    Vector<bool> v;
    for (size_t i = 0; i < len; i++) {
        v.push_back(bools[i % sub_len]);
    }

    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i) == bools[i % sub_len]);
    }

    v.insert(v.begin(), insert_val);
    ctx.CHECK(v.size() == len + 1);

    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i + 1) == bools[i % sub_len]);
    }

    v.insert(v.begin() + insert_offset, insert_val);
    ctx.CHECK(v.at(insert_offset) == insert_val);
    ctx.CHECK(*(v.begin() + insert_offset) == insert_val);
    ctx.CHECK(v.size() == len + 2);
    ctx.CHECK(v.at(0) == insert_val);
    ctx.CHECK(*v.begin() == insert_val);
    for (size_t i = 0; i < len; i++) {
        ctx.CHECK(v.at(i + (i + 1 >= insert_offset ? 2 : 1))
            == bools[i % sub_len]);
    }

    // v.erase(v.begin() + insert_offset);
    // ctx.CHECK(v.size() == len + 1);

    // ctx.CHECK(v.at(0) == insert_val);
    // ctx.CHECK(*v.begin() == insert_val);
    // for (size_t i = 0; i < len; i++) {
    //     printf("%d: %s vs %s\n", i + 1, v.at(i + 1) ? "TRUE" : "FALSE",
    //         bools[i % sub_len] ? "TRUE" : "FALSE");
    //     ctx.CHECK(v.at(i + 1) == bools[i % sub_len]);
    // }

    // v.erase(v.begin() + insert_offset, v.end());
    // ctx.CHECK(v.size() == insert_offset);

    // ctx.CHECK(v.at(0) == insert_val);
    // ctx.CHECK(*v.begin() == insert_val);
    // for (int i = 0; i < insert_offset - 1; i++) {
    //     ctx.CHECK(v.at(i + 1) == bools[i]);
    // }

    ctx.result();
}


/*! This program is a simple test-suite for the Rational class. */
int main() {

    cout << "Testing the Vector class." << endl << endl;

    srand(654321L);

    TestContext ctx(cout);

    test_constructors(ctx);
    test_assignment(ctx);
    test_push_back(ctx);
    test_reverse(ctx);
    test_sort_ints(ctx);
    // test_sort_strings(ctx);
    test_size_manipulation(ctx);
    test_insert_and_erase(ctx);
    test_size_manipulation_with_pointers(ctx);
    test_insert_and_erase_with_pointers(ctx);
    test_size_manipulation_with_bools(ctx);
    test_insert_and_erase_with_bools(ctx);

    // Return 0 if everything passed, nonzero if something failed.
    return !ctx.ok();
}
