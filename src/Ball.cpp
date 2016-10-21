#include "Ball.h"
#include "Static.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Ball::Ball() {
}

Ball::Ball(vec2 _pos, float _gravity, float _radius) {
	pos = _pos;
	gravity = _gravity;
	radius = _radius;

	vel =  vec2(0, 0);
	maxSpd = 7.5;

	ground = 400;
}

void Ball::reset() {
	vel = vec2(0, 0);
}

void Ball::iniVel() {
	vel = random2D();
	vel *= maxSpd;

	if (vel.y > 0)
		vel.y = - vel.y;
	if (rand() % 2)
		vel.x *= -1.0f;
	else vel.x *= 1.0f;
	
	vel = normalize(vel);
	vel *= maxSpd;
}

void Ball::collideNet(Net n) {
	// collide body
	collideCircle(vec2(0, -1), vec2(n.tl.x + n.w/2, n.tl.y), n.w/2);

	if (pos.y >= n.tr.y && pos.y <= ground) {
		if (pos.x < n.tr.x)
			bounceLeft(n.tl.x);

		if (pos.x > n.tl.x)
			bounceRight(n.tr.x);
	}
}

bool Ball::collideCircle(vec2 incVel, vec2 c, float r) {
	vec2 n = pos - c;
	if (mag(n) < r + radius && pos.y <= c.y) {
		n = normalize(n);
		vec2 u = vel - incVel;
		float undot = dot(u, n) * 2.0f;
		vec2 un = n * undot;
		u = u - un;

		vel = u + incVel;
		vel *= 0.6f;

		while (mag(c - pos) <= r + radius)
			pos = pos + vel;
		
		return true;
	}
	return false;
}

void Ball::bounceLeft(float x) {
	if (pos.x >= x - radius) {
		pos.x = x - radius;
		vel.x = -vel.x;
	}
}

void Ball::bounceRight(float x) {
	if (pos.x <= x + radius) {
		pos.x = x + radius;
		vel.x = -vel.x;
	}
}

void Ball::update() {
	vel.y -= gravity;

	// bounce back from wall
	bounceLeft(800);
	bounceRight(0);

	if (pos.y <= radius) {
		pos.y = radius;
		vel.y = -vel.y;
	}

	pos = pos + vel;
}

void Ball::render() {
	// ellipse(pos.x, pos.y, radius*2, radius*2);
	gl::color(Color(100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f));
	gl::drawSolidCircle(pos, radius);
}