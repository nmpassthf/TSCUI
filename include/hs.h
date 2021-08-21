#define __NMP_THF_HEAD_H__ 3  // Modified in 21/05/03
#pragma once
#pragma execution_character_set("utf-8")

// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

#if __cplusplus >= 201103L || _MSVC_LANG >= 201103L
// #include <ccomplex>
#include <cfenv>
#include <cinttypes>
// #include <cstdalign>
// #include <cstdbool>
#include <cstdint>
// #include <ctgmath>
#include <cuchar>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L || _MSVC_LANG >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <codecvt>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#endif

#if __cplusplus >= 201402L || _MSVC_LANG >= 201402L
#include <shared_mutex>
#endif

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#include <any>
#include <charconv>
// #include <execution>
#include <filesystem>
#include <memory_resource>
#include <optional>
#include <string_view>
#include <variant>
#endif

#if __cplusplus > 201703L || _HAS_CXX20
#include <bit>
// #include <compare>
// #include <span>
// #include <syncstream>
#include <version>
#endif

#include <stdlib.h>
#define _INITIALIZER(f)                                        \
    static void f(void);                                       \
                                                               \
    struct __INITIALIZER_##f##_TYPE {                          \
        __INITIALIZER_##f##_TYPE(void) { f(); }                \
    };                                                         \
    static __INITIALIZER_##f##_TYPE __INITIALIZER##f##__CLASS; \
    void f(void)

#define _DESTROYER(f)                                      \
    static void f(void);                                   \
                                                           \
    struct __DESTROYER_##f##_TYPE {                        \
        __DESTROYER_##f##_TYPE(void) { atexit(f); }        \
    };                                                     \
    static __DESTROYER_##f##_TYPE __DESTROYER##f##__CLASS; \
    void f(void)

#define INITIALIZER(FUNC_NAME) _INITIALIZER(FUNC_NAME)
#define DESTROYER(FUNC_NAME) _DESTROYER(FUNC_NAME)

// UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef NMP_THF_UNICODE
#define NMP_THF_UNICODE 1
#endif

#include <fcntl.h>
#include <io.h>
#include <windows.h>

INITIALIZER(SCOPTCP) { SetConsoleOutputCP(65001); }

namespace NMP_THF {

#ifdef NMP_THF_UNICODE

#define WMAIN int wmain(int _argc, wchar_t *_argv[], wchar_t *_envp[])

#define SETMODE_SETCHARSETU16             \
    _setmode(_fileno(stdin), _O_U16TEXT); \
    _setmode(_fileno(stdout), _O_U16TEXT);
// _setmode(_fileno(stderr), _O_U16TEXT);

#define U8INIT                    \
    INITIALIZER(_U8INIT_FUNC) {   \
        system("@cls");           \
        system("chcp 65001>nul"); \
        SetConsoleCP(CP_UTF8);    \
    }

#define UNIMAIN U8INIT int main(int argc, char *argv[])
#define QAPP(name) QApplication name(argc, argv)

// #define wout std::wcout
// #define werr std::wcerr
// #define win std::wcin

using wchar = wchar_t;
using cwchar = const wchar_t;
using wstr = std::wstring;
using cwstr = const std::wstring;

#define cauto const auto

// check no use unU16 out
// #define cout #define _ERROR_
// #define cerr #define _ERROR_
// #define cin #define _ERROR_
// #define fstream #define _ERROR_
// #define ifstream #define _ERROR_
// #define ofstream #define _ERROR_
// #define istream #define _ERROR_
// #define ostream #define _ERROR_
// #define iostream #define _ERROR_
#ifdef _ERROR_
#error "NO UN_U16 IO"
#endif
}

#endif
