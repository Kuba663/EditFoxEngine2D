#ifndef __UTILS_FRAMEWORK_H_
#define __UTILS_FRAMEWORK_H_
#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif
#endif