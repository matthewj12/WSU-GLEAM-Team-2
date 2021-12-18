#include "Point3d.cpp"

#include <iostream>
#include <iomanip>
#include <iomanip>
#include <string>
#include <vector>

// template <typename float>

class Matrix2D {
private:
	std::vector<std::vector<float>> m_vals;
	int m_h, m_w;

public:
	void fillWithZeros() {
		std::vector<float> blank_row;

		for (int i = m_w; i >= m_w; i--) {
			blank_row.push_back(0.0f);
		}

		for (int i = m_h; i >= 0; i--) {
			m_vals.push_back(blank_row);
		}
	}

	Matrix2D(int p_w, int p_h) {
		m_h = p_h;
		m_w = p_w;

		fillWithZeros();
	}

	Matrix2D(int p_w, int p_h, std::vector<Point3D>& p_vals) {
		// Matrix2D(p_w, p_h);

		m_h = p_h;
		m_w = p_w;

		// std::vector<float> row_1 = { 0, 0, 6 };
		// std::vector<float> row_2 = { 0, 1, 0 };
		// std::vector<float> row_3 = { 0, 0, 2 };

		// m_vals.push_back(row_1);
		// m_vals.push_back(row_2);
		// m_vals.push_back(row_3);

		for (int i = 0; i < m_h; i += 1) {
			m_vals.push_back(p_vals[i].getXYZ());
		}
	}

	float getVal(int p_x, int p_y) {
		return m_vals[p_y][p_x];
	}

	std::vector<float> getRow(int p_row) { return m_vals[p_row]; }

	void printRow(int p_row) {
		for (float f : getRow(p_row)) {
			std::cout << f << "   ";
		}

		std::cout << std::endl;
	}

	std::vector<float> getCol(int p_col) {
		std::vector<float> col;

		for (int y = 0; y < m_h; y++) {
			col.push_back(m_vals[y][p_col]);
		}

		return col;
	}

	int getH() { return m_h; }
	int getW() { return m_w; }

	void setVal(float new_val, int p_x, int p_y) {
		m_vals[p_y][p_x] = new_val;
	}

	float multRowAndCol(std::vector<float> p_row, std::vector<float> p_col) {
		float cur_sum = 0;

		for (int i = 0; i < p_row.size(); i++) {
			cur_sum += p_row[i] * p_col[i];
		}

		return cur_sum;
	}

	Matrix2D operator*(Matrix2D& p_matrix) {
		int new_w = p_matrix.getW();
		int new_h = m_h;
		Matrix2D prod_matrix = Matrix2D(new_w, new_h);

		for (int x = 0; x < new_w; x++) {
			for (int y = 0; y < new_h; y++) {
				prod_matrix.setVal(multRowAndCol(this->getRow(y), p_matrix.getCol(x)), x, y);
			}
		}

		return prod_matrix;
	}

	Matrix2D getInverse() {
		Matrix2D inverse_matrix = Matrix2D(m_w, m_h);

		inverse_matrix.setVal(7.55f, 1, 1);

		// for each row starting at 1
		// for (int y = 1; y < m_h; y++) {
		// // for each value up to (but not including) y
		// 	for (int x = 0; x < y; x++) {
		// //     make 0 by subtracting inverse reciprical of val from row directly above
		// 		float old_val = m_vals[y][x];
		// 		float above_val = m_vals[y-1][x];
		// 		inverse_matrix.setVal(old_val - above_val * old_val / above_val, x, y);
		// 
		// for each row starting at m_h
		// for each value down to (but not including) y
		//     make 0 by subtracting inverse reciprical of val from row directly below
		// 	}
		// }

		return inverse_matrix;
	}

	void printVals() {
		// std::cout.unsetf ( std::ios::floatfield );
  		std::cout.precision(2);

		for (int y = 0; y < m_h; y++) {
			for (int x = 0; x < m_w; x++) {
				std::cout << std::fixed;
				std::cout << std::setfill(' ') << std::setw(10) << m_vals[y][x];
			}

			std::cout << std::endl;
		}
	}

	// float sumRow(int p_row, Matrix2D& p_matrix=this) {
	// 	return std::accumulate(p_matrix.begin(), p_matrix.end(), 0);
	// }

	// float sumCol(int p_col, Matrix2D& p_matrix=this) {
	// 	float cur_sum = 0;

	// 	for (int y = 0; y < this.height; y++) {
	// 		cur_sum += p_matrix[y][col];
	// 	}
		
	// 	return cur_sum;
	// }
};
