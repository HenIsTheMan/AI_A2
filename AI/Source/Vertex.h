#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

//typedef struct{
//    double h;
//    double s;
//    double v;
//} hsv;

template <class T>
struct HSV final{
    union{
        struct{
            T h, s, v;
        };
        struct{
            T H, S, V; //Nah
        };
    };
};

struct Position final{
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0) {Set(x, y, z);}
	void Set(float x, float y, float z) {this->x = x; this->y = y; this->z = z;}
};

struct Color final{
	float r, g, b;
	Color(float r = 1, float g = 1, float b = 1) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}

    static Color HSVToRGB(const HSV<float> hsv){
        float H = hsv.h;
        const float S = hsv.s;
        const float V = hsv.v;
        float P, Q, T, fract;

        (H == 360.0f) ? (H = 0.0f) : (H /= 60.0f);
        fract = H - floor(H);
        P = V * (1.0f - S);
        Q = V * (1.0f - S * fract);
        T = V * (1.0f - S * (1.0f - fract));

        if(0.0f <= H && H < 1.0f){
            return {V, T, P};
        } else if (1.0f <= H && H < 2.0f){
            return {Q, V, P};
        } else if (2.0f <= H && H < 3.0f){
            return {P, V, T};
        } else if (3.0f <= H && H < 4.0f){
            return {P, Q, V};
        } else if (4.0f <= H && H < 5.0f){
            return {T, P, V};
        } else if (5.0f <= H && H < 6.0f){
            return {V, P, Q};
        } else{
            return {0.0f, 0.0f, 0.0f};
        }
    }
};

struct TexCoord{
	float u, v;
	TexCoord(float u = 0, float v = 0) {Set(u, v);}
	void Set(float u, float v) {this->u = u; this->v = v;}
};

struct Vertex final{
	Position pos;
	Color color;
	Vector3 normal;
	TexCoord texCoord;
};

#endif