#include "DrawAgents.h"
#include <vector>

using namespace std;



void DrawAgents::draw() {
	fl_color(color);
	fl_pie(x(), y(), w(), h(), color, 360);
}

DrawAgents::DrawAgents(int X, int Y, int W, int H, Fl_Color c) :Fl_Widget(X, Y, W, H), color(c) {}

static void winUpdate(void *data)
{
	static unsigned i = 0;
	Fl_Double_Window *o = (Fl_Double_Window*)data;
	if (i < allAgents.size()) {
		o->add(allAgents[i]);
		o->redraw();
		Fl::add_timeout(0.5, winUpdate, data);
		++i;
	}
}



