#pragma once
#include <vector>
#include <string>
#include <set>
using namespace std;

namespace GrNmbr
{
	struct edge
	{
		int to;
		double weight;
	};

	class Numbering;

	class Graph
	{
		vector<vector<edge>> gr;
	public:

		friend 	ostream& operator<< (ostream &c, Graph &gr);


		vector<vector<edge>> to_vect();
		int size();
		void consoleInput();
		void readFile(string _filename);
		Numbering createMnumb();
		Numbering createTnumb();

		Graph(vector<vector<edge>> _gr) : gr(_gr) {};
		Graph() {};
		vector<edge> operator[] (int index);
	};

	ostream& operator<< (ostream &c, Graph &gr);


	class Numbering
	{
		Graph *gr;
		vector<int> indices;
	public:
		Numbering() : gr(nullptr) {};
		Numbering(Graph *_gr);
		Numbering(Graph *_gr, vector<int> _indices);
		Graph* getGrp();
		int& operator[] (int index);
		int size();
	};

	class NumberationBuilder
	{
		vector<bool> visited;
		int curIndex;
		Graph *curGr;
		vector<int> indices;
		void Mbuild(int curVertex);
		void Nbuild(int curVertex);
		set<int> findNArea(int vertexNum, Numbering *N);
	public:
		
		friend ostream& operator<< (ostream &c, Numbering &x);

		vector<int> getLast();
		void demo();
		Numbering M(Graph *_gr, int start);
		Numbering N(Graph *_gr, int start);
		Numbering T(Graph *_gr, int start, Numbering *_N);
	};

	string getcd();

	ostream& operator<< (ostream &c, Numbering &x);


}

