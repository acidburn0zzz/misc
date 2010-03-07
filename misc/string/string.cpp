#include "string.h"

String::String() {
    str = NULL;
    setString("");
}

String::String(const char *s) {
    str = NULL;
    setString(s);
}

String::String(const String &s) {
    str = NULL;
    setString(s.str);
}

String::~String() {
    delete [] str;
}

void String::copy(char *dest, const char *orig, int n) {
    for (int i=0; i<n; i++)
        dest[i] = orig[i];
    dest[n] = '\0';
}

bool String::compare(const char *s) {
    int n = length(str);

    if (length(s) != n)
        return false;

    for (int i=0; i<n; i++)
        if (str[i] != s[i])
            return false;

    return true;
}

String String::operator=(const char *s) {
    setString(s);

    return *this;
}

String String::operator=(const String &s) {
    setString(s);

    return *this;
}

String String::operator+(const char *s) {
    append(s);

    return *this;
}

String String::operator+(const String &s) {
    append(s);

    return *this;
}

String String::operator+=(const char *s) {
    append(s);

    return *this;
}

String String::operator+=(const String &s) {
    append(s);

    return *this;
}

bool String::operator==(const char *s) {
    return compare(s);
}

bool String::operator==(const String &s) {
    return compare(s.str);
}

char String::operator[](int n) {
    return charAt(n);
}

char *String::getString() {
    return str;
}

void String::setString(const char *s) {
    if (str)
        delete [] str;

    int len = length(s);
    str = new char[len + 1];
    copy(str, s, len);
}

void String::setString(const String &s) {
    setString(s.str);
}

char String::charAt(int n) {
    if (n >= length())
        return 0; //throw out_of_bounds

    return *(str + n);
}

void String::clear() {
    setString("");
}

int String::length() {
    return length(str);
}

int String::length(const char *s) {
    int i=0;

    for (const char *p = s; *p++ != '\0'; i++) ;

    return i;
}

void String::append(const char *s) {
    char *oldStr = str;
    int lenA = length(str);
    int lenB = length(s);

    str = new char[lenA + lenB + 1];

    if (oldStr) {
        copy(str, oldStr, lenA);
        delete oldStr;
    }

    copy(str + lenA, s, lenB);
}

void String::append(const String &s) {
    append(s.str);
}

bool String::equals(const char *s) {
    return compare(s);
}

bool String::equals(const String &s) {
    return compare(s.str);
}

String String::substr(int begin) {
    int len = length();

    if (begin >= len)
        return String("out_of_bounds"); //throw out_of_bounds

    return substr(begin, length() - begin);
}

String String::substr(int begin, int len) {
    char *new_str;

    if ((begin + len) > length())
        return String("out_of_bounds"); //throw out_of_bounds

    new_str = new char[len + 1];

    copy(new_str, str+sizeof(char)*begin, len);
    new_str[len] = '\0';

    return String(new_str);
}
