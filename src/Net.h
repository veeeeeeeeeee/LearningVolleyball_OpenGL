#ifndef NET_H
#define NET_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Net {
public:
	// top right, top left, bottom right, bottom left
	vec2 tr, tl, br, bl;
	float w, h;

	Net();

	void render();
};

#endif