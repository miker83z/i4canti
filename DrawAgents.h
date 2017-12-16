#ifndef DRAWAGENTS_HPP
#define DRAWAGENTS_HPP

#include <iostream>
#include <vector>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

static void winUpdate(void *data);

class DrawAgents : public Fl_Widget {

private:
	Fl_Color color;
	void draw();

public:
	DrawAgents(int X, int Y, int W, int H, Fl_Color c);

};

#endif 