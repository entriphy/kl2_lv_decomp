#include "common.h"

void GetWsMtx() {
    sceVu0CameraMatrix(Scr.WvMtx, Scr.camera_p, Scr.camera_zd, Scr.camera_yd);
    sceVu0ViewScreenMatrix(Scr.VsMtx, Scr.SrcZ, Scr.AspectX, Scr.AspectY, Scr.CenterX, Scr.CenterX, Scr.MinZ, Scr.MaxZ, Scr.NearZ, Scr.FarZ);
    sceVu0MulMatrix(Scr.WsMtx, Scr.VsMtx, Scr.WvMtx);
}

void ScreenInit() {
    Scr.SrcZ = 640.0f;
    Scr.AspectX = 1.0f;
    Scr.AspectY = 0.46f;
    Scr.CenterX = 2048.0f;
    Scr.MinZ = 1.0f;
    Scr.MaxZ = 1048575.0f;
    Scr.NearZ = 1.0f;
    Scr.FarZ = 1048560.0f;
    Scr.camera_p[0] = 0.0f;
    Scr.camera_p[1] = 0.0f;
    Scr.camera_p[2] = 0.0f;
    Scr.camera_p[3] = 1.0f;
    Scr.camera_zd[0] = 0.0f;
    Scr.camera_zd[1] = 0.0f;
    Scr.camera_zd[2] = 1.0f;
    Scr.camera_zd[3] = 1.0f;
    Scr.camera_yd[0] = 0.0f;
    Scr.camera_yd[1] = 1.0f;
    Scr.camera_yd[2] = 0.0f;
    Scr.camera_yd[3] = 1.0f;

    sceVu0CameraMatrix(Scr.WvMtx, Scr.camera_p, Scr.camera_zd, Scr.camera_yd);
    sceVu0ViewScreenMatrix(Scr.VsMtx, Scr.SrcZ, Scr.AspectX, Scr.AspectY, Scr.CenterX, Scr.CenterX, Scr.MinZ, Scr.MaxZ, Scr.NearZ, Scr.FarZ);
    sceVu0MulMatrix(Scr.WsMtx, Scr.VsMtx, Scr.WvMtx);
}

void LightInit() {
    Light3.light0[0] = 0.5f;
    Light3.light0[1] = 0.5f;
    Light3.light0[2] = 0.0f;
    Light3.light0[3] = 0.0f;
    Light3.light1[0] = -0.5f;
    Light3.light1[1] = -0.5f;
    Light3.light1[2] = 0.0f;
    Light3.light1[3] = 0.0f;
    Light3.light2[0] = 0.0f;
    Light3.light2[1] = 0.0f;
    Light3.light2[2] = 0.0f;
    Light3.light2[3] = 0.0f;
    Light3.color0[0] = 0.5f;
    Light3.color0[1] = 0.5f;
    Light3.color0[2] = 0.5f;
    Light3.color0[3] = 0.0f;
    Light3.color1[0] = 0.15f;
    Light3.color1[1] = 0.15f;
    Light3.color1[2] = 0.15f;
    Light3.color1[3] = 0.0f;
    Light3.color2[0] = 0.0f;
    Light3.color2[1] = 0.0f;
    Light3.color2[2] = 0.0f;
    Light3.color2[3] = 0.0f;
    Light3.ambient[0] = 0.2f;
    Light3.ambient[1] = 0.2f;
    Light3.ambient[2] = 0.2f;
    Light3.ambient[3] = 0.0f;

    sceVu0NormalLightMatrix(Light3.NormalLight, Light3.light0, Light3.light1, Light3.light2);
    sceVu0LightColorMatrix(Light3.LightColor, Light3.color0, Light3.color1, Light3.color2, Light3.ambient);
}
