struct CheckButton : Widget {
	Fl_Check_Button* pw;
	int i;
	CheckButton(Point p, int w, int h, string s, Callback cb, int param)
		: Widget(p, w, h, s, cb) {
		i = param;
	}
	void set_label(string s) { label = s; }
	void attach(Windows &window) {
		pw = new Fl_Check_Button(xy.x, xy.y, width, height, label.c_str());
		if (i == 0) pw->value(1); //Save Sim Data
		pw->callback(do_it, &i);
		pw->color(FL_DARK_BLUE);
		pw->labelfont(FL_HELVETICA);
		pw->labelcolor(FL_WHITE);
		pw->labelsize(14);
		own = &window;
	}
	void hide() { pw->hide(); }
	void show() { pw->show(); }
};

