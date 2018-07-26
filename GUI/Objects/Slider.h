struct Slider : Widget {
	Fl_Value_Slider* pw;
	uchar t1; Fl_Align algn1; Fl_Color labcol1;
	Slider *par;
	Slider() : Widget(Point(0,0), 0, 0, "", 0) {};
	Slider(Point p, int w, int h, string s, uchar t, Fl_Align algn, Fl_Color labcol, Callback cb, Slider *pa)
		: Widget(p, w, h, s, cb) {
		t1 = t;
		algn1 = algn;
		labcol1 = labcol;
		par = pa;
	}
	void set_value(double v) { pw->value(v); }
	double get_value() { return pw->value(); }
	void set_bounds(double b1, double b2) { pw->bounds(b1, b2); }
	void set_param(Slider *param) { par = param; }
	void resize(int w, int h) { pw->hide(); }
	void attach(Windows &window) {
		pw = new Fl_Value_Slider(xy.x, xy.y, width, height, label.c_str());
		pw->type(t1); //FL_HOR_SLIDER
		pw->align(algn1); //FL_ALIGN_RIGHT
		pw->labelfont(FL_HELVETICA);
		pw->labelcolor(labcol1);
		pw->labelsize(14);
		pw->callback(do_it, (void *)par);
		own = &window;
	}
	int equals(Fl_Value_Slider *oth) {
		if (xy.x == oth->x())
			if (xy.y == oth->y())
				if (get_value() == oth->value())
					return 1;
		return 0;
	}
};
