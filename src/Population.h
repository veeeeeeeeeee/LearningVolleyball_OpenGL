#ifndef POPULATION_H
#define POPULATION_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "DNA.h"
#include "Net.h"
#include "Ball.h"
#include "Bot.h"

class Population {
public:
	int pop;
	vector<DNA> population;
	vector<DNA> buffer;

	vector<Bot> a;
	Net net;
	Ball ball;

	static const int LEFT = 0;
	static const int RIGHT = 1;
	static const int MOVING = 0;
	static const int HITTING = 1;
	
	float gravity;
	int frame;
	bool terminateRound;

	// chosen is for after-trained bot
	int chosen0;
	int chosen1;

	int currentGen;
	bool nextGen; // true being getting the next chosen ones
	bool nextCompetition;
	float prevFit;

	Population();

	DNA getIndividual(int id);

	//void prelim();
	//void simulation();
	void selection();
	void nextGeneration();
	void evolve();
};

#endif