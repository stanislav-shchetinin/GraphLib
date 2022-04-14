#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <queue>
#include <random>

const long long NO_WAY = 9e18, NEGATIVE_CYCLE = -9e18;

//рандомное число между 0 и 1 (нецелое)
long double rand01() {
    return (long double)(rand()) / RAND_MAX;
}

//генератор случайных чисел
std::mt19937 rnd(time(0));

//Структура ребра (два номера вершин, вес, по умолчанию вес равен 1)
struct Edge {
    int firstVert = -1, secondVert = -1;
    long long weight = 1;
    Edge() {}
    Edge(int firstVert, int secondVert, long long weight) :firstVert(firstVert), secondVert(secondVert), weight(weight) {}
    Edge(int firstVert, int secondVert) :firstVert(firstVert), secondVert(secondVert) {}

    //Два ребра равные, если совпадают начала и концы и вес
    friend const bool operator==(const Edge& x, const Edge& y) {
        return bool(x.firstVert == y.firstVert && x.secondVert == y.secondVert && x.weight == y.weight);
    }
    //Вывод ребра
    friend std::ostream& operator<<(std::ostream& cout, const Edge& edge) {
        cout << "{ first_vert: " << edge.firstVert << ", second_vert: " << edge.secondVert << ", weight: " << edge.weight << " }";
        return cout;
    }
    //Ввод ребра
    friend std::istream& operator>>(std::istream& cin, Edge& edge) {
        cin >> edge.firstVert >> edge.secondVert >> edge.weight;
        return cin;
    }
    //--edge уменьшает на единицу номера вершин
    Edge& operator--() {
        --firstVert;
        --secondVert;
        return *this;
    }
    //Для сортировки
    friend const bool operator<(const Edge& x, const Edge& y) {
        return bool(x.firstVert < y.firstVert || x.firstVert == y.firstVert && x.secondVert < y.secondVert ||
            x.firstVert == y.firstVert && x.secondVert == y.secondVert && x.weight < y.weight);
    }
};


//Структура вершины, которая соеденена ребром с первой вершиной в списке смежностей (номер вершины, вес, по умолчанию равен 1)
struct SecondVert {
    int vert = -1;
    long long weight = 1;
    SecondVert() {}
    SecondVert(int vert) :vert(vert) {}
    SecondVert(int vert, long long weight) :vert(vert), weight(weight) {}

    //Нужно для использование set
    friend const bool operator<(const SecondVert& x, const SecondVert& y) {
        return bool((x.weight < y.weight) || (x.weight == y.weight && x.vert < y.vert));
    }

};

//Структура для кратчайших путей (после выполнения алгоритма есть информация о кратчайшем расстоянии
//между двумя вершинами, кратчайшем расстоянии от стартовой до всех остальных, номера вершин, 
//которые представляют из себя кратчайший путь между вершинами)
struct ResultShortWay {
    long long distStartFinish = NO_WAY;
    std::vector <long long> dist;
    std::vector <int> shortWay;
    ResultShortWay() {}
    ResultShortWay(long long distStartFinish, std::vector <int> shortWay) :distStartFinish(distStartFinish), shortWay(shortWay) {}
};

//Структура данных для хранения предков в задача о комивояжёре
struct ParentSalesman {
    long long mask;
    int last;
    ParentSalesman() {}
    ParentSalesman(long long mask, int last) :mask(mask), last(last) {}

    friend const bool operator!=(const ParentSalesman& x, const ParentSalesman& y) {
        return bool(x.mask != y.mask || x.last != y.last);
    }

};

