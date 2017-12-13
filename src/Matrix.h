#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<vector>

class Matrix {
public:
	Matrix(int n_canti, int N);
	int get_in_position(int x, int y);
	void set_in_position(int x, int y);
	int get_dim();
	int get_canti();
	int* get_canto_pos( int i );
	bool is_not_occupied(int *x);
private: 
	int N;
	int** mat;
	int** canti_pos;
	void set_canti(int canto);
};

#endif // MATRIX_HPP
