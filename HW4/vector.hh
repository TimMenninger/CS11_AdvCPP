/*
 vector.hh

 Implements my own version of Vector, similar to std::vector.

 Revisions:
    09 Apr 2017 - Tim Menninger: Created
*/

#ifndef VECTOR
#define VECTOR

#include <stdlib.h>
#include <string>
#include <assert.h>
#include <math.h>
#include "common.hh"

/******************************************************************************

 BASE VECTOR CLASS

 Contains all of the vector functions used by all vector instantiations.

******************************************************************************/
template <typename T>
class VectorBase {

private:
    T *arr;                 /* Array of elements of type T */
    int len;                /* Size of the array */
    int cap;                /* Capacity of the array */

    /* Initializes the array pointer */
    void init() {
        arr = cap == 0 ? NULL : (T*) malloc(cap * sizeof(T));
        for (int i = 0; i < cap; ++i)
            arr[i] = T();
    };

    /* Re-initializes the array pointer, assuming arr is defined. */
    void reinit(int new_cap) {
        /* Don't want this to be called with a zero-argument. */
        if (new_cap == 0 && arr) delete arr;
        /* Make space for new array size, copying over contents */
        arr = new_cap == 0 ? NULL : (T*) realloc(arr, new_cap * sizeof(T));
        /* Initialize anything new */
        for (int i = cap; i < new_cap; ++i)
            arr[i] = T();
        /* Update capacity */
        cap = new_cap;
    };

public:

    typedef T* iterator;


    /******************************
     CONSTRUCTORS
     ******************************/

    VectorBase() : len(0), cap(0) { init(); };
    VectorBase(int size) : len(size), cap(smallestPow2(size)) { init(); };
    VectorBase(int size, int cap) : len(size), cap(cap) { init(); };

    /* Copy constructor */
    VectorBase(const VectorBase<T>& v) : len(v.size()), cap(v.capacity()) {
        init();
        memcpy((void *) arr, (void *) v.arr, cap * sizeof(T));
    };

    /* Move constructor */
    VectorBase(VectorBase<T>&& v) {
        memcpy((void *) this, (void *) &v, sizeof(VectorBase<T>&&));
        memset((void *) &v, 0, sizeof(VectorBase<T>&&));
    };


    /******************************
     DESTRUCTOR
     ******************************/

    ~VectorBase() { if (arr) delete arr; };


    /******************************
     ACCESSORS
     ******************************/

    int size() const { return len; };
    int capacity() const { return cap; };

    /* Returns the element at the argued index. */
    T at(int i) { return arr[i]; };

    /* Returns an iterator at the beginning of the array, pointing to the
       first element. */
    iterator begin() { return arr; };

    /* Returns an iterator at the end of the array, just past the end */
    iterator end() { return (arr + len); };


    /******************************
     OPERATORS
     ******************************/

    /* Copy assignment */
    VectorBase operator=(const VectorBase<T>& v) {
        if (this != &v)
            *this = VectorBase(v);
        return *this;
    };

    /* Move assignment */
    VectorBase operator=(const VectorBase<T>&& v) {
        if (this != &v) {
            memcpy((void *) this, (void *) &v, sizeof(VectorBase<T>&&));
            memset((void *) &v, 0, sizeof(VectorBase<T>&&));
        }
        return *this;
    };

    /* Access an element in the array. */
    T& operator[](int i) { return arr[i]; };
    const T& operator[](int i) const { return arr[i]; };


    /******************************
     VECTOR MUTATION
     ******************************/

    /* Updates the capacity and allocates space for it.  If the new capacity is
       smaller than the current one, nothing happens. */
    void reserve(int new_cap) {
        /* Only change anything if new cap is larger */
        if (new_cap > cap)
            reinit(new_cap);
    };

    /* If the capacity is larger than the size, this updates the capacity to
       be the same as the size, and reallocates the array. */
    void shrink_to_fit() {
        /* Don't need to do anything if cap is already the same as size */
        if (cap > len)
            reinit(len);
    };

