#ifndef TYPES_H
#define TYPES_H

#include "ps2_pointer.h"
#include "imgui.h"

typedef unsigned char u8;
typedef volatile u8 vu8;
typedef signed char s8;
typedef volatile s8 vs8;

typedef unsigned short u16;
typedef volatile u16 vu16;
typedef signed short s16;
typedef volatile s16 vs16;

typedef unsigned int u32;
typedef volatile u32 vu32;
typedef signed int s32;
typedef volatile s32 vs32;

typedef unsigned long long u64;
typedef volatile u64 vu64;
typedef signed long long s64;
typedef volatile s64 vs64;

void drawBool(KlonoaMemory* mem, const char* name, bool* ptr) {
    if (ImGui::Checkbox(name, ptr)) {
        mem->write<char>((char*)ptr - mem->ps2_ram, *ptr);
    }
}

void drawFloat(KlonoaMemory* mem, const char* name, float* ptr, float min, float max) {
    if (ImGui::SliderFloat(name, ptr, min, max)) {
        mem->write<float>((char*)ptr - mem->ps2_ram, *ptr);
    }
}

void drawFloat2(KlonoaMemory* mem, const char* name, float* ptr, float min, float max) {
    if (ImGui::SliderFloat2(name, ptr, min, max)) {
        mem->write<ulong>((char*)ptr - mem->ps2_ram, *(ulong *)ptr);
    }
}

typedef struct FVECTOR {
	float x;
	float y;
	float z;
	float w;

    void draw(KlonoaMemory* mem, const char* name, float min, float max) {
        if (ImGui::SliderFloat4(name, (float *)this, min, max)) {
            mem->write_obj<FVECTOR>((char*)this - mem->ps2_ram);
        }
    }

    void drawColor(KlonoaMemory* mem, const char* name) {
        if (ImGui::ColorEdit3(name, (float *)this)) {
            mem->write_obj<FVECTOR>((char*)this - mem->ps2_ram);
        }
    }
} __attribute__((aligned(16)));

typedef struct {
	int	x;
	int	y;
	int	z;
	int	w;
} IVECTOR __attribute__((aligned(16)));

typedef struct {
	short	x;
	short	y;
	short	z;
	short	w;
} SVECTOR __attribute__((aligned(16)));

typedef struct {
	FVECTOR m[4];
} FMATRIX __attribute__((aligned(16)));

typedef struct {} GeneralWork;
typedef struct {} Prim;

// Size: 0xF0
template <class P = Prim, class W = GeneralWork>
struct OBJWORK {
    u32 pers; // 0x00
    u32 draw; // 0x04
    u32 drmir; // 0x08
    u32 drmiref; // 0x0C
    u32 dreff; // 0x10
    PS2Pointer<P> prim; // 0x14, klMODEL, 
    PS2Pointer<W> work; // 0x18
    s16 stat0; // 0x1C
    s16 stat1; // 0x1E
    s16 prty; // 0x20
    s16 hitram; // 0x22
    s16 bun0; // 0x24
    s16 bun1; // 0x26
    s16 pad0; // 0x28
    s16 pad1; // 0x2A
    s16 live; // 0x2C
    s16 stat; // 0x2E
    FVECTOR posi; // 0x30
    FVECTOR spd; // 0x40
    FVECTOR muki; // 0x50
    FVECTOR ang; // 0x60
    FVECTOR rot; // 0x70
    FVECTOR rtw; // 0x80
    s32 reg0; // 0x90
    s32 reg1; // 0x94
    s32 reg2; // 0x98
    s32 reg3; // 0x9C
    s32 reg4; // 0xA0
    s32 reg5; // 0xA4
    s32 reg6; // 0xA8
    s32 reg7; // 0xAC
    float freg0; // 0xB0
    float freg1; // 0xB4
    float freg2; // 0xB8
    float freg3; // 0xBC
    float freg4; // 0xC0
    float freg5; // 0xC4
    float freg6; // 0xC8
    float freg7; // 0xCC
    s64 htblid; // 0xD0
    s64 dummy; // 0xD8
    s16 option; // 0xE0
    s16 sflag; // 0xE2
    u32 hpo; // 0xE4
    u32 gimmick; // 0xE8
    u32 _hOp; // 0xEC

