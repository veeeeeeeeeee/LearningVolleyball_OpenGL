#include "Perceptron.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Perceptron::Perceptron(int _n) {
	n = 2.0f * _n;
	Rand r;

	weights.assign(n, 0);
	// weights from -1, 1
	for (int i=0; i<n; i++)
		weights[i] = randFloat(-1.0f, 1.0f);
	inputs.assign(n, 0);

	// bias from trial & error
	bias = randFloat(-2.0f, 2.0f);
}

void Perceptron::genInputs(vector<vec2>& inp) {
	// from inp - vectors
	// get the floats (pos.x, pos.y) and put into Perceptron's own inputs array
	for (int i=0; i<inp.size(); i++) {
		// inputs are scaled down
		if (i % 2 == 0) {
			inputs[i*2] = inp[i].x / getWindowWidth();
			inputs[i*2 + 1] = inp[i].y / getWindowHeight();
		}
		else {
			// velocity limits are smaller, approx 10
			inputs[i*2] = inp[i].x / 10.0f;
			inputs[i*2 + 1] = inp[i].y / 10.0f;
		}
	}
}

float Perceptron::feedForward() {
	float sum = 0;
	
	for (int i=0; i<n; i++) {
		sum += inputs[i] * weights[i];
	}
	
	return sum + bias;
}

// sigmoid function
float Perceptron::activation(float sum) {
	return 1 / (1+exp(-sum));
}