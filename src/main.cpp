#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;

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
