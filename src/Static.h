#ifndef STATIC_H
#define STATIC_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static float mag(vec2 v) {
	return sqrt(v.x*v.x + v.y*v.y);
}

static float dot(vec2 u, vec2 v) {
	return u.x*v.x + u.y*v.y;
}

static vec2 normalize(vec2 v) {
	float m = mag(v);
	return v/m;
}

static vec2 random2D() {
	Rand r1, r2;
	vec2 v = vec2(r1.randFloat(), r2.randFloat());

	return normalize(v);
}

#endif