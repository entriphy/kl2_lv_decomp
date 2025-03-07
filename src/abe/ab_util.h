#ifndef AB_UTIL_H
#define AB_UTIL_H

#include "abe.h"

#ifdef __cplusplus

template <typename T>
class CVector {
public:
	union {
        T x;
        T r;
        T s;
        T u;
    };

	union{
        T y;
        T g;
        T t;
        T v;
    };

	union {
        T z;
        T b;
        T q;
    };

	union {
        f32 w;
        f32 a;
    };
	
	CVector<T>& operator=(CVector &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

	CVector() {
        x = T();
        y = T();
        z = T();
        w = T();
    }

	CVector(T v0) {
        x = v0;
        y = T();
        z = T();
        w = T();
    }

	CVector(T v0, T v1) {
        x = v0;
        y = v1;
        z = T();
        w = T();
    }

	CVector(T v0, T v1, T v2) {
        x = v0;
        y = v1;
        z = v2;
        w = T();
    }

	CVector(T v0, T v1, T v2, T v3) {
        x = v0;
        y = v1;
        z = v2;
        w = v3;
    }

	CVector<T>& Set(T v0, T v1, T v2, T v3) {
        x = v0;
        y = v1;
        z = v2;
        w = v3;
    }

	T& operator[](s32 index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
        }
    }

	// T* operator float *();
};

template <typename T>
class CMatrix {
private:
	/* 0x00 */ CVector<T> m_matrix[4];
	
public:
	CMatrix<T>& operator=();
	CMatrix();
	CMatrix();
	CVector<T>& operator[]();
	CVector<T>* operator CVector<T> *();
	T*[4] operator T (*)[3]();
};

typedef CVector<s32> CIVector;
typedef CVector<f32> CFVector;
typedef CMatrix<f32> CFMatrix;

#endif

extern s32 MfifoOn;

#endif