    /* Resizes the array.  This initializes anything that was previously within
       size but no longer is. */
    void resize(int count) {
        /* If we need more space, we need to reallocate */
        if (count > cap)
            reinit(smallestPow2(count));

        /* Now resize, clearing anything that is no longer in use */
        for (int i = count; i < len; ++i)
            arr[i] = T();

        /* Update size */
        len = count;
    };

    /* Clears the array, keeping the capacity the same but changing the length
       to zero. */
    void clear() { resize(0); };

    /* Appends element to the end of the array. */
    void push_back(const T& elem) {
        /* If we need more space, allocate it. */
        while (len >= cap)
            reinit(std::max(1, cap) << 1);

        /* Add to the end of the array and increment len */
        arr[len++] = elem;
    };

    /* Appends element to the end of the array. */
    void push_back(T&& elem) {
        /* If we need more space, allocate it. */
        while (len >= cap)
            reinit(std::max(1, cap) << 1);

        /* Add to the end of the array and increment len */
        arr[len++] = elem;
    };

    /* Inserts an element at the specified position, pushing everything else
       back. */
    void insert(iterator pos, const T& elem) {
        /* We might need more space for the new element. */
        push_back(arr[len-1]);

        /* Insert the element and push everything else back. */
        iterator it = end()-1;
        while (--it >= pos)
            *(it+1) = *it;
        *pos = elem;
    };

    /* Inserts an element at the specified position, pushing everything else
       back. */
    void insert(iterator pos, T&& elem) {
        /* We might need more space for the new element. */
        push_back(arr[len-1]);

        /* Insert the element and push everything else back. */
        iterator it = end()-1;
        while (--it >= pos)
            *(it+1) = *it;
        *pos = elem;
    };

    /* Erases everything from the first point to the element before the last
       point. */
    void erase(iterator first, iterator last) {
        /* Make sure we have valid pointers. */
        assert(first);
        assert(last);

        /* Number of elements deleted to help shifting things left. */
        int numDeleted = (last - first);
        /* Don't erase if they're the same. */
        if (numDeleted <= 0)
            return;

        /* Shift everything so that we erased the desired parts, and then the
           gibberish is on the end (which we will remove with a resize) */
        while (first < end()-numDeleted) {
            *first = *(first+numDeleted);
            first++;
        }

        /* Resize according to how many we deleted. */
        resize(len-numDeleted);
    };

    /* Erases what is at the position argued. */
    void erase(iterator pos) {
        erase(pos, pos+1);
    }
};

/******************************************************************************

 VECTOR CLASS

 Contains implementation of vector for everything that doesn't otherwise have
 a partial specification

******************************************************************************/
template<typename T>
class Vector : public VectorBase<T> {
public:

    typedef VectorBase<T> Base;


    /******************************
     CONSTRUCTORS
     ******************************/

    Vector() : Base::VectorBase() {};
    Vector(int size) : Base::VectorBase(size) {};
    Vector(int size, int cap) : Base::VectorBase(size, cap) {};

    /* Copy constructor */
    Vector(const Vector<T>& v) : Base::VectorBase(v) {};

    /* Move constructor */
    Vector(Vector<T>&& v) : Base::VectorBase(v) {};


    /******************************
     OPERATORS
     ******************************/

    /* Copy assignment */
    Vector operator=(const Vector<T>& v) {
        if (this != &v)
            *this = Vector(v);
        return *this;
    };

    /* Move assignment */
    Vector operator=(const Vector<T>&& v) {
        if (this != &v) {
            memcpy((void *) this, (void *) &v, sizeof(Vector<T>&&));
            memset((void *) &v, 0, sizeof(Vector<T>&&));
        }
        return *this;
    };
};

