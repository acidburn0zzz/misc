#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

class String {
    private:
        char *str;
        int len;
        void copy(char *dest, const char *orig, int n=0);
    
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
        char operator[](int n);
        
        char *getString();
        void setString(const char *s);
        void setString(const String &s);
        
        char at(int n);
        void clear();
        int length();
        int length(const char *s);
        void append(const char *s);
        void append(const String &s);
};

#endif //__STRING_H__
