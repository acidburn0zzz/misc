#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

class String {
    private:
        char *str;
        void copy(char *dest, const char *orig, int n=0);
        int length(const char *s);
        bool compare(const char *s);

    public:
        String();
        String(const char *s);
        String(const String &s);
        ~String();

        String operator=(const char *s);
        String operator=(const String &s);
        String operator+(const char *s);
        String operator+(const String &s);
        String operator+=(const char *s);
        String operator+=(const String &s);
        bool operator==(const char *s);
        bool operator==(const String &s);
        char operator[](int n);

        char *getString();
        void setString(const char *s);
        void setString(const String &s);

        char charAt(int n);
        void clear();
        int length();
        void append(const char *s);
        void append(const String &s);
        bool equals(const char *s);
        bool equals(const String &s);
        String substr(int begin);
        String substr(int begin, int len);
};

#endif //__STRING_H__
