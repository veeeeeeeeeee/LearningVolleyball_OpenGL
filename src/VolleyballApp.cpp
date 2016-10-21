#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include "Static.h"
#include "Agent.h"
#include "Ball.h"
#include "Bot.h"
#include "DNA.h"
#include "Net.h"
#include "Perceptron.h"
#include "Population.h"

class VolleyballApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	//void keyDown(KeyEvent event) override;
	//void keyUp(KeyEvent event) override;
	void update() override;
	void draw() override;

	void printTextData();

	Agent player;
	vector<Bot> a; // 2 bots for evolving process (simulation)
	vector<DNA*> d; // 2 dna for 2 bots simulating, 2 dnas are plugged into 2 bots

	Bot a1, a2; // post-trained bots
	DNA dna1, dna2; // post-trained bots
	Net n;
	Ball b;
	Population p;

	int framecount;
	float gravity;
	bool newRound;
	bool isTraining;
	int round;

	const float ground = 400;

	static const int LEFT = 0;
	static const int RIGHT = 1;
	static const int MOVING = 0;
	static const int HITTING = 1;

	void checkEndRound();

	const string PATH="F:\\Documents\\1 Studying\\MONASH\\2016-s1\\3094\\Assignments\\2\\DNA.txt";
	const string GENPATH="F:\\Documents\\1 Studying\\MONASH\\2016-s1\\3094\\Assignments\\2\\Generation.txt";

	void loadPopulation(string path);
};

void VolleyballApp::setup() {
	framecount = 0;

	newRound = false;
	isTraining = false;
	round = 0;

	gravity = -0.25;

	//dna1 = DNA();
	//dna2 = DNA();

	//player = Agent(vec2(getWindowWidth()/4, getWindowHeight()*2/3), ground, 40, gravity, n);
    
    n = Net();
    b = Ball(vec2(getWindowWidth()/2, getWindowHeight()/4), gravity, 10);
	b.iniVel();

	a.assign(2, Bot());

	// reading post-trained bots data
	ifstream iStream("F:\\Documents\\1 Studying\\MONASH\\2016-s1\\3094\\Assignments\\2\\DNA.txt", ios::in);
	string t;
	int ignore;
	char c_ignore;
	iStream >> t;
	iStream >> ignore;
	iStream >> c_ignore;

	float w;
	for (int i=0; i<dna1.p.size(); i++) {
		for (int j=0; j<dna1.p[i].weights.size(); j++) {
			iStream >> w;
			dna1.p[i].weights[j] = w;
		}
		iStream >> w;
		dna1.p[i].bias = w;
	}
	iStream >> w;
	dna1.fitness = w;

	for (int i=0; i<dna2.p.size(); i++) {
		for (int j=0; j<dna2.p[i].weights.size(); j++) {
			float w;
			iStream >> w;
			dna2.p[i].weights[j] = w;
		}
		iStream >> w;
		dna1.p[i].bias = w;
	}
	iStream >> w;
	dna2.fitness = w;

	iStream.close();

	a1 = Bot(vec2(getWindowWidth()/4, getWindowHeight()*2/3), ground, 40, gravity, dna2, n);
    a2 = Bot(vec2(getWindowWidth()*3/4, getWindowHeight()*2/3), ground, 40, gravity, dna1, n);

	//loadPopulation(GENPATH);
}

void VolleyballApp::mouseDown(MouseEvent event) {
}

// reading a trained population
void VolleyballApp::loadPopulation(string path) {
	p = Population();
	ifstream is(path, ios::in);
	string s;
	char c;
	int i;
	float w;

	is >> s >> i >> c;
	p.currentGen = i;
	// for now 30 = no. of individuals
	for (int j=0; j<p.pop; j++) {
		is >> s >> i >> c;
		for (int k=0; k<3; k++) {
			for (int l=0; l<12; l++) {
				is >> w;
				p.population[j].p[k].weights[l] = w;
			}
			is >> w;
			p.population[j].p[k].bias = w;
		}
		is >> w;
		p.population[j].fitness = w;
	}
	is >> w;
	p.prevFit = w;

	is.close();
}

/*
void VolleyballApp::keyDown(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_a) {
		player.movingLeft = true;
	}
	if (event.getCode() == KeyEvent::KEY_d) {
		player.movingRight = true;
	}
	if (event.getCode() == KeyEvent::KEY_w) {
		player.jump();
	}
}

void VolleyballApp::keyUp(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_a) {
		player.movingLeft = false;
	}
	if (event.getCode() == KeyEvent::KEY_d) {
		player.movingRight = false;
	}
}
*/

/*
void VolleyballApp::checkEndRound() {
	if (b.pos.y >= ground - b.radius && !newRound) {
    	b.pos.y = ground - b.radius;
		framecount = -50;
		newRound = true;
	}
}
*/

