#include "GraphNumbering.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace GrNmbr;

void GrNmbr::NumberationBuilder::Mbuild(int curVertex)
{
	indices[curVertex] = curIndex;
	visited[curVertex] = true;
	for (auto it : (*curGr)[curVertex])
	{
		if (!visited[it.to])
		{
			++curIndex;
			Mbuild(it.to);
		}
	}
}

void GrNmbr::NumberationBuilder::Nbuild(int curVertex)
{

	visited[curVertex] = true;
	for (auto it : (*curGr)[curVertex])
	{
		if (!visited[it.to])
		{
			Nbuild(it.to);
		}
	}
	indices[curVertex] = curIndex;
	--curIndex;
}

set<int> GrNmbr::NumberationBuilder::findNArea(int index, Numbering *N)
{
	
	set<int> n = { index };
	bool added = false;
	do {
		added = false;
		for (int i = 0; i < curGr->size(); ++i)
			for (int j = 0; j < (*curGr)[i].size(); ++j)
				if (n.find((*curGr)[i][j].to) != n.end() && n.find(i) == n.end() && (*N)[i] > (*N)[(*curGr)[i][j].to])
				{
					n.insert(i);
					added = true;
				}
	} while (added == true);
	n.erase(index);
	return n;
}

GrNmbr::Numbering::Numbering(Graph * _gr) : gr(_gr) 
{
	indices.resize(gr->size());
}

GrNmbr::Numbering::Numbering(Graph * _gr, vector<int> _indices)
{
	if (_gr->size() != _indices.size())
		return;
	gr = _gr;
	indices = _indices;
}


vector<int> GrNmbr::NumberationBuilder::getLast()
{
	return indices;
}

void GrNmbr::NumberationBuilder::demo()
{
	Graph gr;
	string filename;
	cout << "Enter filename (must be stored here : " << getcd() << " )" << endl;
	cin >> filename;
	gr.readFile(filename);
	cout << "Graph : " << endl;
	cout << gr;

	int start;
	cout << "Enter start vertex : " << endl;
	cin >> start;

	NumberationBuilder build;
	Numbering grM, grN, grT;

	grM = build.M(&gr, start);
	cout << "M-Numberation : " << endl;
	cout << grM;

	grN = build.N(&gr, start);
	cout << "N-Numberation : " << endl;
	cout << grN;


	grT = build.T(&gr, start, &grN);
	cout << "T-Numberation : " << endl;
	cout << grT;


}

Numbering GrNmbr::NumberationBuilder::M(Graph *_gr, int start)
{
	if (start >= _gr->size())
		return Numbering();
	curGr = _gr;
	indices.resize(_gr->size());
	visited = vector<bool>(_gr->size(), false); //!!!!!
	curIndex = 0;
	Mbuild(start);
	return Numbering(_gr, indices);
}

Numbering GrNmbr::NumberationBuilder::N(Graph * _gr, int start)
{
	if (start >= _gr->size())
		return Numbering();
	curGr = _gr;
	indices.resize(_gr->size());
	visited = vector<bool>(_gr->size(), false); //!!!!!
	curIndex = _gr->size()-1;
	Nbuild(start);
	return Numbering(_gr, indices);
}

Numbering GrNmbr::NumberationBuilder::T(Graph * _gr, int start, Numbering *_N)
{
	if (start >= _gr->size())
		return Numbering();
	if (_N == nullptr || _gr->size() != _N->size())
		*_N = N(_gr, start);
	Numbering T(_gr);
	for (int i = 0; i < T.size(); ++i)
		T[i] = -1;

	curGr = _gr;
	int NT = 0;
	indices.resize(_gr->size());
	set<int> NArea;
	for (int i = 0; i < _N->size(); ++i)
	{
		int p;
		for (int k = 0; k < _N->size(); ++k)
			if ((*_N)[k] == i)
			{
				p = k;
				break;
			}
		if (T[p] == -1)
			T[p] = NT++;
		NArea = findNArea(i, _N);
		for (auto j : NArea)
			T[j] = NT++;
	}
	return T;
}


Graph* GrNmbr::Numbering::getGrp()
{
	return gr;
}

int& GrNmbr::Numbering::operator[](int index)
{
	return indices[index];
}

int GrNmbr::Numbering::size()
{
	return indices.size();
}





vector<vector<edge>> GrNmbr::Graph::to_vect()
{
	return gr;
}

int GrNmbr::Graph::size()
{
	return gr.size();
}

void GrNmbr::Graph::consoleInput()
{
}

void GrNmbr::Graph::readFile(string _filename)
{
	ifstream fin(_filename, ios::in);

	if (!fin.is_open())
	{
	//	ErrorMessage("Couldn't open file.");
		return;
	}

	char buff[20], weight[20];
	int i = 0, to;
	while (!fin.eof())
	{
		fin >> buff;
		if (isdigit(buff[0]))
		{
			to = atoi(buff);
			if (gr.size() <= to)
				gr.resize(to + 1);

			fin >> weight;

			gr[i].push_back({ to, atof(weight)});
		}
		else {
			++i;
			if (gr.size() < i)
				gr.resize(i);
		}
	}
	fin.close();
}

GrNmbr::Numbering GrNmbr::Graph::createMnumb()
{
	NumberationBuilder numberate;
	return numberate.M(this, 0);
}

GrNmbr::Numbering GrNmbr::Graph::createTnumb()
{
	NumberationBuilder numberate;
	return numberate.T(this, 0, nullptr);
}

vector<edge> GrNmbr::Graph::operator[](int index)
{
	return gr[index];
}

ostream & GrNmbr::operator<<(ostream & c, Graph &gr)
{
	for (int i = 0; i < gr.size(); ++i)
	{
		c << "Vertex " << i << ": ";
		for (auto j : gr[i])
			c << "{ " << i << " -> " << j.to << " w = " << j.weight << " } ";
		c << endl;
	}
	c << endl;
	return c;
}

string GrNmbr::getcd()
{
	wchar_t *w = nullptr;
	wstring ws = _wgetcwd(w, _MAX_PATH);
	return string(ws.begin(), ws.end());
}

ostream & GrNmbr::operator<<(ostream & c, Numbering & x)
{
	c << "{ ";
	for (int i = 0; i < x.size(); ++i)
		c << x[i] << " ";
	c << "}" << endl;
	return c;
}

