/**
 * @file  string.cc
 * @brief String
 */

#include <stdio.h>
#include <locale.h>

#include "string.h"

namespace ktn {

String::String(const TCHAR* s) {
  int length = 0;
  while (*s != _T('\0')) {
    ++length;
    ++s;
  }
  string_ = new TCHAR[length + 1];
  if (!string_) { length_ = 0; return; }

  length_ = length;
  s -= length;
  for (int i = 0; i < length + 1; ++i) {
    string_[i] = *s;
    ++s;
  }
}

String::String(const TCHAR* s, int length) {
  string_ = new TCHAR[length + 1];
  if (!string_) { length_ = 0; return; }

  length_ = length;
  for (int i = 0; i < length + 1; ++i) {
    string_[i] = *s;
    ++s;
  }
}

String::String(const String& s) {
  if (this == &s) { return; }

  string_ = new TCHAR[s.length_ + 1];
  if (!string_) { length_ = 0; return; }

  length_ = s.length_;

  for (int i = 0; i < length_ + 1; ++i) {
    string_[i] = s.string_[i];
  }
}

String& String::operator=(const String& s) {
  if (this == &s) { return *this; }

  if (length_ < s.length_) {
    delete [] string_;
    string_ = new TCHAR[s.length_ + 1];
    if (!string_) { length_ = 0; return *this; }
    length_ = s.length_;
  }
  for (int i = 0; i < length_ + 1; ++i) {
    string_[i] = s.string_[i];
  }

  return *this;
}

void String::out() const {
  if (length_ == 0 || string_ == nullptr) {
    puts("");
    return;
  }
  _tprintf(_T("%s\n"), string_);
}

std::_tostream& operator<<(std::_tostream& os, const String& s) {
  return os << s.string_;
}
/*
std::_tistream& operator<<(std::_tistream& is, String& s) {
  return is;
}*/

TCHAR& String::operator[](int index) {
  if (index < 0) {
    index += length_;
  }
  if (index > length_) {
    fprintf(stderr, "Warn: index is too large. %s:%d:\n", __FILE__, __LINE__);
    index = length_ - 1;
  } else if (index < 0) {
    fprintf(stderr, "Warn: index is too small. %s:%d:\n", __FILE__, __LINE__);
    index = 0;
  }
  return string_[index];
}

bool String::operator!() const {
  return length_ == 0 || !string_;
}

String String::operator-() const {
  TCHAR* temp = new TCHAR[length_ + 1];
  if (!temp) { return String(); }

  int i = 0;
  for (; i < length_; ++i) {
    temp[i] = string_[length_ - 1 - i];
  }
  temp[i] = _T('\0');

  String res(temp, length_);
  delete [] temp;
  return res;
}

int String::indexOf(const TCHAR c, int from) const {
  int index = -1;
  for (int i = from; i < length_; ++i) {
    if (string_[i] == c) {
      index = i;
      break;
    }
  }

  return index;
}

int String::lastIndexOf(const TCHAR c, int from) const {
  int index = -1;
  if (from < 0) { from = 0; }
  else if (from > length_ - 1) { from = length_ - 1; }
  for (int i = from; i; --i) {
    if (string_[i] == c) {
      index = i;
      break;
    }
  }

  return index;
}

String String::toUpperCase() {
  TCHAR* w = new TCHAR[length_ + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length_ + 1; ++i) {
    w[i] = towupper(string_[i]);
  }

  String res(w, length_);
  delete [] w;
  return res;
}

String String::toLowerCase() {
  TCHAR* w = new TCHAR[length_ + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length_ + 1; ++i) {
    w[i] = towlower(string_[i]);
  }

  String res(w, length_);
  delete [] w;
  return res;
}

String String::substr(int from, int length) {
  if (length < 1) { return String(); }
  else if (length > length_) { return *this; }
  if (from < 0) { from = 0; }
  if (from + length > length_) {
    length = length_ - from;
  }
  TCHAR* w = new TCHAR[length + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length; ++i) {
    w[i] = string_[from + i];
  }
  w[length] = _T('\0');

  String res(w, length);
  delete [] w;
  return res;
}

String String::slice(int from, int to) {
  from %= length_;
  to %= length_;
  if (from < 0) { from += length_; }
  if (to < 0) { to += length_; }
  int length = to - from + 1;
  if (length <= 0) { return String(); }
  TCHAR* w = new TCHAR[length + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length; ++i) {
    w[i] = string_[from + i];
  }
  w[length] = _T('\0');

  String res(w, length);
  delete [] w;
  return res;
}

String String::trimLeft() {
  int i = 0;
  for (; i < length_; ++i) {
    if (iswspace(string_[i]) == 0) {
      break;
    }
  }
  int length = length_ - i;
  TCHAR* temp = new TCHAR[length + 1];
  if (!temp) { return String(); }

  for (int j = 0; i < length_ + 1; ++i, ++j) {
    temp[j] = string_[i];
  }

  String res(temp, length);
  delete [] temp;
  return res;
}

String String::trimRight() {
  if (length_ == 0) { return String(); }

  int i = length_ - 1;
  for (; i; --i) {
    if (iswspace(string_[i]) == 0) {
      break;
    }
  }
  int length = i + 1;
  TCHAR* temp = new TCHAR[length + 1];
  if (!temp) { return String(); }
  int j = 0;
  for (i = 0; i < length; ++i, ++j) {
    temp[j] = string_[i];
  }
  temp[j] = _T('\0');

  String res(temp, length);
  delete [] temp;
  return res;
}

String String::trim() {
  if (length_ == 0) { return String(); }

  int first = 0;
  for (; first < length_; ++first) {
    if (iswspace(string_[first]) == 0) {
      break;
    }
  }
  int last = length_ - 1;
  for (; last; --last) {
    if (iswspace(string_[last]) == 0) {
      break;
    }
  }

  int length = last - first + 1;
  TCHAR* temp = new TCHAR[length + 1];
  if (!temp) { return String(); }
  
  int j = 0;
  for (int i = first; i < last + 1; ++i, ++j) {
    temp[j] = string_[i];
  }
  temp[j] = _T('\0');

  String res(temp, length);
  delete [] temp;
  return res;
}

String String::operator+(const String& s) const {
  TCHAR* w = new TCHAR[length_ + s.length_ + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length_; ++i) {
    w[i] = string_[i];
  }
  for (int i = 0; i < s.length_ + 1; ++i) {
    w[i + length_] = s.string_[i];
  }

  String res(w, length_ + s.length_);
  delete [] w;
  return res;
}

String String::operator-(const TCHAR c) const {
  TCHAR* temp = new TCHAR[length_ + 1];
  if (!temp) { return String(); }

  int length = 0;
  for (int i = 0; i < length_; ++i) {
    if (string_[i] != c) {
      temp[length] = string_[i];
      ++length;
    }
  }
  temp[length] = _T('\0');

  String res(temp, length);
  delete [] temp;
  return res;
}

String String::operator*(int times) const {
  if (times == 0) {
    return String();
  } else if (times < 0) {
    times = -times;

    int length = length_ * times;
    TCHAR* w = new TCHAR[length + 1];
    if (!w) { return String(); }

    for (int i = 0; i < length; ++i) {
      w[i] = string_[length_ - 1 - i % length_];
    }
    w[length] = _T('\0');

    String res(w, length);
    delete [] w;
    return res;
  }

  int length = length_ * times;
  TCHAR* w = new TCHAR[length + 1];
  if (!w) { return String(); }

  for (int i = 0; i < length; ++i) {
    w[i] = string_[i % length_];
  }
  w[length] = _T('\0');

  String res(w, length);
  delete [] w;
  return res;
}

String& String::operator+=(const String& b) {
  *this = *this + b;
  return *this;
}

String& String::operator-=(const TCHAR c) {
  *this = *this - c;
  return *this;
}

String& String::operator*=(int times) {
  *this = *this * times;
  return *this;
}

bool String::operator==(const String& s) const {
  if (this == &s) { return true; }
  if (length_ != s.length_) { return false; }
  for (int i = 0; i < length_; ++i) {
    if (string_[i] != s.string_[i]) { return false; }
  }
  return true;
}

bool String::operator!=(const String& s) const {
  if (this == &s) { return false; }
  if (length_ != s.length_) { return true; }
  for (int i = 0; i < length_; ++i) {
    if (string_[i] != s.string_[i]) { return true; }
  }
  return false;
}

} // namespace ktn
