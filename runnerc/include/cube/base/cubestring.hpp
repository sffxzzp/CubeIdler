#ifndef cubestring_h__
#define cubestring_h__

#ifdef _WIN32
#pragma once
#endif


#include <cassert>
#include <cstring>

#ifdef __GNUC__

    // Linux
    #define CUBE_CUSTOM_ALLOC 0

    #if __cplusplus >= 201103L
        #define CUBE_USE_CPLUSPLUS11 1
    #else
        #define CUBE_USE_CPLUSPLUS11 0
    #endif
#else
    // Windows

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #define CUBE_CUSTOM_ALLOC 1

    #if CUBE_CUSTOM_ALLOC
        #include <new>
    #endif

    #if _MSC_VER >= 1800L
        #define CUBE_USE_CPLUSPLUS11 1
    #else
        #define CUBE_USE_CPLUSPLUS11 0
    #endif
#endif


#pragma push_macro("new")
#undef new

#include "cube_types.h"

namespace Cube {
#pragma pack(push, 1)


class CubeString {
public:
    typedef char ch_t;
    typedef ch_t* iterator;
    typedef const ch_t* const_iterator;

    CubeString() {
        init();
    }

    CubeString(const CubeString& rhs) {
        init();
        copy(rhs.buff, rhs.str_length);
    }

    CubeString(const ch_t* rhs) {
        init();
        if (rhs) {
            size_t len = strlen(rhs);
            if (len > 0 && rhs != buff) {
                copy(rhs, len);
            }
        }
    }

#if CUBE_USE_CPLUSPLUS11
    CubeString(CubeString&& rhs)
        : buff(rhs.buff),
        buff_size(rhs.buff_size),
        str_length(rhs.str_length) {

        rhs.init();
    }
#endif

    CubeString(const ch_t* buf, size_t n) {
        init();
        copy(buf, n);
    }

    CubeString& operator=(const CubeString& rhs) {
        if (&rhs != this) {
            copy(rhs.c_str(), rhs.size());
        }
        return *this;
    }

    CubeString& operator=(const ch_t* rhs) {
        if (rhs != data()) {
            copy(rhs, strlen(rhs));
        }
        return *this;
    }

#if CUBE_USE_CPLUSPLUS11
    CubeString &operator=(CubeString&& rhs) {
        if (&rhs == this) {
            return *this;
        }
        buff = rhs.buff;
        buff_size = rhs.buff_size;
        str_length = rhs.str_length;

        rhs.init();
        return *this;
    }
#endif

    template<size_t N>
    CubeString &operator=(ch_t(&rhs)[N]) {
        init();
        copy(rhs, N);
        return *this;
    }

    ~CubeString() {
        destory();
    }
    
    bool empty() const {
        return length() == 0;
    }

    void assign(const ch_t* buf, size_t n) {
        copy(buf, n);
    }

    size_t length() const {
        return str_length;
    }

    size_t size() const {
        return str_length;
    }

    void resize(size_t n) {
        _reserve(n, true);
        str_length = n;
    }

    const ch_t* c_str() const {
        return buff;
    }

    const char* data() const {
        return buff;
    }

    void push_back(const ch_t& ch) {
        assert(str_length <= buff_size);
        _reserve(str_length, true);
        buff[str_length++] = ch;
        buff[length()] = '\0';
    }

    void pop_back(const ch_t& ch) {
        assert(str_length <= buff_size);
        buff[--str_length] = '\0';
    }

    void clear() {
        size_t s = buff_size;
        init();
        buff_size = s;
    }

    void erase(size_t index) {
        if (index >= str_length) {
            return;
        }

        if (index == str_length - 1) {
            --str_length;
            buff[str_length] = '\0';
            return;
        }

        for (size_t i = index; i < str_length - 1; ++i) {
            buff[i] = buff[i + 1];
        }
        --str_length;
        buff[str_length] = '\0';
    }

    CubeString& append(const CubeString& rhs) {
        return append(rhs.c_str(), rhs.length());
    }

    CubeString& append(const ch_t* rhs, size_t len) {
        size_t new_length = length() + len;
        _reserve(new_length, true);

        for (size_t i = 0, j = length(); j < new_length; i++, j++) {
            buff[j] = rhs[i];
        }
        buff[new_length] = '\0';
        str_length = new_length;
        return *this;
    }

    int32_t compare(const CubeString& rhs) const {
        return compare(rhs.c_str(), rhs.length());
    }

    int32_t compare(const ch_t* rhs) const {
        return compare(rhs, strlen(rhs));
    }

    int32_t compare(const ch_t* rhs, size_t rhs_compare_len) const {
        if (!data() && !rhs) {
            return 0;
        }
        if (!data()) {
            return -1;
        }
        if (!rhs) {
            return 1;
        }

        size_t len = length();
        if (len > rhs_compare_len) {
            len = rhs_compare_len;
        }
        int32_t result = memcmp(data(), rhs, len);
        if (result == 0) {
            result = static_cast<int32_t>(length()) - static_cast<int32_t>(rhs_compare_len);
        }
        return result;
    }

    void reserve(size_t n) {
        _reserve(n, true);
    }
    
    iterator begin() {
        if (buff) {
            return &buff[0];
        }
        return NULL;
    }

    const_iterator begin() const {
        if (buff) {
            return &buff[0];
        }
        return NULL;
    }

