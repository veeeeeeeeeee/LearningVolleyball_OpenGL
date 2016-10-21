#ifndef AGENT_H
#define AGENT_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "Net.h"

class Agent {
public:
	vec2 pos;
	vec2 oriPos;
	vec2 vel;

	float jumpSpd;
	float moveSpd;
	float ground;
	float radius;
	float gravity;
	int lr;

	bool movingLeft;
	bool movingRight;

	int score;
	Net net;

	Agent();
	Agent(vec2 _pos, float _ground, float _radius, float _gravity, Net _n);
	void moveLeft();
	void moveRight();
	void jump();

	void update();
	void render(bool lr);
};

#endif