//Функция проверки минимального и максимального номера вершины (используется для преоброзования представления графа из списка ребер)
//maxAllowedVal - максимальное допустимое значение
//Так же функция возвращает максимальный номер вершины в графе
int checkNumVec(std::vector <Edge> listOfEdges, int maxAllowedVal) {

    int maxNumVec = -1; //Максимальный номер ребра, изначально даем минимально возмжное значение, т.е. -1
    int minNumVec = maxAllowedVal + 1; //Минимальный номер ребра, изначально даем минимально возможное значение, т.е 10^6 + 1
    for (size_t i = 0; i < listOfEdges.size(); ++i) {
        //Максимальный номер вершины будет максимумом из себя и двух вершин, которыми задано ребро номер i
        //Минимальный номер вершины будет минимумом из себя и двух вершин, которыми задано ребро номер i
        maxNumVec = std::max({ maxNumVec, listOfEdges[i].firstVert, listOfEdges[i].secondVert });
        minNumVec = std::min({ minNumVec, listOfEdges[i].firstVert, listOfEdges[i].secondVert });
    }

    try {
        if (maxNumVec > maxAllowedVal) {
            throw 1;
        }
        if (minNumVec < 0) {
            throw 2;
        }
    }
    catch (int numError) {

        //Максимальный/минимальный номер вершины слишком маленький/большой, предлагается сжать номера

        if (numError == 1) {
            std::cout << "Error #" << numError <<
                " - The vertex number is too large. If the number is not important, compress the numbers using the function compressVerNumForListOfEdges"
                << std::endl;
        }
        else {
            std::cout << "Error #" << numError <<
                " - The vertex number is too small. If the number is not important, compress the numbers using the function compressVerNumForListOfEdges"
                << std::endl;
        }

    }
    return maxNumVec;
}

//Индексация ведется с 0

//Функция сжатия номеров вершин в графе, представленном списком ребер
std::vector <Edge> vertCompression(std::vector <Edge> listOfEdges) {
    
    int curNum = 0;
    std::map <int, int> map;

    //Сопостовление номерам вершин числу от 0 до количества вершин
    for (size_t i = 0; i < listOfEdges.size(); ++i) {

        if (!map[listOfEdges[i].firstVert]) {
            map[listOfEdges[i].firstVert] = curNum;
            ++curNum;
        }
        
        if (!map[listOfEdges[i].secondVert]) {
            map[listOfEdges[i].secondVert] = curNum;
            ++curNum;
        }

    }

    std::vector <Edge> result(listOfEdges.size());

    for (size_t i = 0; i < listOfEdges.size(); ++i) {
        result[i] = { map[listOfEdges[i].firstVert], map[listOfEdges[i].secondVert], listOfEdges[i].weight };
    }

    return result;
    
}

//Изменение представления графа
//Перевод из списка ребер в список cмежностей
//Максимальный номер вершины графы не должен превышать 10^6 минимальный номер не может быть меньше 0
//isOrientedEdge = 0, если граф неориентированный, иначе = 1 (по умолчанию равен 0)

std::vector <std::vector <SecondVert>> fromListOfEdgesToListOfAdjacencies(std::vector <Edge> listOfEdges, bool isOrientedEdge = false) {

    int maxNumVec = checkNumVec(listOfEdges, (int)1e6);

    std::vector <std::vector <SecondVert>> result(maxNumVec + 1);

    for (int i = 0; i < listOfEdges.size(); ++i) {
        result[listOfEdges[i].firstVert].push_back({ listOfEdges[i].secondVert, listOfEdges[i].weight });
        if (!isOrientedEdge) {
            result[listOfEdges[i].secondVert].push_back({ listOfEdges[i].firstVert, listOfEdges[i].weight });
        }
    }
    
    return result;
}

//Перевод из списка смежностей в список ребер
std::vector <Edge> fromListOfAdjacenciesToListOfEdges(std::vector <std::vector <SecondVert>> listOfAdjacencies) {
    std::vector <Edge> result;
    for (size_t firstVert = 0; firstVert < listOfAdjacencies.size(); ++firstVert) {
        for (size_t i = 0; i < listOfAdjacencies[firstVert].size(); ++i) {
            result.push_back({ (int)firstVert, listOfAdjacencies[firstVert][i].vert, listOfAdjacencies[firstVert][i].weight });
        }
    }
    return result;
}

//Перевод из списка ребер в матрицу смежностей
//Максимальное значение = 10^3, так как иначе будет использовано слишком много памяти
//valNoEdge - значение, которое обозначает, что между i-ой строкой и j-ым столбом (i, j - номера вершин графа) нету ребра, по умолчанию равно 1e9                    
std::vector <std::vector <int>> fromListOfEdgesToAdjacencyMatrix(std::vector <Edge> listOfEdges, int valNoEdge = 1e9, bool isOrientedEdge = false) {
    int maxNumVec = checkNumVec(listOfEdges, (int)1e3);
    std::vector <std::vector <int>> result(maxNumVec + 1, std::vector <int>(maxNumVec + 1, valNoEdge));
    
    for (size_t i = 0; i < listOfEdges.size(); ++i) {
        result[listOfEdges[i].firstVert][listOfEdges[i].secondVert] = listOfEdges[i].weight;
        if (!isOrientedEdge) {
            result[listOfEdges[i].secondVert][listOfEdges[i].firstVert] = listOfEdges[i].weight;
        }
    }

    return result;

}

