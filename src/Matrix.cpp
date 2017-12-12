#include "Matrix.h"

Matrix::Matrix(int n, int n_canti) {
	this->N = n;
	mat = new int*[n];
	for (int i = 0; i < n; i++){
		mat[i] = new int[n];
	}
	canti_pos = new int*[n_canti];
	for (int i = 0; i < n_canti; i++) {
		mat[i] = new int[2];
	}
	set_canti(n_canti);
}

int Matrix::get_dim() {
	return N;
}

int Matrix::get_canti() {
	return (N*sizeof(canti_pos[0][0])) / sizeof(canti_pos[0][0]);
}

int* Matrix::get_canto_pos(int canto) {
	return canti_pos[canto];
}

void Matrix::set_canti(int canti) {
	switch (canti) {
	case 1:
		canti_pos[0][0] = 0;
		canti_pos[0][1] = 0;
		break;
	case 2:
		canti_pos[0][0] = 0;
		canti_pos[0][1] = 0;
		canti_pos[1][0] = N - 1;
		canti_pos[1][1] = 0;
		break;
	case 3:
		canti_pos[0][0] = 0;
		canti_pos[0][1] = 0;
		canti_pos[1][0] = N - 1;
		canti_pos[1][1] = 0;
		canti_pos[2][0] = 0;
		canti_pos[2][1] = N - 1;
		break;
	case 4:
		canti_pos[0][0] = 0;
		canti_pos[0][1] = 0;
		canti_pos[1][0] = N - 1;
		canti_pos[1][1] = 0;
		canti_pos[2][0] = 0;
		canti_pos[2][1] = N - 1;
		canti_pos[3][0] = N - 1;
		canti_pos[3][1] = N - 1;
		break;
	}
}

bool Matrix::is_not_occupied(int *x) {
	if(  )
}