//
// FLTK drawing example showing simple line drawing animation
// erco 03/22/07
//
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define BG_COLOR   45
#define WHITE_AGENT 255
#define BLACK_AGENT 0
class MyTimer : public Fl_Box {
	void draw() {
		// COMPUTE NEW COORDS OF LINE
		static long start = time(NULL);
		long tick = time(NULL) - start;
		char secs[80];
		float pi = 3.14 - (((tick % 60) / 60.0) * 6.28);
		int radius = h() / 2;
		int x1 = (int)(x() + w() / 2),
			y1 = (int)(y() + h() / 2),
			x2 = (int)(x1 + (sin(pi) * radius)),
			y2 = (int)(y1 + (cos(pi) * radius));

		// TELL BASE WIDGET TO DRAW ITS BACKGROUND
		Fl_Box::draw();

		// DRAW 'SECOND HAND' OVER WIDGET'S BACKGROUND
		fl_color(WHITE_AGENT);
		fl_pie(x2, y2, 20, 20, 0.0, 360.0);
		fl_color(BLACK_AGENT);
		fl_pie(x2 - 40, y2 - 40, 20, 20, 0.0, 360.0);


	}
	static void Timer_CB(void *userdata) {
		MyTimer *o = (MyTimer*)userdata;
		o->redraw();
		Fl::repeat_timeout(0.25, Timer_CB, userdata);
	}
public:
	// CONSTRUCTOR
	MyTimer(int X, int Y, int W, int H, const char*L = 0) : Fl_Box(X, Y, W, H, L) {
		box(FL_FLAT_BOX);
		color(BG_COLOR);
		Fl::add_timeout(0.25, Timer_CB, (void*)this);
	}
};
