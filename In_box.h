struct In_box : Widget {
	Fl_Input* pw;
	Fl_Color color;
	In_box(Point p, int w, int h, string s, Fl_Color c)
		: Widget(p, w, h, s, 0) {
		color = c;
	}
	void set_value(string s) { pw->value(s.c_str()); }
	string get_value() { return string(pw->value()); }
	void attach(Windows &window) {
		pw = new Fl_Input(xy.x, xy.y, width, height, label.c_str());
		pw->align(FL_ALIGN_RIGHT);
		pw->color(FL_WHITE);
		pw->textfont(FL_HELVETICA);
		pw->textcolor(FL_BLACK);
		pw->textsize(12);
		pw->labelfont(FL_HELVETICA);
		pw->labelcolor(color);
		pw->labelsize(14);
		own = &window;
	}
};