/**
 * complicated updates:
 * in current generation p.currentGen
 * current competition between p.chosen0 and p.chosen1
 * p.chosen0 and p.chosen1 are every pair in the generation
 * 
 * in 1 competition:
 * if agent hits the ball, increase agent fitness
 *	fitness gets boosted further based on:
 *	whether the ball will be sent over the net
 *  how high the agent is from the ground
 * if the ball lands on the ground:
 *	fitness gets increased or decreased based on how far the ball is from the agent
 * 
 * all the hardcoded floats are to restrict fitness to over-increase to a very large value
 * e.g. divide the fitness change further by 500.0f
 * 
 * after 5 occurrences of ball hitting ground/ agent hitting ball, move onto the next competition
 * 
 * after every pair is assessed, move onto next generation:
 *  p.selection gets the fittest 25%
 *  p.nextGeneration perform crossover - mutation
 */

void VolleyballApp::update() {
	if (isTraining) {
		if (p.nextCompetition && framecount >= 0) { // competition done, both fitnesses modified
			round = 0; // each competition lasts 5 occurrence of hittingball or ball hitting ground
			framecount = 0;

			b = Ball(vec2(getWindowWidth()/2, getWindowHeight()/4), gravity, 10);
			b.iniVel();

			p.chosen1 ++; // select the next pair to compete each other
			if (p.chosen1 == p.pop) {
				p.chosen0 ++;
				p.chosen1 = p.chosen0 + 1;
			}

			if (p.chosen0 == p.pop-1) { // finished last competition of the generation
				p.chosen0 = p.chosen1 = 0;

				p.nextGen = true;
				// 2 best DNA gets printed into a text file for testing later
				float maxFit = -999;
				int ind = -1;
				int nd = -1;
				for (int i=0; i<p.pop; i++) {
					if (maxFit < p.population[i].fitness) {
						maxFit = p.population[i].fitness;
						ind = i;
					}
				}

				float secondMaxFit = -999;
				for (int i=0; i<p.pop; i++) {
					if (i != ind && secondMaxFit != maxFit && secondMaxFit < p.population[i].fitness) {
						secondMaxFit = p.population[i].fitness;
						nd = i;
					}
				}

				// write out 2 best individuals into DNA.txt
				ofstream oStream;
				oStream.open(PATH, ofstream::out);
				oStream << "Generation: " << to_string(p.currentGen+1) << endl;
				oStream << p.getIndividual(ind).DNA_data() << endl;
				oStream << p.getIndividual(nd).DNA_data() << endl;
				oStream.close();
				
				// the complete current generation Generation.txt
				oStream.open(GENPATH, ofstream::out);
				oStream << "Generation: " << to_string(p.currentGen+1) << endl;
				for (int i=0; i<p.pop; i++) {
					oStream << "Individual: " + to_string(i+1) << endl;
					DNA print_dna = p.getIndividual(i);
					oStream << print_dna.DNA_data();
				}
				oStream << to_string(p.prevFit) << endl;
				oStream.close();

				p.selection();
				p.nextGeneration();
				p.prevFit = maxFit;
			}
			else { // start new competition
				d.clear();
				d.push_back(new DNA());
				d.push_back(new DNA());

				// 2 dnas being plugged into 2 bots
				d[LEFT] = &p.population[p.chosen0];
				d[RIGHT] = &p.population[p.chosen1];

				a[LEFT] = Bot(vec2(getWindowWidth()/4, getWindowHeight()*2/3), ground, 40, gravity, *d[LEFT], n);
				a[RIGHT] = Bot(vec2(getWindowWidth()*3/4, getWindowHeight()*2/3), ground, 40, gravity, *d[RIGHT], n);

				framecount = 0;
				b = Ball(vec2(getWindowWidth()/2, getWindowWidth()/4), gravity, 10);
				b.iniVel();

				p.nextCompetition = false;
			}
		}

		int behaviour = -1;
		int bot_no = -1;

		if (!p.nextCompetition) { // current competition is still running
			bool goalAchieved = false;
			// do updates
			a[0].update(n, b, a[1], false);
			a[1].update(n, b, a[0], true);
			b.update();

			b.collideNet(n);
			// end condition 1 - ball hits the ground
			if (b.pos.y >= ground - a[0].radius) {
				goalAchieved = true;
				behaviour = MOVING;

				if (b.pos.x < n.tl.x + n.w/2) // left
					bot_no = LEFT;
				else bot_no = RIGHT;
			}
			// end condition 2 - agent hits the ball
			for (int k=0; k<2; k++) {
				if (b.collideCircle(a[k].vel, a[k].pos, a[k].radius)) {
					goalAchieved = true;
					bot_no = k;
					behaviour = HITTING;
					break;
				}
			}

			if (goalAchieved) { // modify fitness after either ball hitting ground or a bot hits the ball
				round ++;
				
				if (behaviour == HITTING) {
					// the higher the bot jumps when hitting the ball, the higher fitness gets increased
					// 300 ~ maximum height agent can jump
					// 20 -> the increase is limited to 0.05
					// also based on how far it has moved from the previous fitness evalutaion
					// 100 ~ maximum distance it can move each occurrence
					d[bot_no]->fitness += abs(a[bot_no].pos.y - ground) * 0.01f * 0.003f;
					d[bot_no]->fitness += (a[bot_no].deltaMoved * 0.0001f);
					a[bot_no].deltaMoved = 0;

					bool ballguess = true;
					Ball tmp = b;

					// does the ball have potential to go over the net after being hit by the agent
					bool success_hit = false;
					while (ballguess) {
						tmp.update();
						
						// if the ball could get over the net
						tmp.collideNet(n);
						if (abs(tmp.pos.x - a[bot_no].pos.x) > abs(a[bot_no].pos.x - n.tl.x)) {
							ballguess = false;
							success_hit = true;
						}
						else {
							// if ball falls onto the ground, didnt get over the net
							if (tmp.pos.y >= ground - tmp.radius) {
								ballguess = false;
								success_hit = false;
							}
						}
					}
					
					// if it does, fitness increases
					if (success_hit) {
						d[bot_no]->fitness += 0.01f;
					}
				}

				// ball falls on the ground without anyone hitting it
				// how far the ball is from the agent when it falls under the ground
				float distance = abs(b.pos.x - a[bot_no].pos.x);
				if (behaviour == MOVING) {
					if (distance > 1) {
						d[bot_no]->fitness += 0.001f / distance;
					}
					else d[bot_no]->fitness += 0.001f;

					b = Ball(vec2(getWindowWidth()/2, getWindowHeight()/4), gravity, 10);
					b.iniVel();
					// ball hit the ground, reset
					a[0].pos = a[0].oriPos;
					a[1].pos = a[1].oriPos;
				}
			}
			// 5 occurrences of ball hitting ground or agent hitting ball
			// move onto next competition
			if (round >= 5) {
				p.nextCompetition = true;
				framecount = 0;
			}
		}
		framecount++;
	}
	else { // post-trained simulation
		a1.update(n, b, a2, false);
		a2.update(n, b, a1, true);
		b.update();

		b.collideNet(n);
		b.collideCircle(a1.vel, a1.pos, a1.radius);
		b.collideCircle(a2.vel, a2.pos, a2.radius);

		if (b.pos.y >= ground - b.radius) {
			b = Ball(vec2(getWindowWidth()/2, getWindowHeight()/4), gravity, 10);
			b.iniVel();

			a1.pos = a1.oriPos;
			a2.pos = a2.oriPos;
		}
	}
	
	if (isTraining)
		console() << getAverageFps() << endl;
}

