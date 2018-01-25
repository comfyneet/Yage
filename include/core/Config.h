#ifndef YAGE_CORE_CONFIG_H
#define YAGE_CORE_CONFIG_H

#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <array>
#include <chrono>
#include <codecvt>
#include <exception>
#include <fstream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <thread>

#ifdef _MSC_VER
#  define YE_DLLEXPORT __declspec(dllexport)
#  define YE_DLLIMPORT __declspec(dllimport)
#  pragma warning(disable: 4251) // class 'X' needs to have dll-interface to be used by clients of class 'Y'
#else
#   error("Unknown compiler")
#endif

#ifdef _YE_BUILD_DLL
#  define YE_API          YE_DLLEXPORT
#  define YE_TEMPLATE_API YE_DLLEXPORT
#  define YE_EXTERN
#elif defined(YE_DLL)
#  define YE_API          YE_DLLIMPORT
#  define YE_TEMPLATE_API
#  define YE_EXTERN       extern
#else
#  define YE_API
#  define YE_TEMPLATE_API
#  define YE_EXTERN
#endif

#endif
