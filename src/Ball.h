#ifndef BALL_H
#define BALL_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "Net.h"

class Ball {
public:
	Ball();
	Ball(vec2 _pos);

	vec2 pos;
	vec2 vel;

	float gravity;
	float radius;
	float maxSpd;
	float ground;

	Ball(vec2 _pos, float _gravity, float _radius);
	void iniVel();
	void reset();
	void collideNet(Net n);
	bool collideCircle(vec2 incVel, vec2 c, float r);
	void bounceLeft(float x);
	void bounceRight(float x);

	void update();
	void render();
};

#endif