/******************************************************************************

 VECTOR CLASS FOR VOID*

 This vector class implements the Vector class for exclusively void* types.

******************************************************************************/
template <> class Vector<void*> : public VectorBase<void*> {
public:

    typedef VectorBase<void*> Base;


    /******************************
     CONSTRUCTORS
     ******************************/

    Vector() : Base::VectorBase() {};
    Vector(int size) : Base::VectorBase(size) {};
    Vector(int size, int cap) : Base::VectorBase(size, cap) {};

    /* Copy constructor */
    Vector(const Vector<void*>& v) : Base::VectorBase(v) {};

    /* Move constructor */
    Vector(Vector<void*>&& v) : Base::VectorBase(v) {};


    /******************************
     OPERATORS
     ******************************/

    /* Copy assignment */
    Vector operator=(const Vector<void*>& v) {
        if (this != &v)
            *this = Vector(v);
        return *this;
    };

    /* Move assignment */
    Vector operator=(const Vector<void*>&& v) {
        if (this != &v) {
            memcpy((void *) this, (void *) &v, sizeof(Vector<void*>&&));
            memset((void *) &v, 0, sizeof(Vector<void*>&&));
        }
        return *this;
    };
};

/******************************************************************************

 PARTIAL SPECIALIZATION FOR VECTOR OF POINTERS

 This vector class inherits from the Vector<void*> base class.  It handles
 exclusively pointers, casting them all to void * and using one copy of
 Vector<void*>.

******************************************************************************/
template <typename T>
class Vector<T*> : private Vector<void*> {

public:

    typedef Vector<void*> Base;
    typedef T** iterator;


    /******************************
     CONSTRUCTORS
     ******************************/

    Vector() : Base::Vector() { };
    Vector(int size) : Base::Vector(size) { };
    Vector(int size, int cap) : Base::Vector(size, cap) { };

    /* Copy constructor */
    Vector(const Vector<T*>& v) : Base::Vector(v) { };

    /* Move constructor */
    Vector(Vector<T*>&& v) : Base::Vector(v) { };


    /******************************
     DESTRUCTOR
     ******************************/

     /* Everything already freed by base class. */
    ~Vector() { };


    /******************************
     ACCESSORS
     ******************************/

    int size() const { return Base::size(); };
    int capacity() const { return Base::capacity(); };

    /* Returns the element at the argued index. */
    T* at(int i) { return reinterpret_cast<T*>(Base::at(i)); };

    /* Returns an iterator at the beginning of the array, pointing to the
       first element. */
    iterator begin() { return (iterator) Base::begin(); };

    /* Returns an iterator at the end of the array, just past the end */
    iterator end() { return (iterator) Base::end(); };


    /******************************
     OPERATORS
     ******************************/

    /* Copy assignment */
    Vector operator=(const Vector<T*>& v) {
        return reinterpret_cast<Vector<T*>>(Base::operator=(v));
    };

    /* Move assignment */
    Vector operator=(const Vector<T*>&& v) {
        return reinterpret_cast<Vector<T*>>(Base::operator=(v));
    };

    /* Access an element in the array. */
    T*& operator[](int i) {
        return reinterpret_cast<T*&>(Base::at(i));
    };
    const T*& operator[](int i) const {
        return reinterpret_cast<const T*&>(Base::at(i));
    };


    /******************************
     VECTOR MUTATION
     ******************************/

    /* Updates the capacity and allocates space for it.  If the new capacity is
       smaller than the current one, nothing happens. */
    void reserve(int new_cap) { Base::reserve(new_cap); };

    /* If the capacity is larger than the size, this updates the capacity to
       be the same as the size, and reallocates the array. */
    void shrink_to_fit() { Base::shrink_to_fit(); };

    /* Resizes the array.  This initializes anything that was previously within
       size but no longer is. */
    void resize(int count) { Base::resize(count); };

    /* Clears the array, keeping the capacity the same but changing the length
       to zero. */
    void clear() { Base::resize(0); };

    /* Appends element to the end of the array. */
    void push_back(const T* elem) { Base::push_back((void*) elem); };

    /* Appends element to the end of the array. */
    void push_back(T*&& elem) { Base::push_back((void*) elem); };

    /* Inserts an element at the specified position, pushing everything else
       back. */
    void insert(iterator pos, const T* elem) {
        Base::insert((Base::iterator) pos, (void*) elem);
    };

    /* Inserts an element at the specified position, pushing everything else
       back. */
    void insert(iterator pos, T*&& elem) {
        Base::insert((Base::iterator) pos, (void*) elem);
    };

