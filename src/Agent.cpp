#include "Agent.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Agent::Agent() {
}

Agent::Agent(vec2 _pos, float _ground, float _radius, float _gravity, Net _n) {
	pos = _pos;
	oriPos = pos;
	ground = _ground;
	radius = _radius;
	gravity = _gravity;
	net = _n;
	
	vel = vec2(0, 0);
	movingLeft = movingRight = false;
	
	jumpSpd = 8.0f;
	moveSpd = 5.0f;
	lr = 0;
	
	score = 0;
}

void Agent::moveLeft() {
	lr --;
}

void Agent::moveRight() {
	lr ++;
}

void Agent::jump() {
	if (pos.y >= ground)
		vel.y = -jumpSpd;
}

void Agent::update() {
	lr = 0;
	
	if (movingLeft)
		moveLeft();
	
	if (movingRight)
		moveRight();
	
	vel.x = lr*moveSpd;
	vel.y -= gravity;
	
	pos = pos + vel;
	
	if (pos.y >= ground) {
		pos.y = oriPos.y;
		vel.y = 0;
	}
	
	if (pos.x >= getWindowWidth() - radius)
		pos.x = getWindowWidth() - radius;
	
	if (pos.x <= net.tr.x + radius && pos.x > net.tl.x)
		pos.x = net.tr.x + radius;
		
	if (pos.x <= radius)
		pos.x = radius;
	
	if (pos.x >= net.tl.x - radius && pos.x < net.tr.x)
		pos.x = net.tl.x - radius;
}

void Agent::render(bool lr) {
//	arc(pos.x, pos.y, radius*2, radius*2, -PI, 0);
//	line(pos.x - radius, pos.y, pos.x + radius, pos.y);
	if (!lr)
		gl::color(Color(1.0f, 165.0f/255.0f, 0));
	else
		gl::color(Color(148.0f/255.0f, 0, 211.0f/255.0f));
	gl::drawSolidCircle(pos, radius);
	
	Rectf hide = Rectf(pos.x - radius, pos.y, pos.x + radius, pos.y + radius);
	gl::color(Color(1, 1, 1));
	gl::drawSolidRect(hide);
}