#ifndef BOT_H
#define BOT_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "Agent.h"
#include "Ball.h"
#include "DNA.h"

static const int M_LEFT = 0;
static const int M_RIGHT = 1;
static const int JUMP = 2;

class Bot : public Agent {
public:
	DNA dna;
	vector<vec2> inp;
	vector<bool> decision;

	float threshold;
	bool lr; // false = left, true = right
	float deltaMoved;
	
	Bot();
	Bot(vec2 _pos, float _ground, float _radius, float _gravity, DNA _dna, Net _n);

	// for bots on right hand side, map inputs and treat it as left
	void mapInputs();

	void update(Net n, Ball b, Agent o, bool lr);
};

#endif