//Перевод из матрицы смежности в список ребер
std::vector <Edge> fromAdjacencyMatrixToListOfEdges(std::vector <std::vector <int>> adjacencyMatrix, int valNoEdge) {
    std::vector <Edge> result;

    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix.size(); ++j) {
            if (adjacencyMatrix[i][j] != valNoEdge) {
                result.push_back({ (int)i, (int)j, adjacencyMatrix[i][j] });
            }
        }
    }

    return result;

}

//Перевод из списка смежностей в матрицу смежностей
std::vector <std::vector<int>> fromListOfAdjacenciesToAdjacencyMatrix(std::vector <std::vector<SecondVert>> listOfAdjacencies, int valNoEdge) {
    int maxNumVec = listOfAdjacencies.size(), minNumVec = 0;
    int maxAllowedVal = (int)1e3;
    try {
        if (maxNumVec > maxAllowedVal) {
            throw 1;
        }
        if (minNumVec < 0) {
            throw 2;
        }
    }
    catch (int numError) {

        //Максимальный/минимальный номер вершины слишком маленький/большой, предлагается сжать номера

        if (numError == 1) {
            std::cout << "Error #" << numError <<
                " - The vertex number is too large. If the number is not important, compress the numbers using the function compressVerNumForListOfEdges"
                << std::endl;
        }
        else {
            std::cout << "Error #" << numError <<
                " - The vertex number is too small. If the number is not important, compress the numbers using the function compressVerNumForListOfEdges"
                << std::endl;
        }

    }
    std::vector <std::vector<int>> result(maxNumVec + 1, std::vector<int>(maxNumVec + 1, valNoEdge));

    for (size_t firstVert = 0; firstVert < listOfAdjacencies.size(); ++firstVert) {
        for (size_t i = 0; i < listOfAdjacencies[firstVert].size(); ++i) {
            result[(int)firstVert][listOfAdjacencies[firstVert][i].vert] = listOfAdjacencies[firstVert][i].weight;
        }
    }

    return result;

}

//Перевод из матрицы смежностей в список смежностей
std::vector <std::vector<SecondVert>> fromAdjacencyMatrixToListOfAdjacencies(std::vector <std::vector <int>> adjacencyMatrix, int valNoEdge) {
    std::vector <std::vector <SecondVert>> result(adjacencyMatrix.size());
    for (size_t i = 0; i < adjacencyMatrix.size(); ++i) {
        for (size_t j = 0; j < adjacencyMatrix.size(); ++j) {
            if (adjacencyMatrix[i][j] != valNoEdge) {
                result[i].push_back({ (int)j,  adjacencyMatrix[i][j] });
            }
        }
    }
    return result;

}

//Функция поиска минимального пути (Алгоритм Дейкстры), если ответом является NO_WAY значит пути нет
//Граф задан списком смежностей
ResultShortWay shortWayDijkstra(int startVer, int finishVer, std::vector <std::vector<SecondVert>> graph) {
    ResultShortWay result;
    std::vector <int> parent;
    int n = graph.size();

    result.dist.assign(n, NO_WAY);
    parent.assign(n, -1);
    result.dist[startVer] = 0;

    std::set <SecondVert> set;
    set.insert(SecondVert(startVer, 0));

    while (!set.empty()) {
        SecondVert curSecondVert = *set.begin();
        int vert = curSecondVert.vert;

        set.erase(curSecondVert);

        for (SecondVert update : graph[vert]) {
            if (result.dist[vert] + update.weight < result.dist[update.vert]) {

                set.erase(SecondVert(update.vert, result.dist[update.vert]));
                result.dist[update.vert] = result.dist[vert] + update.weight;
                parent[update.vert] = vert;
                set.insert(SecondVert(update.vert, result.dist[update.vert]));

            }
        }

    }

    int curVert = finishVer;
    while (curVert != -1) {
        result.shortWay.push_back(curVert);
        curVert = parent[curVert];
    }

    std::reverse(result.shortWay.begin(), result.shortWay.end());

    result.distStartFinish = result.dist[finishVer];

    return result;
}

