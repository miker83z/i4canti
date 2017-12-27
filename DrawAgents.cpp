#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "DrawAgents.h"
#define BG_COLOR   45
#define WHITE_AGENT 255
#define BLACK_AGENT 0

	DrawAgents::DrawAgents(int X, int Y, int W, int H, const char*L) : Fl_Box(X, Y, W, H, L) {
			box(FL_FLAT_BOX);
			color(BG_COLOR);
			Fl::add_timeout(0.25, Agent_RD, (void*)this);
	}


	void DrawAgents::draw() {

			static long start = time(NULL);
			long tick = time(NULL) - start;
			float pi = 3.14 - (((tick % 60) / 60.0) * 6.28);
			int radius = h() / 2;
			int x1 = (int)(x() + w() / 2),
				y1 = (int)(y() + h() / 2),
				x2 = (int)(x1 + (sin(pi) * radius)),
				y2 = (int)(y1 + (cos(pi) * radius));

			// Background
			Fl_Box::draw();

			// Drawing agents
			fl_color(WHITE_AGENT);
			fl_pie(x2, y2, 20, 20, 0.0, 360.0);





		}


	static void Agent_RD(void *userdata) {
			DrawAgents *o = (DrawAgents*)userdata;
			o->redraw();
			Fl::repeat_timeout(0.25, Agent_RD, userdata);
	}



