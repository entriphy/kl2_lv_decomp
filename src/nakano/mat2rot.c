#include "nakano/mat2rot.h"
#include "nakano//vu0mac.h"

void InvRotMatrixZYX(sceVu0FVECTOR vec, sceVu0FMATRIX rotm) {
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;
    sceVu0FMATRIX m;
    f32 c;

    sceVu0CopyMatrix(m, rotm);
    v0[0] = 0.0f;
    v0[1] = 0.0f;
    v0[2] = 1.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    vec[0] = -atan2f(v1[1], v1[2]);
    c = vu0_SquareRoot(v1[2] * v1[2] + v1[1] * v1[1]);
    vec[1] = atan2f(v1[0], c);
    v0[0] = 0.0f;
    v0[1] = 1.0f;
    v0[2] = 0.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    v0[0] = -vec[0];
    v0[1] = -vec[1];
    v0[2] = 0.0f;
    v0[3] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, v0[0]);
    sceVu0RotMatrixY(m, m, v0[1]);
    sceVu0ApplyMatrix(v0, m, v1);
    vec[2] = -atan2f(v0[0], v0[1]);
}

void InvRotMatrixYXZ(sceVu0FVECTOR vec, sceVu0FMATRIX rotm) {
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;
    sceVu0FMATRIX m;
    f32 c;

    sceVu0CopyMatrix(m, rotm);
    v0[0] = 0.0f;
    v0[1] = 1.0f;
    v0[2] = 0.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    vec[2] = -atan2f(v1[0], v1[1]);
    c = vu0_SquareRoot(v1[0] * v1[0] + v1[1] * v1[1]);
    vec[0] = atan2f(v1[2], c);
    v0[0] = 1.0f;
    v0[1] = 0.0f;
    v0[2] = 0.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    v0[0] = -vec[0];
    v0[1] = 0.0f;
    v0[2] = -vec[2];
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixZ(m, m, v0[2]);
    sceVu0RotMatrixX(m, m, v0[0]);
    sceVu0ApplyMatrix(v0, m, v1);
    vec[1] = -atan2f(v0[2], v0[0]);
}

void InvRotMatrixZXY(sceVu0FVECTOR vec, sceVu0FMATRIX rotm) {
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;
    sceVu0FMATRIX m;
    f32 c;

    sceVu0CopyMatrix(m, rotm);
    v0[0] = 0.0f;
    v0[1] = 0.0f;
    v0[2] = 1.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    vec[1] = atan2f(v1[0], v1[2]);
    c = vu0_SquareRoot(v1[0] * v1[0] + v1[2] * v1[2]);
    vec[0] = -atan2f(v1[1], c);
    v0[0] = 1.0f;
    v0[1] = 0.0f;
    v0[2] = 0.0f;
    v0[3] = 1.0f;
    sceVu0ApplyMatrix(v1, m, v0);
    v0[0] = -vec[0];
    v0[1] = -vec[1];
    v0[2] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, v0[1]);
    sceVu0RotMatrixX(m, m, v0[0]);
    sceVu0ApplyMatrix(v0, m, v1);
    vec[2] = atan2f(v0[1], v0[0]);
}