#include <libvu0.h>
#include "types.h"

template <class T>
class CVector {
public:
    union {
        T x;
        T r;
        T s;
        T u;
    };
    union {
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
        T w;
        T a;
    };

    CVector(T x, T y, T z, T w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

template <class T>
class CMatrix {
private:
    CVector<T> m_matrix[4];
};

typedef CVector<s32> CIVector;
typedef CVector<f32> CFVector;
typedef CVector<f32> CFMatrix;

class CObject {

public:
    static const s32 STAT1 = -1;
};

class CObjSonicA {
protected:
    /* 0x00 */ sceVu0IVECTOR m_color;
    /* 0x10 */ f32 m_zoom;

public:
    static const s32 STAT1 = 11;

};

class CObjEbWaveA : public CObjSonicA { // 0x70

protected:
    /* 0x20 */ s16 m_texture;
    /* 0x24 */ f32 m_width;
    /* 0x28 */ sceVu0FVECTOR* m_posi;
    /* 0x2c */ sceVu0FMATRIX* m_camrev;
    /* 0x30 */ sceVu0FMATRIX m_direction;

public:
    static const s32 STAT1 = 25;

    void Init(sceVu0FVECTOR* posi, sceVu0FMATRIX direction, f32 zoom, sceVu0IVECTOR color, f32 width, s32 tex);
};