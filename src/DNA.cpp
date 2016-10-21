#include "DNA.h"
#include "cinder\Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

DNA::DNA() {
	// DNA represents the whole network
	// DNA has 3 perceptrons triggering 3 output (control)
	// each perceptron takes in 6 vectors, 12 inputs (Perceptron constructor)
	for (int i=0; i<3; i++)
		p.push_back(Perceptron(6));
	
	fitness = 0;
}

DNA::DNA(vector<Perceptron>& _p) {
	p = _p;
}

DNA DNA::mutate(float mRate) {
	DNA child = DNA(p);
	Rand r;
	
	for (int i=0; i<child.p.size(); i++) {
		for (int j=0; j<child.p[i].weights.size(); j++) {
			if (r.randFloat(1.0f) < mRate)
				// similar to randomising weights in Perceptron constructor
				child.p[i].weights[j] = r.randFloat(-1.0f, 1.0f);
		}
		if (r.randFloat(1.0f) < mRate)
			// bias same with randomising weights
			child.p[i].bias = r.randFloat(-2.0f, 2.0f);
	}
	// copy the fitness
	child.fitness = fitness;
	return child;
}

DNA DNA::crossover(DNA partner) {
	DNA child = DNA();
	for (int i=0; i<child.p.size(); i++) {
		for (int j=0; j<child.p[i].weights.size(); j++) {
			// inherit the genome data from either one of the parents
			// random 50/50 chance
			if (randBool())
				child.p[i].weights[j] = p[i].weights[j];
			else child.p[i].weights[j] = partner.p[i].weights[j];
		}
		// inherit the bias in the same manner
		if (randBool())
			child.p[i].bias = p[i].bias;
		else child.p[i].bias = partner.p[i].bias;
	}
	// fitness is average of the parents
	child.fitness = (fitness + partner.fitness) / 2;

	return child;
}

// print output purposes
string DNA::DNA_data() {
	string data = "";

	for (int i=0; i<p.size(); i++) {
		for (int j=0; j<p[i].weights.size(); j++) {
			data += to_string(p[i].weights[j]);
			data += " ";
		}
		data += to_string(p[i].bias);
		data += "\n";
	}
	data += to_string(fitness);
	data += "\n";

	return data;
}