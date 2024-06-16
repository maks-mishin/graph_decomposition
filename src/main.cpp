#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;

// что является узлами графа - узлы или ячейки сетки
enum class GraphVertices { NODES, ELEMENTS };
// каким образом делятся ячейки исходной сетки на треугольники
enum class DivideCellsTriangle { LEFT, RIGHT };

class Vertex {
public:
	int Value;
	Vertex() { Value = 0; }
	Vertex(int val) { Value = val; }
};

// класс, описывающий граф, построенный по заданной сетке
// в матрицу смежности помещаются связи между ребрами + главная диагональ
class SimpleGraph {
public:
	vector<Vertex> vertex;
	vector<vector<int>> matrixAdjacency;
	int vertexCount;

	void Init(int size) {
		vertexCount = size;
		matrixAdjacency.resize(vertexCount);
		for (int i = 0; i < vertexCount; i++) {
			matrixAdjacency[i].resize(vertexCount);
			for (int j = 0; j < vertexCount; j++) {
				matrixAdjacency[i][j] = 0;
			}
		}
		vertex.resize(vertexCount);
	}

	void AddVertex(int value) {
		// добавление новой вершины со значением value
		// в свободную позицию массива vertex
	}

	// v - индекс вершины в списке vertex
	void RemoveVertex(int v) {
		// код удаления вершины со всем её ребрами
	}

	bool IsEdge(int v1, int v2) {
		// true если есть ребро между вершинами v1 и v2
		return (matrixAdjacency[v1][v2] == matrixAdjacency[v2][v1] == 1);
	}

	void AddEdge(int v1, int v2) {
		// добавление ребра между вершинами v1, v2
		matrixAdjacency[v1][v2] = 1;
		matrixAdjacency[v2][v1] = 1;
	}

	void RemoveEdge(int v1, int v2) {
		// удаление ребра между вершинами v1 и v2
		matrixAdjacency[v1][v2] = 0;
		matrixAdjacency[v2][v1] = 0;
	}

	void PrintMatrixAdjacency() {
		// печать матрицы смежности графа
		for (int i = 0; i < vertexCount; i++) {
			for (int j = 0; j < vertexCount; j++) {
				cout << matrixAdjacency[i][j] << " ";
			}
			cout << endl;
		}
	}
};

// class for describe the matrix of graph
// matrix of adjacency + main diagonal
class SparseMatrix {
public:
	int nRows = 0;
	int* IA = nullptr;
	int* JA = nullptr;
	double* A = nullptr;

	SparseMatrix(int _nRows) {
		nRows = _nRows;
		IA = new int[nRows + 1];
		// fill IA arrays
		JA = new int[IA[nRows]];
		A = new double[IA[nRows]];
	}

	~SparseMatrix() {
		delete[] IA;
		delete[] JA;
		delete[] A;
	}
};

class Edge {
public:
	int id;
	int vertices[2]; // глобальные id вершин данного ребра
	int cells[2]; // глобальные id двух ячеек, которые разделяет ребро, -1, если ячейка граничная

	Edge(int id, int v1, int v2) : id(id) {
		vertices[0] = v1;
		vertices[1] = v2;
		cells[0] = cells[1] = -1; // initialize as boundary edges
	}
};

class Node {
public:
	int id;
	double coord[2];
	set<int> edges; // вектор глобальных id ребер, которые сходятся в данном узле

	Node(int id, double x, double y) : id(id) {
		coord[0] = x;
		coord[1] = y;
	}
};

class Cell {
public:
	int id;
	double coordCenter[2];
	set<int> edges; // вектор глобальных id ребер, из которых состоит ячейка
	set<int> vertices; // вектор глобальных id вершин, из которых состоит ячейка

	Cell(int id, double x, double y) : id(id) {
		coordCenter[0] = x;
		coordCenter[1] = y;
	}
};

class Mesh {
public:
	// параметры решетки
	// Nx, Ny – число клеток в решетке по вертикали и горизонтали
	// параметры для количества однопалубных и двухпалубных кораблей
	// треугольных и четырехугольных элементов
	int Nx, Ny, K1, K2;

	// сеточные данные
	int nodesCount = 0, cellCount = 0, edgesCount = 0;
	vector<Node> nodes;
	vector<Cell> cells;
	vector<Edge> edges;

	map<tuple<int, int>, int> mapNodeToEdgeId;
	DivideCellsTriangle divideCells;

	Mesh(int _Nx, int _Ny, int _K1, int _K2, DivideCellsTriangle _divideCells) {
		Nx = _Nx;
		Ny = _Ny;
		K1 = _K1;
		K2 = _K2;
		divideCells = _divideCells;
	}

	// TODO: implement in future version
	void PrintTopology(); // печать топологии сетки
	void CreateTopology(); // создание топологии сетки
	void PrintSourceGrid(); // печать исходной решетки для построения сетки
};

// Генерация графа/портрета разреженной матрицы по заданной сетке
void Generate() {
}

// Заполнение разреженной матрицы по заданному портрету
void Fill() {
}

// Решение СЛАУ с полученной матрицей
void Solve() {
}

// Проверка корректности программы и выдача измерений
void Report() {
}

int main()
{
	return 0;
}
