#ifndef MATRIX_HPP
#define MATRIX_HPP

class Matrix {
public:
	Matrix(int n_canti, int N);
	int get_in_position(int x, int y);
	void set_in_position(int x, int y, int o_x, int o_y);
	int get_dim();
	int get_canti();
	int* get_canto_pos( int i );
	bool is_not_occupied(int x, int y);
private: 
	int N;
	int num_canti;
	int** mat;
	int** canti_pos;
	void set_canti(int canto);
};

#endif // MATRIX_HPP