    iterator end() {
        if (buff && buff_size > 0) {
            return &buff[length() - 1];
        }
        return NULL;
    }
    const_iterator end() const {
        if (buff && buff_size > 0) {
            return &buff[length() - 1];
        }
        return NULL;
    }

    ch_t operator[](size_t index) {
        assert(index < str_length);
        return buff[index];
    }

    const ch_t operator[](size_t index) const {
        assert(index < str_length);
        return buff[index];
    }

    CubeString& operator+=(const CubeString& rhs) {
        return append(rhs.c_str(), rhs.length());
    }

    CubeString& operator+=(const ch_t* rhs) {
        return append(rhs, strlen(rhs));
    }

private:
    void _reserve(size_t n, bool is_copy_old_str) {
        auto new_size = n + 1;
        if (new_size <= buff_size) {
            return;
        }
        if (new_size < 32) {
            n = 32;
        }

        ch_t* old_buff = buff;
        size_t old_capacity = buff_size;
        buff_size = n > 1024 * 1024 ? n + 1024 * 1024 : 2 * n;
        buff = new_string(buff_size);

        if (is_copy_old_str && old_buff) {
            size_t i = 0;
            for (; i < str_length; ++i) {
                buff[i] = old_buff[i];
            }
            buff[i] = '\0';
        } else {
            str_length = 0;
        }

        if (old_buff) {
            free_buff(old_buff, &old_capacity);
        }
    }

    void copy(const ch_t* hs, size_t n) {
        if (!hs || n == 0) {
            clear();
            return;
        }

        _reserve(n, false);
        str_length = n;
        size_t i = 0;
        for (; i < n; ++i) {
            buff[i] = hs[i];
        }
        buff[i] = '\0';
    }

    void init() {        
        str_length = 0;
        buff_size = 16;
        buff = new_string(buff_size);
    }

    void destory() {
        free_buff(buff, &buff_size);
        str_length = 0;
    }

    ch_t* new_string(size_t n) {
        if (n == 0) {
            return NULL;
        }
#if CUBE_CUSTOM_ALLOC
        // Windows
        size_t bytes = sizeof(ch_t) * n;
        ch_t* value = reinterpret_cast<ch_t*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes));
        for (size_t i = 0; i < n; ++i) {
            new (&value[i]) ch_t();
        }
        return value;
#else
        // Linux
        ch_t* value = new ch_t[n];
        return value;
#endif
    }

    void free_buff(ch_t*& buf, size_t* buf_size) {
#if CUBE_CUSTOM_ALLOC
        // Windows
        if (*buf_size == 0) {
            return;
        }
        for (size_t i = 0; i < *buf_size; ++i) {
            buf[i].~ch_t();
        }

        HeapFree(GetProcessHeap(), 0, buf);

        buf = NULL;
        *buf_size = 0;
#else
        // Linux
        delete[] buf;
        buf = NULL;
        *buf_size = 0;
#endif
    }

private:
    ch_t* buff;
    size_t buff_size;
    size_t str_length;
    enum { k };
};

inline
bool operator==(const CubeString& lhs, const CubeString& rhs) {
    return lhs.compare(rhs) == 0 ? true : false;
}
inline
bool operator==(const CubeString& lhs, const CubeString::ch_t* rhs) {
    return lhs.compare(rhs) == 0 ? true : false;
}
inline
bool operator==(const CubeString::ch_t*, const CubeString& rhs) {
    return rhs.compare(rhs) == 0 ? true : false;
}


inline
bool operator!=(const CubeString& lhs, const CubeString& rhs) {
    return lhs.compare(rhs) != 0 ? true : false;
}
inline
bool operator!=(const CubeString& lhs, const CubeString::ch_t* rhs) {
    return lhs.compare(rhs) != 0 ? true : false;
}
inline
bool operator!=(const CubeString::ch_t*, const CubeString& rhs) {
    return rhs.compare(rhs) != 0 ? true : false;
}


inline
bool operator>(const CubeString& lhs, const CubeString& rhs) {
    return lhs.compare(rhs) > 0 ? true : false;
}
inline
bool operator>(const CubeString& lhs, const CubeString::ch_t* rhs) {
    return lhs.compare(rhs) > 0 ? true : false;
}
inline
bool operator>(const CubeString::ch_t*, const CubeString& rhs) {
    return rhs.compare(rhs) > 0 ? true : false;
}

inline
bool operator<(const CubeString& lhs, const CubeString& rhs) {
    return lhs.compare(rhs) < 0 ? true : false;
}
inline
bool operator<(const CubeString& lhs, const CubeString::ch_t* rhs) {
    return lhs.compare(rhs) < 0 ? true : false;
}
inline
bool operator<(const CubeString::ch_t*, const CubeString& rhs) {
    return rhs.compare(rhs) < 0 ? true : false;
}

inline
CubeString operator+(const CubeString& lhs, const CubeString& rhs) {
    CubeString str = lhs;
    str += rhs;
    return str;
}
inline
CubeString operator+(const CubeString& lhs, const CubeString::ch_t* rhs) {
    CubeString str = lhs;
    str += rhs;
    return str;
}
inline
CubeString operator+(const CubeString::ch_t* lhs, const CubeString& rhs) {
    CubeString str = lhs;
    str += rhs;
    return str;
}

#pragma pack(pop)
}

#pragma pop_macro("new")
#endif // cubestring_h__

