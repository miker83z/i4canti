struct Button : Widget {
	Fl_Button* pw;
	Button(Point p, int w, int h, string s, Callback cb)
	: Widget(p, w, h, s, cb) {}
	void set_label(string s) { label = s; }
	void attach(Windows &window) {
		pw = new Fl_Button(xy.x, xy.y, width, height, label.c_str());
		pw->callback(do_it);
		pw->color(FL_DARK_BLUE);
		pw->labelfont(FL_HELVETICA);
		pw->labelcolor(FL_WHITE);
		pw->labelsize(14);
		own = &window;
	}
	void hide() { pw->hide(); }
	void show() { pw->show(); }
};