void VolleyballApp::printTextData() {
	string tmp = "";
	vec2 pos(550, 25);
	vec2 down(0, 25);
	Color color = (Color(100.0f/255, 100.0f/255, 100.0f/255));
	Font font = Font("Arial", 20);

	if (isTraining) {
		tmp = "Generation: " + to_string(p.currentGen+1);
		gl::drawString(tmp, pos, color, font);
		pos += down;

		tmp = "Individual: " + to_string(p.chosen0+1);
		tmp += " vs. ";
		tmp += to_string(p.chosen1+1);
		gl::drawString(tmp, pos, color, font);
		pos += down;

		tmp = "Fitnesses: " + to_string(p.getIndividual(p.chosen0).fitness); tmp += " ; ";
		tmp += to_string(p.getIndividual(p.chosen1).fitness);
		gl::drawString(tmp, pos, color, font);
		pos += down;

		tmp = "Best Fitness: ";
		float maxFit = -999;
		for (int i=0; i<p.population.size(); i++) {
			if (maxFit < p.population[i].fitness)
				maxFit = p.population[i].fitness;
		}
		tmp += to_string(maxFit);
		gl::drawString(tmp, pos, color, font);
		pos += down;

		pos = vec2(25, 25);
		tmp = "FPS: " + to_string(getAverageFps());
		gl::drawString(tmp, pos, color, font);
	}
	else {
		tmp = "Fitnesses: " + to_string(dna1.fitness); tmp += " ; ";
		tmp += to_string(dna2.fitness);
		gl::drawString(tmp, pos, color, font);
		pos += down;
	}
}

void VolleyballApp::draw() {
	if (isTraining) {
		gl::clear(Color(1, 1, 1));
		// render player, agent, ball
		//player.render();
		//a1.render();
		//a2.render();
		a[0].render(false);
		a[1].render(true);
		b.render();
		
		// render net
		n.render();
		
		// line(0, 400, 800, 400);
		gl::color(Color(100.0f/255, 100.0f/255, 100.0f/255));
		gl::drawLine(vec2(0, ground), vec2(getWindowWidth(), ground));

		printTextData();
	}
	else {
		gl::clear(Color(1, 1, 1));
		a1.render(false);
		a2.render(true);
		b.render();

		n.render();
		gl::drawLine(vec2(0, ground), vec2(getWindowWidth(), ground));
		printTextData();
	}
}

CINDER_APP(VolleyballApp, RendererGl, [&](VolleyballApp::Settings *settings) {
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
})