    void drawObj(KlonoaMemory* mem) {
        posi.draw(mem, "posi", -2000, 2000);
        spd.draw(mem, "spd", -1.0, 1.0);
        muki.draw(mem, "muki", -1.0, 1.0);
        ang.draw(mem, "ang", -1.0, 1.0);
        rot.draw(mem, "rot", -1.0, 1.0);
    }
};

typedef struct {
    FVECTOR Trans; // 0x00
    FVECTOR Rot; // 0x10
    FVECTOR Scale; // 0x20
    u32 pObj; // 0x30
    int ActNum; // 0x34
    int old_muki; // 0x38
    int wip_timer; // 0x3C
    int dummy[4]; // 0x40
} PsfxMODEL;

typedef struct {
    PsfxMODEL klm;
    PsfxMODEL ksm;
    PsfxMODEL kmm;
    PsfxMODEL shm;
    PsfxMODEL shm2;
} klMODEL;

typedef struct {
    FVECTOR trans;
    FVECTOR rot;
    FVECTOR light0;
    FVECTOR light1;
    FVECTOR light2;
    FVECTOR color0;
    FVECTOR color1;
    FVECTOR color2;
    FVECTOR ambient;
    FVECTOR fogcol;
    bool draw_enable;
    bool prim;
    bool lod;
    bool mmesh;
    bool basetex;
    bool fog;
    bool multitex;
    bool headtex;
    bool headtexval;
    bool heightajust;
    bool basetexarea;
    u8 pad;
    float planesizex;
    float planesizez;
    float meshsizex;
    float meshsizez;
    float height;
    float intervalx;
    float intervalz;
    float radiusx;
    float radiusz;
    float texajust;
    float speedx;
    float speedz;
    float shear;
    float basetexspeedx;
    float basetexspeedz;
    float multitexspeedx;
    float multitexspeedz;
    float random;
    float ajust_h_far;
    float ajust_h_near;
    float ajust_h_val;
    float alphablend;
    u8 padding[12];

    void draw(KlonoaMemory* mem) {
        trans.draw(mem, "trans", -2500, 2500);
        rot.draw(mem, "rot", -3.14, 3.14);
        light0.draw(mem, "light0", -2.0, 2.0);
        light1.draw(mem, "light1", -2.0, 2.0);
        light2.draw(mem, "light2", -2.0, 2.0);

        color0.drawColor(mem, "color0");
        color1.drawColor(mem, "color1");
        color2.drawColor(mem, "color2");
        ambient.drawColor(mem, "ambient");
        fogcol.drawColor(mem, "fogcol");

        drawFloat2(mem, "planesize", &planesizex, 0, 5000);
        drawFloat2(mem, "meshsize", &meshsizex, 0, 5000);
        drawFloat2(mem, "interval", &intervalx, 0, 1000);
        drawFloat2(mem, "radius", &radiusx, 0, 1000);
        drawFloat2(mem, "speed", &speedx, 0, 1000);
        drawFloat2(mem, "basetexspeed", &basetexspeedx, 0, 100);
        drawFloat2(mem, "multitexspeed", &multitexspeedx, 0, 100);

        drawFloat(mem, "height", &height, 0, 5);
        drawFloat(mem, "texajust", &texajust, 0, 10);
        drawFloat(mem, "shear", &shear, 0, 1);
        drawFloat(mem, "random", &random, 0, 1);
        drawFloat(mem, "ajust_h_far", &ajust_h_far, 0, 10000);
        drawFloat(mem, "ajust_h_near", &ajust_h_near, 0, 10000);
        drawFloat(mem, "ajust_h_val", &ajust_h_val, 0, 10000);
        drawFloat(mem, "alphablend", &alphablend, 0, 1);

        drawBool(mem, "draw_enable", &draw_enable);
        drawBool(mem, "prim", &prim);
        drawBool(mem, "lod", &lod);
        drawBool(mem, "mmesh", &mmesh);
        drawBool(mem, "basetex", &basetex);
        drawBool(mem, "fog", &fog);
        drawBool(mem, "multitex", &multitex);
        drawBool(mem, "headtex", &headtex);
        drawBool(mem, "headtexval", &headtexval);
        drawBool(mem, "heightajust", &heightajust);
        drawBool(mem, "basetexarea", &basetexarea);
    }
} kitWaveParam;

#endif