void dfs(int ver, std::vector <bool>& used, std::vector <std::vector<SecondVert>>& graph, std::set <int>& verNegativeCycleCol) {
    used[ver] = true;
    verNegativeCycleCol.insert(ver);
    for (auto x : graph[ver]) {
        if (!used[x.vert]) {
            dfs(x.vert, used, graph, verNegativeCycleCol);
        }
    }
}

//Поиск кратчайшего пути (Алгоритм Форда-Беллмана)
//Граф задан списком смежностей
ResultShortWay shortWayFordBellman(int startVer, int finishVer, std::vector <std::vector<SecondVert>> graph) {

    ResultShortWay result;
    int n = graph.size();
    std::vector <int> parent(n, -1);
    result.dist.assign(n, NO_WAY);
    result.dist[startVer] = 0;
    std::vector <Edge> edge = fromListOfAdjacenciesToListOfEdges(graph);

    //Итерации релаксации
    for (int i = 0; i < n - 1; ++i) {
        for (auto curEd : edge) {
            if (result.dist[curEd.firstVert] < NO_WAY) {
                if (result.dist[curEd.secondVert] > result.dist[curEd.firstVert] + curEd.weight) {
                    result.dist[curEd.secondVert] = result.dist[curEd.firstVert] + curEd.weight;
                    parent[curEd.secondVert] = curEd.firstVert;
                }
            }
        }
    }

    //вершины, которые входят в отриц цикл
    std::set <int> verNegativeCycleMain, verNegativeCycleCol;

    for (auto curEd : edge) {
        if (result.dist[curEd.firstVert] < NO_WAY) {
            if (result.dist[curEd.secondVert] > result.dist[curEd.firstVert] + curEd.weight) {
                verNegativeCycleMain.insert(curEd.firstVert);
                verNegativeCycleMain.insert(curEd.secondVert);
            }
        }
    }

    std::vector <bool> used(n, false);
    for (auto x : verNegativeCycleMain) {
        dfs(x, used, graph, verNegativeCycleCol);
    }

    for (int i = 0; i < n; ++i) {
        if (verNegativeCycleCol.find(i) != verNegativeCycleCol.end()) {
            result.dist[i] = NEGATIVE_CYCLE;
        }
    }
    result.distStartFinish = result.dist[finishVer];

    int curVer = finishVer;

    while (curVer != -1) {
        result.shortWay.push_back(curVer);
        curVer = parent[curVer];
    }

    std::reverse(result.shortWay.begin(), result.shortWay.end());

    return result;
}

//Поиск кратчайшего пути (Алгоритм Форда-Беллмана)
//Граф задан матрицей смежностей
//Возвращает матрицу, в которой на пересечении i, j кратчайший путь от вершины i до вершины j
std::vector <std::vector <long long>> shortWayFloyd(std::vector <std::vector <long long>> graph) {
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        graph[i][i] = std::min(graph[i][i], 0LL);
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (graph[i][k] < NO_WAY && graph[k][j] < NO_WAY && k != i && k != j && i != j) {
                    graph[i][j] = std::min(graph[i][j], graph[i][k] + graph[k][j]);
                }
            }
        }
    }

    return graph;

}

//Функция поиска кратчайшего пути (только для невзешанных графов)
//Граф подается ввиде списка смежностей
ResultShortWay shortWayBFS(int startVer, int finishVert, std::vector <std::vector <SecondVert>> graph) {
    int n = graph.size();
    ResultShortWay result;
    result.dist.assign(n, NO_WAY);

    std::queue <int> q;
    q.push(startVer);
    result.dist[startVer] = 0;
    std::vector <int> parent(n, -1);

    while (!q.empty()) {
        int ver = q.front();
        q.pop();

        if (result.dist[ver] != NO_WAY) {
            for (auto curVer : graph[ver]) {
                if (result.dist[curVer.vert] > result.dist[ver] + 1) {
                    result.dist[curVer.vert] = result.dist[ver] + 1;
                    parent[curVer.vert] = ver;
                    q.push(curVer.vert);
                }
            }
        }

    }

    result.distStartFinish = result.dist[finishVert];

    int curVer = finishVert;
    while (curVer != -1) {
        result.shortWay.push_back(curVer);
        curVer = parent[curVer];
    }

    std::reverse(result.shortWay.begin(), result.shortWay.end());

    return result;

}

