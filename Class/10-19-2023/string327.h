#ifndef STRING327_H
#define STRING327_H

#include <iostream>

class string327 {
    private:
        char *str;
    public:
        string327(); // Empty constructor
        string327(const char *s); // take a string constructor
        string327(const string327 &s); // copy constructor
        ~string327();  // Destroy function
        int length();  // length of string
        bool operator < (const string327 &rhs);
        bool operator > (const string327 &rhs);
        bool operator == (const string327 &rhs);
        bool operator != (const string327 &rhs);
        bool operator <= (const string327 &rhs);
        bool operator >= (const string327 &rhs);
        string327 operator+(const string327 &rhs);
        string327 operator+(const char *rhs);
        string327 &operator+=(const string327 &rhs);
        string327 &operator+=(const char *rhs);
        string327 &operator=(const string327 &rhs);
        string327 &operator=(const char *rhs);
        char &operator[](const int i);
        const char *c_str();

        // Friends have accesss to the privates
        friend std::istream &operator>>(std::istream &lhs, const string327 &rhs);
};

// string327 s, t;
// cout << s = t << endl;
// foo(s += t);
// cout << s[i] << endl;
// s[i] = 'h';
// cin >> s >> t >> endl
// cin.operator>>(s).operator>>(t)>>(endl)
// std::istream &operator>>(std::istream &lhs, const string327 &rhs);
// std::ostream &operator>>(std::ostream &lhs, const string327 &rhs);
// cerr <<
// class string327 foo;
#endif