#ifndef DARRAY_H
#define DARRAY_H
template <class T>

#define DEFAULT_SIZE = 10

class darray {
    private:
        T *a;
        int size;

        void resize();
    public:
        darray() : a(new T[DEFAULT_SIZE]), size(DEFAULT_SIZE) {};
        darray(int size);
        ~darray();

        T  &operator[](int i);
        void add(const T &t);
        ostream &print(ostream &o);
        void set(int i, const T &t);
        T remove(int i);
        T &get(int i);
}
#endif