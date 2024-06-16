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

void Mesh::PrintTopology() {
	cout << "Nodes:" << endl;
	for (const auto& node : nodes) {
		cout << "Node " << node.id << " (" << node.coord[0] << ", " << node.coord[1] << ") nodeEdges: ";
		for (const auto& edgeId : node.edges) {
			cout << edgeId << " ";
		}
		cout << endl;
	}

	cout << "Edges:" << endl;
	for (const auto& edge : edges) {
		cout << "Edge " << edge.id << " (" << edge.vertices[0] << ", " << edge.vertices[1] << "): ";
		cout << "Cells " << edge.cells[0] << ", " << edge.cells[1] << endl;
	}

	cout << "Cells:" << endl;
	for (const auto& cell : cells) {
		cout << "Cell " << cell.id << " Center (" << cell.coordCenter[0] << ", " << cell.coordCenter[1] << "): ";
		cout << "Edges: ";
		for (const auto& edgeId : cell.edges) {
			cout << edgeId << " ";
		}
		cout << "Vertices: ";
		for (const auto& vertexId : cell.vertices) {
			cout << vertexId << " ";
		}
		cout << endl;
	}
}

void Mesh::PrintSourceGrid() {
	int globalCellId = 0;
	int cellWidth = 5;

	for (int i = 0; i < Nx; i++) {
		for (int j = 0; j < Ny; ++j) {
			cout << "+";
			cout << string(cellWidth, '-');
		}
		cout << "+" << endl;

		for (int j = 0; j < Ny; j++) {
			if (globalCellId < K1)
				cout << "| " << setw(cellWidth - 2) << " " << " ";
			else
				cout << "| " << setw(cellWidth - 2) << "/" << " ";
			globalCellId = (globalCellId + 1) % (K1 + K2);
		}
		cout << "|" << endl;
	}
	for (int j = 0; j < Ny; ++j) {
		cout << "+";
		cout << string(cellWidth, '-');
	}
	cout << "+" << endl;
}

