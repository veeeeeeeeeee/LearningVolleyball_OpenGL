#include "Population.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Population::Population() {
	pop = 6;
	
	for (int i=0; i<pop; i++) {
		population.push_back(DNA());
	}

	for (int i=0; i<pop; i++) {
		buffer.push_back(DNA());
	}

	chosen0 = 0;
	chosen1 = 0;
	currentGen = 0;
	nextCompetition = true;
	nextGen = false;
	prevFit = 0;
}

void Population::selection() { // sort based on fitness, get the first 25%, put into buffer
	buffer = population;
	for (int i=0; i<buffer.size(); i++) {
		float maxFit = buffer[i].fitness;
		int maxInd = i;

		for (int j=i+1; j<pop; j++) {
			if (maxFit < buffer[j].fitness) {
				maxFit = buffer[j].fitness;
				maxInd = j;
			}
		}

		if (maxInd != i) {
			DNA swap = buffer[i];
			buffer[i] = buffer[maxInd];
			buffer[maxInd] = swap;
		}
	}
}

void Population::nextGeneration() { // perform cross-over of 2 random individuals among the better half, top 50%
	float currentFit = -1;
	for (int i=0; i<pop; i++) {
		if (currentFit < population[i].fitness)
			currentFit = population[i].fitness;
	}
	float lowestFit = 999;
	for (int i=0; i<pop; i++) {
		if (lowestFit > population[i].fitness)
			lowestFit = population[i].fitness;
	}

	for (int i=0; i<pop; i++) {
		int p1 = randInt(0, buffer.size()*buffer.size());
		p1 = buffer.size() - sqrt(p1);
		if (p1 == buffer.size()) p1--;
		int p2 = randInt(0, buffer.size()*buffer.size());
		p2 = buffer.size() - sqrt(p2);
		if (p2 == buffer.size()) p2--;

		// avoid crossover-ing with itself
		if (p2 == p1)
			p2 --;
		if (p2 == -1)
			p2 = buffer.size() - 1;

		population[i] = buffer[p1].crossover(buffer[p2]);
		// 0.005 being the default mutationRate
		// creature always has a chance to mutate regardless of which generation
		// if the current gen is not evolving far from previous gen, increase mutateion rate
		// if the current gen is too uniform, increase mutation rate
		population[i].mutate(0.005f + 0.05f/abs(currentFit - prevFit) + 0.05f/(currentFit - lowestFit));
		//console() << currentFit << " " << prevFit << " " << lowestFit << endl;
	}
	currentGen ++;
}

DNA Population::getIndividual(int id) {
	if (id > 0 && id < pop)
		return population[id];
	return population[0];
}

/**
 * Evolve process:
 * Make the pop (1 instance)
 * loop n times (n population)
 * Each gen:
 *     modify fitness based on simulation, cross 2 of every individual simluation()
 *     get the fittest - selection()
 *     mutate into next gen - nextGeneration()
 */