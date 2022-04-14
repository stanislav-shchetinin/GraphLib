#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

const long long NO_WAY = 9e18, NEGATIVE_CYCLE = -9e18;

//��������� ����� (��� ������ ������, ���, �� ��������� ��� ����� 1)
struct Edge {
    int firstVert = -1, secondVert = -1;
    long long weight = 1;
    Edge() {}
    Edge(int firstVert, int secondVert, long long weight) :firstVert(firstVert), secondVert(secondVert), weight(weight) {}
    Edge(int firstVert, int secondVert) :firstVert(firstVert), secondVert(secondVert) {}

    //��� ����� ������, ���� ��������� ������ � ����� � ���
    friend const bool operator==(const Edge& x, const Edge& y) {
        return bool(x.firstVert == y.firstVert && x.secondVert == y.secondVert && x.weight == y.weight);
    }
    //����� �����
    friend std::ostream& operator<<(std::ostream& cout, const Edge& edge) {
        cout << "{ first_vert: " << edge.firstVert << ", second_vert: " << edge.secondVert << ", weight: " << edge.weight << " }";
        return cout;
    }
    //���� �����
    friend std::istream& operator>>(std::istream& cin, Edge& edge) {
        cin >> edge.firstVert >> edge.secondVert >> edge.weight;
        return cin;
    }
    //--edge ��������� �� ������� ������ ������
    Edge& operator--() {
        --firstVert;
        --secondVert;
        return *this;
    }
};

//��������� �������, ������� ��������� ������ � ������ �������� � ������ ���������� (����� �������, ���, �� ��������� ����� 1)
struct SecondVert {
    int vert = -1;
    long long weight = 1;
    SecondVert() {}
    SecondVert(int vert) :vert(vert) {}
    SecondVert(int vert, long long weight) :vert(vert), weight(weight) {}

    //����� ��� ������������� set
    friend const bool operator<(const SecondVert& x, const SecondVert& y) {
        return bool((x.weight < y.weight) || (x.weight == y.weight && x.vert < y.vert));
    }

};

//��������� ��� ���������� ����� (����� ���������� ��������� ���� ���������� � ���������� ����������
//����� ����� ���������, ���������� ���������� �� ��������� �� ���� ���������, ������ ������, 
//������� ������������ �� ���� ���������� ���� ����� ���������)
struct ResultShortWay {
    long long distStartFinish = NO_WAY;
    std::vector <long long> dist;
    std::vector <int> shortWay;
    ResultShortWay() {}
};

//��������� ������ ��� �������� ������� � ������ � ����������
struct ParentSalesman {
    long long mask;
    int last;
    ParentSalesman() {}
    ParentSalesman(long long mask, int last) :mask(mask), last(last) {}

    friend const bool operator!=(const ParentSalesman& x, const ParentSalesman& y) {
        return bool(x.mask != y.mask || x.last != y.last);
    }

};

std::vector <Edge> vertCompression(std::vector <Edge>);
std::vector <std::vector <SecondVert>> fromListOfEdgesToListOfAdjacencies(std::vector <Edge>, bool isOrientedEdge = false);
std::vector <Edge> fromListOfAdjacenciesToListOfEdges(std::vector <std::vector <SecondVert>> listOfAdjacencies);
std::vector <std::vector <int>> fromListOfEdgesToAdjacencyMatrix(std::vector <Edge>, int valNoEdge = 1e9, bool isOrientedEdge = false);
std::vector <Edge> fromAdjacencyMatrixToListOfEdges(std::vector <std::vector <int>>, int valNoEdge);
std::vector <std::vector<int>> fromListOfAdjacenciesToAdjacencyMatrix(std::vector <std::vector<SecondVert>>, int valNoEdge);
std::vector <std::vector<SecondVert>> fromAdjacencyMatrixToListOfAdjacencies(std::vector <std::vector <int>>, int);
ResultShortWay shortWayDijkstra(int, int, std::vector <std::vector<SecondVert>>);
ResultShortWay shortWayFordBellman(int, int, std::vector <std::vector<SecondVert>>);
std::vector <std::vector <long long>> shortWayFloyd(std::vector <std::vector <long long>>);
ResultShortWay shortWayBFS(int, int, std::vector <std::vector <SecondVert>>);
ResultShortWay salesmanExact(int startVert, std::vector <std::vector <int>>);
ResultShortWay salesmanFast(std::vector <Edge>, int);
