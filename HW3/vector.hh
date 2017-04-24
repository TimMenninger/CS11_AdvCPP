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

    /* Copies everything from first array to second.  This assumes that the
       arrays have compatible capacities. */
    void deepcopy(T* v) {
        init();
        for (int i = 0; i < cap; ++i)
            arr[i] = v[i];
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
        deepcopy(v.arr);
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
    T& at(int i) { return arr[i]; };
    const T& at(int i) const { return arr[i]; };

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
        if (numDeleted == 0)
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
    T*& at(int i) { return reinterpret_cast<T*&>(Base::at(i)); };
    const T*& at(int i) const {
        return reinterpret_cast<const T*&>(Base::at(i));
    };

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

#endif // ifndef VECTOR
