#ifndef DNA_H
#define DNA_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "Perceptron.h"

class DNA {
public:
	vector<Perceptron> p;
	float fitness;

	DNA();
	DNA(vector<Perceptron>& _p);

	DNA mutate(float mRate);
	DNA crossover(DNA partner);

	string DNA_data();
};

#endif