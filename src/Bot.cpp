#include "Bot.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Bot::Bot() {
}

Bot::Bot(vec2 _pos, float _ground, float _radius, float _gravity, DNA _dna, Net _n)
	: Agent(_pos, _ground, _radius, _gravity, _n) {
	
	dna = _dna;
	Rand r;
	
	inp.assign(6, vec2(0, 0));
	decision.assign(3, false);
	threshold = 0.8f;
	deltaMoved = 0;
}

void Bot::update(Net n, Ball b, Agent o, bool lr) {
	/*
	if (frameCount % 20 == 0)
	if (random(1.0) > 0.5)
		movingLeft = true;
	else movingLeft = false;
	
	if (frameCount % 20 == 10)
	if (random(1.0, 2.0) > 1.5)
		movingRight = true;
	else movingRight = false;
	
	if (frameCount % 40 == 0)
	if (random(1.0) > 0.1)
		jump();
	
	*/

	//--- get inputs and feed forward ---//
	// inp are vectors data
	inp[0] = pos;
	inp[1] = vel;
	inp[2] = b.pos;
	inp[3] = b.vel;
	inp[4] = o.pos;
	inp[5] = o.vel;

	if (lr) { // map the inputs to left side
		mapInputs();
	}
	
	for (int i=0; i<3; i++) {
		dna.p[i].genInputs(inp);
		
		decision[i] = false;
		float output = dna.p[i].feedForward();
		// does the perceptron trigger the control
		decision[i] = (dna.p[i].activation(output) > threshold)? true : false;
		//console() << dna.p[i].activation(output) << " ";
	}
	
	if (lr) {
		// right hand side bot
		movingLeft = decision[M_RIGHT];
		movingRight = decision[M_LEFT];
	}
	else {
		movingLeft = decision[M_LEFT];
		movingRight = decision[M_RIGHT];
	}

	if (decision[JUMP])
		jump();

	deltaMoved += abs(vel.x);
	
	Agent::update();
}

void Bot::mapInputs() {
	// right hand side bot
	inp[1].x *= -1.0f;
	inp[3].x *= -1.0f;
	inp[5].x *= -1.0f;

	inp[0].x = getWindowWidth() - inp[0].x;
	inp[2].x = getWindowWidth() - inp[0].x;
	inp[4].x = getWindowWidth() - inp[0].x;
}