    /* Erases everything from the first point to the element before the last
       point. */
    void erase(iterator first, iterator last) {
        Base::erase((Base::iterator) first, (Base::iterator) last);
    };

    /* Erases what is at the position argued. */
    void erase(iterator pos) {
        Base::erase((Base::iterator) pos, (Base::iterator) pos+1);
    };
};

/******************************************************************************

 PARTIAL SPECIALIZATION FOR BOOLEAN

 This vector class uses unisgned integers to represent bit vectors.

******************************************************************************/
template <>
class Vector<bool> {

private:
    class Bit;              /* For accessing and mutating bits. */
    class Iterator;         /* For iterating over bits. */

    Vector<uint32_t> arr;   /* Each element contains 32 bool values. */
    int len;                /* Number of bits in vector. */
    int cap;                /* Capacity of bits. */

    /* Initializes the uint32 vector based on len and cap. */
    void init() {
        /* If the capacity is 0, we have no vector. */
        if (cap == 0)
            return;

        /* Otherwise, we only need one uint32 for every 32 bools. */
        arr = Vector<uint32_t>(ceil((double) cap/32));
    };

public:

    typedef Iterator iterator;


    /******************************
     CONSTRUCTORS
     ******************************/

    Vector() : len(0), cap(0) { init(); };
    Vector(int size) : len(size), cap(smallestPow2(size)) { init(); };
    Vector(int size, int cap) : len(size), cap(cap) { init(); };

    /* Copy constructor */
    Vector(const Vector<bool>& v) : len(v.size()), cap(v.capacity()) {
        init();
        memcpy((void *) &arr, (void *) &v.arr,
            ceil((double) cap/32) * sizeof(uint32_t));
    };

    /* Move constructor */
    Vector(Vector<bool>&& v) {
        memcpy((void *) this, (void *) &v, sizeof(Vector<bool>&&));
        memset((void *) &v, 0, sizeof(Vector<bool>&&));
    };


    /******************************
     DESTRUCTOR
     ******************************/

    /* Nothing to delete. */
    ~Vector() { };


    /******************************
     ACCESSORS
     ******************************/

    int size() const { return len; };
    int capacity() const { return cap; };

    bool at(int i) const {
        /* Ensure i is in range since the vector can't do it. */
        if (i >= len)
            throw std::out_of_range("Vector<bool>::at");

        return (bool) (arr[i/32] & (1 << i%32));
    };

    /* Returns an iterator at the beginning of the array, pointing to the
       first element. */
    iterator begin() { return Iterator(&arr, 0); };

    /* Returns an iterator at the end of the array, just past the end */
    iterator end() { return Iterator(&arr, len); };


    /******************************
     OPERATORS
     ******************************/

    /* Copy assignment */
    Vector operator=(const Vector<bool>& v) {
        if (this != &v)
            *this = Vector(v);
        return *this;
    };

    /* Move assignment */
    Vector operator=(const Vector<bool>&& v) {
        if (this != &v) {
            memcpy((void *) this, (void *) &v, sizeof(Vector<bool>&&));
            memset((void *) &v, 0, sizeof(Vector<bool>&&));
        }
        return *this;
    };

    /* Mutate element in the array. */
    Bit operator[](int i) {
        if (i >= len)
            throw std::out_of_range("Index out of range");
        return Bit(&arr, i);
    };


    /******************************
     VECTOR MUTATION
     ******************************/

    /* Updates the capacity and allocates space for it.  If the new capacity is
       smaller than the current one, nothing happens. */
    void reserve(int new_cap) {
        for (int i = arr.size(); i < ceil((double) new_cap/32); i++)
            /* Allocate as many 32-bit values as we need, init to false */
            arr.push_back(0);

        /* Save the new capacity. */
        cap = std::max(cap, new_cap);
    };

    /* If the capacity is larger than the size, this updates the capacity to
       be the same as the size, and reallocates the array. */
    void shrink_to_fit() {
        /* Change the capacity to equal the size. */
        cap = len;

        /* Remove unused values from the array. */
        arr.erase(arr.begin() + (int) ceil((double) cap/32), arr.end());
    };

