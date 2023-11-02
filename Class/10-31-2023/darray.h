#ifndef DARRAY_H
#define DARRAY_H

#include <iostream>
#include <exception>

#define DEFAULT_SIZE 10

template <class T>

class darray {
    private:
        T *a;
        int size;
        int caboose;
        void resize() {
            T *tmp;
            int i;
            tmp = new T[size * 2];
            for (i = 0; i < size; i++) {
                tmp[i] = a[i];
            }

            delete [] a;
            a = tmp;
            size *= 2;
        };
    public:
        class iterator {
            private:
                int itr;
            public:
                iterator() : itr(0) {}
                void operator++() {
                    if((itr + 1) < caboose) {
                        itr++;
                    }
                }
                // iterator start() {
                //     iterator i;
                //     return i;
                // }
                T &operator*() {
                    return a[itr];
                }
        };

        darray() : a(new T[DEFAULT_SIZE]), size(DEFAULT_SIZE), caboose(0) {}
        darray(int s) : a(new T[s]), size(s), caboose(0) {}
        ~darray() {
            delete [] a;
        }

        T  &operator[](int i) {
            if(i >= size) {
                throw "Index out of bounds";
            }
            return a[i];
        }
        void add(const T &t) {
            if(caboose == size) {
                resize();
            }
            a[caboose++] = t;
        }
        void set(int i, const T &t) {
            a[i] = t;
        }
        T remove(int i) {
            int j;
            T tmp;
            tmp = a[i];
            for (j = i + 1; j < caboose; j++) {
                a[j - 1] = a[j];
            }
            caboose--;
            return tmp;
        }
        T &get(int i) {
            return a[i];
        }
        std::ostream &print(std::ostream &o) const {
            int i;
            o << "darray[" << std::endl;
            for(i = 0; i < caboose; i++) {
                o << " " << a[i] <<  "," << std::endl;
            }
            o << "]" << std::endl;
            return o;
        }
};

template <class T>
std::ostream &operator<<(std::ostream &o, const darray<T> &a) {
    return a.print(o);
}
#endif