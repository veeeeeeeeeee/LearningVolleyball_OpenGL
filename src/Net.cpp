#include "Net.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Net::Net() {
	// middle of the screen
	tl = vec2(390, 360);
	tr = vec2(410, 360);

	bl = vec2(390, 400);
	br = vec2(410, 400);

	w = tr.x - bl.x;
	h = bl.y - tr.y;
}

void Net::render() {
	// rect(tl.x, tl.y, w, h);
	// ellipse(tl.x + w/2, tl.y, w, w);
	gl::color(Color(50.0f/255.0f, 205.0f/255.0f, 50.0f/255.0f));
	gl::drawSolidCircle(vec2(tl.x + w/2.0f, tl.y), w/2.0f);

	Rectf body = Rectf(tl.x, tl.y, br.x, br.y);
	gl::drawSolidRect(body);
}