    /* Resizes the array.  This initializes anything that was previously within
       size but no longer is. */
    void resize(int count) {
        /* Remove any unneeded values in the uint32 array. */
        arr.resize(ceil((double) count/32));

        /* Initialize everything that was previously in the array.  We do this
           by erasing entire 32-bit values where possible, then iterating over
           bits in the last one, clearing what is unused. */
        if (count < len)
            arr[arr.size()-1] &= ~(-1 << count%32);

        /* Update the size. */
        len = count;
        cap = std::max(cap, count);
    };

    /* Clears the array, keeping the capacity the same but changing the length
       to zero. */
    void clear() { resize(0); };

    /* Appends element to the end of the array. */
    void push_back(const bool& elem) {
        /* Make space if we need it. */
        while (len >= cap)
            reserve(std::max(1, cap << 1));

        /* Assign the bit accordingly and update the length. */
        arr[len/32] |= (elem << len%32);
        len++;
    };

    /* Appends element to the end of the array. */
    void push_back(bool&& elem) {
        /* Make space if we need it. */
        while (len >= cap)
            reserve(std::max(1, cap << 1));

        /* Assign the bit accordingly and update the length. */
        arr[len/32] |= (elem << len%32);
        len++;
    };

    /* Inserts an element at the specified position, pushing everything else
       back. */
    void insert(iterator pos, const bool elem) {
        /* We might need more space for the new element. */
        resize(len+1);

        /* Insert the element and push everything else back. */
        iterator it = end();
        for (; it != pos; it--) {
            *it = *(it-1);
        }
        *it = elem;
    };

    /* Erases everything from the first point to the element before the last
       point. */
    void erase(iterator first, iterator last) {
        /* Number of elements deleted to help shifting things left. */
        int numDeleted = (last - first);

        /* Don't erase if they're the same. */
        if (numDeleted <= 0)
            return;

        /* Shift everything so that we erased the desired parts, and then the
           gibberish is on the end (which we will remove with a resize) */
        while (first < end()-numDeleted) {
            *first = *(first+numDeleted);
            first++;
        }

        /* Resize according to how many we deleted. */
        resize(len-numDeleted);
    };

    /* Erases what is at the position argued. */
    void erase(iterator pos) {
        erase(pos, pos+1);
    };

/***************************
 HELPER CLASSES
 **************************/
private:

    /* Because we can't return a reference to a single bit in a uint, we need
       to make a special class that gives the appearance that we can... */
    class Bit {

        friend class Iterator;

    private:

        Vector<uint32_t> *v;/* The integer where we are mutating a bit. */
        int idx;            /* The index of the bit being mutated. */

    public:

        /* Constructors */
        Bit() : v(0), idx(0) {};
        Bit(Vector<uint32_t> *v) : v(v), idx(0) {};
        Bit(Vector<uint32_t> *v, int i) : v(v), idx(i) {};

        /* Destructor */
        ~Bit() {}; /* Vector freed by Vector class */

        /* The index to use when setting the value. */
        void setIndex(int i) { idx = i; };

        /* Add to index. */
        void addToIndex(int offset) { idx += offset; };

        /* This is what eventually gets called on the [] operator in Vector. */
        Bit operator=(const bool&& b) {
            /* Set the i'th bit in the vector to the argued bool. */
            (*v)[idx/32] &= ~(1 << (idx%32));
            (*v)[idx/32] |= b << (idx%32);
            return *this;
        };

        /* This is what eventually gets called on the [] operator in Vector. */
        Bit operator=(const Bit& bit) {
            /* Set the i'th bit in the vector to the argued bool. */
            bool b = (bool) ((*bit.v)[bit.idx/32] & (1 << (bit.idx%32)));

            /* Clear that bit then OR in the new one. */
            (*v)[idx/32] &= ~(1 << (idx%32));
            (*v)[idx/32] |= b << (idx%32);
            return *this;
        };

        /* Need an operator that takes a regular boolean. */
        Bit operator=(const bool& b) {
            /* Set the i'th bit in the vector to the argued bool. */
            (*v)[idx/32] &= ~(1 << (idx%32));
            (*v)[idx/32] |= b << (idx%32);
            return *this;
        }

