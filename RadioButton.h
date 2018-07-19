struct RadioButton : Widget {
	Fl_Round_Button* pw;
	int i;
	RadioButton *par;
	RadioButton() : Widget(Point(0, 0), 0, 0, "", 0) {};
	RadioButton(Point p, int w, int h, string s, Callback cb, RadioButton *pa)
		: Widget(p, w, h, s, cb) {
		par = pa;
	}
	void set_label(string s) { label = s; }
	void attach(Windows &window) {
		pw = new Fl_Round_Button(xy.x, xy.y, width, height, label.c_str());
		pw->callback(do_it, (void *)par);
		pw->color(FL_DARK_BLUE);
		pw->align(FL_ALIGN_BOTTOM);
		pw->labelfont(FL_HELVETICA);
		pw->labelcolor(FL_WHITE);
		pw->labelsize(14);
		own = &window;
	}
	void hide() { pw->hide(); }
	void show() { pw->show(); }
	bool isOn() { return ((int)pw->value()); }
	void on() { pw->setonly(); }
	void off() { pw->value(0); }
	int equals(Fl_Round_Button *oth) {
		if (xy.x == oth->x())
			if (xy.y == oth->y())
				if (pw->value() == oth->value())
					return 1;
		return 0;
	}
};
