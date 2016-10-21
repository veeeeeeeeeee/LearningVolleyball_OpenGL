#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "cinder\app\App.h"
#include "cinder\app\RendererGl.h"
#include "cinder\gl\gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Perceptron {
public:
	int n;
	float c;
	float bias;

	vector<float> weights;
	vector<float> inputs;

	Perceptron(int _n);
	void genInputs(vector<vec2>& inp);
	float feedForward();
	float activation(float sum);
};

#endif