        /* Compare two bits. */
        const bool operator==(Bit& b) {
            return ((*v)[idx/32] & (1 << idx%32)) ==
                ((*b.v)[b.idx/32] & (1 << b.idx%32));
        }
        const bool operator!=(Bit& b) {
            return ((*v)[idx/32] & (1 << idx%32)) !=
                ((*b.v)[b.idx/32] & (1 << b.idx%32));
        }

        /* We define how to interpret Bit as a bool so when the Vector
           is accessed through the [] operator, which returns Bit, we
           can implicitly cast to this boolean value. */
        const operator bool() {
            /* For whatever reason, this only works if we store it before
               returning. */
            bool out = (*v)[idx/32] & (1 << idx%32);
            return out;
        };
    };

    /* A special class that allows us to "dereference" and iterate over bits
       in an unsigned integer. */
    class Iterator {

    private:

        Bit *bit;           /* Allows us to modify bits using the iterator. */

    public:

        /* Constructors */
        Iterator() : bit(new Bit()) {};
        Iterator(Vector<uint32_t> *v, int i) : bit(new Bit(v, i)) {};
        Iterator(const Iterator& it) {
            memcpy((void *) this, (void *) &it, sizeof(Iterator));
            this->bit = new Bit();
            memcpy((void *) this->bit, (void *) it.bit, sizeof(Bit));
        };

        /* Destructor */
        ~Iterator() { if (bit) delete bit; };

        /* Operators */
        /* post increment */
        const Iterator operator++(int) {
            /* Store the original version that will be returned */
            Iterator out(*this);
            /* Increment the instance */
            *this += 1;
            return out;
        };

        /* pre increment */
        Iterator& operator++() {
            /* Add one then return */
            *this += 1;
            return *this;
        };

        /* post decrement */
        const Iterator operator--(int) {
            /* Store the original version that will be returned */
            Iterator out(*this);
            /* Decrement the instance */
            *this -= 1;
            return out;
        };

        /* pre decrement */
        Iterator& operator--() {
            /* Subtract one then return */
            *this -= 1;
            return *this;
        };

        /* Dereference for reading/writing */
        Bit& operator* () { return *bit; };

        /* Move assignment */
        Iterator operator=(const Iterator&& it) {
            /* Set the i'th bit in the vector to the argued bool. */
            memcpy((void *) this, (void *) &it, sizeof(Iterator));
            memset((void *) &it, 0, sizeof(Iterator));
            return *this;
        };

        /* Copy assignment */
        Iterator operator=(const Iterator& it) {
            /* Set the i'th bit in the vector to the argued bool. */
            *this = Iterator(it);
            return *this;
        };
        bool operator>(const Iterator& it) {
            return (bit->idx > it.bit->idx);
        };
        bool operator<(const Iterator& it) {
            return (bit->idx < it.bit->idx);
        };
        bool operator>=(const Iterator& it) {
            return (bit->idx >= it.bit->idx);
        };
        bool operator<=(const Iterator& it) {
            return (bit->idx <= it.bit->idx);
        };

        /* Compare two iterators. */
        const bool operator==(const Iterator& it) {
            return (bit->v == it.bit->v) && (bit->idx == it.bit->idx);
        };
        const bool operator!=(const Iterator& it) {
            return (bit->v != it.bit->v) || (bit->idx != it.bit->idx);
        };

        /* Pointer arithmetic with iterators */
        Iterator operator-(const int& i) {
            Iterator out(*this);
            out.bit->addToIndex(-i);
            return out;
        };
        const Iterator& operator-=(const int& i) {
            bit->addToIndex(-i);
            return *this;
        };
        Iterator operator+(const int& i) {
            Iterator out = iterator(*this);
            out.bit->addToIndex(i);
            return out;
        };
        const Iterator& operator+=(const int& i) {
            bit->addToIndex(i);
            return *this;
        };

        /* This allows us to see the difference between two iterators */
        int operator-(const Iterator& i) {
            if (i.bit->v != bit->v)
                return -1;
            return bit->idx - i.bit->idx;
        };
    };
};

#endif // ifndef VECTOR