//Точная, но медленная реализация задачи о комивояжёре
//Граф задается матрицей смежности
//Не стоит использовать, если количество вершин в графе больше 20
ResultShortWay salesmanExact(int startVert, std::vector <std::vector <int>> graph) {

    int n = graph.size();
    ResultShortWay result;
    std::vector <std::vector <long long>> dp((long long)(1 << n), std::vector <long long>(n, NO_WAY));
    std::vector <std::vector <ParentSalesman>> parent((long long)(1 << n), std::vector <ParentSalesman>(n, ParentSalesman(-1, -1)));

    dp[0][0] = 0;
    dp[(1 << startVert)][startVert] = 0;

    for (long long mask = 2; mask < (long long)(1 << n); ++mask) {
        for (int last = 0; last < n; ++last) {
            if ((mask & (1 << last)) == 0) continue;
            if (mask == (1 << last)) continue;
            long long shortWay = NO_WAY;
            for (int k = 0; k < n; ++k) {
                if (graph[k][last] <= 0) continue;

                if (shortWay > dp[mask ^ (1 << last)][k] + graph[k][last]) {
                    shortWay = dp[mask ^ (1 << last)][k] + graph[k][last];
                    parent[mask][last] = { mask ^ (1 << last), k };
                }

            }

            if (shortWay < NO_WAY) {
                dp[mask][last] = std::min(shortWay, dp[mask][last]);
            }
        }

    }

    ParentSalesman lastVer = ParentSalesman((long long)(1 << n) - 1, 0);

    for (int last = 1; last < n; ++last) {
        if (dp[(1 << n) - 1][last] < dp[(1 << n) - 1][lastVer.last]) {
            lastVer.last = last;
        }
    }

    result.distStartFinish = dp[lastVer.mask][lastVer.last];
    result.shortWay.push_back(lastVer.last);

    while (parent[lastVer.mask][lastVer.last] != ParentSalesman(-1, -1)) {
        lastVer = parent[lastVer.mask][lastVer.last];
        result.shortWay.push_back(lastVer.last);
    }

    std::reverse(result.shortWay.begin(), result.shortWay.end());

    return result;

}

long long sumDist(std::vector <int> result, std::vector <Edge>& graph) {
    long long dist = 0;
    for (int i = 0; i < result.size() - 1; ++i) {
        auto it = std::lower_bound(graph.begin(), graph.end(), Edge(result[i], result[i + 1], 0));
        if (it == graph.end()) {
            dist = NO_WAY;
            break;
        }
        Edge curEdge = *it;
        if (curEdge.firstVert != result[i] || curEdge.secondVert != result[i + 1]) {
            dist = NO_WAY;
            break;
        }
        else {
            dist += curEdge.weight;
        }
    }
    return dist;
}

//Быстрая, но менее точная реализация задачи о комивояжёре
//Стартовая вершина = 0
//Граф задается списком ребер
ResultShortWay salesmanFast(std::vector <Edge> graph, int cntVert) {

    int n = cntVert;
    if (n == 1) {
        return ResultShortWay(0, { 0 });
    }
    std::sort(graph.begin(), graph.end());
    std::vector <int> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = i;
    }
    std::shuffle(result.begin() + 1, result.end(), rnd);
    long long ans = sumDist(result, graph);

    long double temp = 1;
    int cntIter = 10000;

    while (cntIter--) {
        temp *= 0.99;
        std::vector <int> curResult = result;
        int randIndFirst = rand() % n, randIndSecond = rand() % n;
        if (randIndFirst == 0) {
            ++randIndFirst;
        }
        if (randIndSecond == 0) {
            ++randIndSecond;
        }
        std::swap(curResult[randIndFirst], curResult[randIndSecond]);
        long long curAns = sumDist(curResult, graph);

        if (curAns < ans || rand01() < std::exp((ans - curAns) / temp)) {
            result = curResult;
            ans = curAns;
        }

    }

    return ResultShortWay(ans, result);

}