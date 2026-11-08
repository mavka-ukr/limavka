#ifndef TSIL_H
#define TSIL_H

#include "./numbertypes.h"

typedef bool \u043b\u043e\u0433\u0456\u0447\u043d\u0435;

typedef uint8_t \u043f8;
typedef uint16_t \u043f16;
typedef uint32_t \u043f32;
typedef uint64_t \u043f64;
typedef int8_t \u04468;
typedef int16_t \u044616;
typedef int32_t \u044632;
typedef int64_t \u044664;
typedef float \u044032;
typedef double \u044064;

typedef size_t \u043f\u0440\u0438\u0440\u043e\u0434\u043d\u0435;
typedef ssize_t \u0446\u0456\u043b\u0435;

typedef void \u043d\u0456\u0449\u043e;
typedef void* \u0430\u0434\u0440\u0435\u0441\u0430;

typedef struct \u044e8 \u044e8;
typedef struct \u043a\u0434 \u043a\u0434;

struct \u044e8 {
  \u043f8* \u0434\u0430\u043d\u0456;
  \u043f\u0440\u0438\u0440\u043e\u0434\u043d\u0435 \u0440\u043e\u0437\u043c\u0456\u0440;
};

struct \u043a\u0434 {
  \u043f8* \u0434\u0430\u043d\u0456;
  \u043f\u0440\u0438\u0440\u043e\u0434\u043d\u0435 \u0440\u043e\u0437\u043c\u0456\u0440;
};

static const \u043b\u043e\u0433\u0456\u0447\u043d\u0435 \u043d\u0456 = 0;
static const \u043b\u043e\u0433\u0456\u0447\u043d\u0435 \u0442\u0430\u043a = 1;
static const \u0430\u0434\u0440\u0435\u0441\u0430 \u043f\u0443\u0441\u0442\u043e = 0;

#endif // TSIL_H