void Mesh::CreateTopology() {
	// построение топологии заданной сетки
	double cellSize = 1.0; //размер каждой ячейки, все ячейки квадратные пока что
	int globalCellId = 0;

	// Create nodes (for rect and triangle cells same code)
	int nodeId = 0;
	map<tuple<int, int>, int> nodeMap;

	for (int i = 0; i <= Nx; i++) {
		for (int j = 0; j <= Ny; j++) {
			nodes.push_back({ nodeId, i * cellSize, j * cellSize });
			nodeMap[{i, j}] = nodeId;
			nodeId++;
		}
	}

	// Create cells and edges
	int cellId = 0, edgeId = 0;
	// делаем обход по прямоугольным ячейкам, некоторые из них деляется на треугольники
	for (int i = 0; i < Nx; i++) {
		for (int j = 0; j < Ny; j++) {
			// поставить условие на тот случай, если будут треугольные ячейки

			if (globalCellId < K1) {
				// обрабатываем как просто прямоугольные ячейки
				tuple<double, double> cellCenter = { (i + 0.5) * cellSize, (j + 0.5) * cellSize };
				cells.push_back({ cellId, get<0>(cellCenter), get<1>(cellCenter) });

				Cell& cell = cells.back();
				cell.id = cellId;
				cellId++;

				int v0 = nodeMap[{i, j}];
				int v1 = nodeMap[{i + 1, j}];
				int v2 = nodeMap[{i + 1, j + 1}];
				int v3 = nodeMap[{i, j + 1}];

				cell.vertices = { v0, v1, v2, v3 };
				vector<tuple<int, int>> edgeVertices = {
					{v0, v1}, {v1, v2}, {v2, v3}, {v3, v0}
				};
				// Define edges of the cell
				for (const auto& ev : edgeVertices) {
					if (mapNodeToEdgeId.find(ev) == mapNodeToEdgeId.end()) {
						edges.push_back({ edgeId, get<0>(ev), get<1>(ev) });
						mapNodeToEdgeId[ev] = edgeId;
						mapNodeToEdgeId[{get<1>(ev), get<0>(ev)}] = edgeId;
						edgeId++;
					}
					cell.edges.insert(mapNodeToEdgeId[ev]);
				}
			}
			else {
				// делим прямоугольную ячейку на две треугольные из правого верхнего угла в нижний левый
				// первая ячейка, центр ячейки находим как центр тяжести треугольника
				{
					tuple<double, double> cellCenter = {
					(i * cellSize + i * cellSize + (i + 1) * cellSize) / 3.0,
					(j * cellSize + j * cellSize + (j + 1) * cellSize) / 3.0
					};
					cells.push_back({ cellId, get<0>(cellCenter), get<1>(cellCenter) });
					Cell& cell = cells.back();
					cell.id = cellId;
					cellId++;

					int v0 = nodeMap[{i, j}];
					int v1 = nodeMap[{i + 1, j}];
					int v2 = nodeMap[{i, j + 1}];

					cell.vertices = { v0, v1, v2 };
					vector<tuple<int, int>> edgeVertices = {
						{v0, v1}, {v1, v2}, {v2, v0}
					};
					// Define edges of the cell
					for (const auto& ev : edgeVertices) {
						if (mapNodeToEdgeId.find(ev) == mapNodeToEdgeId.end()) {
							edges.push_back({ edgeId, get<0>(ev), get<1>(ev) });
							mapNodeToEdgeId[ev] = edgeId;
							mapNodeToEdgeId[{get<1>(ev), get<0>(ev)}] = edgeId;
							edgeId++;
						}
						cell.edges.insert(mapNodeToEdgeId[ev]);
					}
				}

				// вторая ячейка
				{
					tuple<double, double> cellCenter = {
					(i * cellSize + (i + 1) * cellSize + (i + 1) * cellSize) / 3.0,
					(j * cellSize + (j + 1) * cellSize + (j + 1) * cellSize) / 3.0
					};
					cells.push_back({ cellId, get<0>(cellCenter), get<1>(cellCenter) });
					Cell& cell = cells.back();
					cell.id = cellId;
					cellId++;

					int v0 = nodeMap[{i, j + 1}];
					int v1 = nodeMap[{i + 1, j}];
					int v2 = nodeMap[{i + 1, j + 1}];

					cell.vertices = { v0, v1, v2 };
					vector<tuple<int, int>> edgeVertices = {
						{v0, v1}, {v1, v2}, {v2, v0}
					};
					// Define edges of the cell
					for (const auto& ev : edgeVertices) {
						if (mapNodeToEdgeId.find(ev) == mapNodeToEdgeId.end()) {
							edges.push_back({ edgeId, get<0>(ev), get<1>(ev) });
							mapNodeToEdgeId[ev] = edgeId;
							mapNodeToEdgeId[{get<1>(ev), get<0>(ev)}] = edgeId;
							edgeId++;
						}
						cell.edges.insert(mapNodeToEdgeId[ev]);
					}
				}
			}
			globalCellId = (globalCellId + 1) % (K1 + K2);
		}
	}

	// Fill edges array of each node
	for (auto& node : nodes) {
		for (const auto& edge : edges) {
			bool matchNodes = node.id == edge.vertices[0] || node.id == edge.vertices[1];
			if (!matchNodes) continue;

			node.edges.insert(edge.id);
		}
	}

	// Set two cells that edge divides
	for (auto& edge : edges) {
		for (auto& cell : cells) {
			for (auto& cellEdgeId : cell.edges) {
				if (edge.id == cellEdgeId) {
					if (edge.cells[0] == -1) edge.cells[0] = cell.id;
					if (edge.cells[1] == -1 && edge.cells[0] != cell.id) edge.cells[1] = cell.id;
				}
			}
		}
	}
}

void FillGraph(const Mesh& mesh, SimpleGraph& graph) {
	for (const auto& [ev, edgeId] : mesh.mapNodeToEdgeId) {
		// добавляем ребро в граф, дублирование вычислений, но пусть так
		graph.AddEdge(get<0>(ev), get<1>(ev));

		// добавляем 1 на главную диагональ, каждая вершина графа сама себе сосед
		// для построения разреженной матрицы
		graph.AddEdge(get<0>(ev), get<0>(ev));
	}
}

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

int main() {
	Mesh mesh(6, 6, 3, 4, DivideCellsTriangle::LEFT);
	mesh.PrintSourceGrid();
	mesh.CreateTopology();

	SimpleGraph graph;
	graph.Init(mesh.nodes.size());
	// заполнение графа по заданной сетке
	FillGraph(mesh, graph);
	graph.PrintMatrixAdjacency();
	return 0;
}
