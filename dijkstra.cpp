#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <map>
#include <list>

uint32_t final = 0;
std::vector<std::set<uint32_t>> nodes;

struct Route {
    std::vector<uint32_t> vertex;
    uint32_t weight;
};
#if 0
std::map<std::pair<uint32_t, uint32_t>, uint32_t> weights;
inline void addWeight(uint32_t n1, uint32_t n2, uint32_t w) {
  std::pair<uint32_t, uint32_t> node_p = std::make_pair(n1, n2);
  std::map<std::pair<uint32_t, uint32_t>, uint32_t>::iterator itr = weights.find(node_p);
  if (itr != weights.end()) {
    if (w < itr->second)
      itr->second = w;
  } else {
    weights.insert(std::make_pair(node_p, w));
  }
}

inline void sortWeight() { }

inline uint32_t getWeight(uint32_t n1, uint32_t n2) {
  std::pair<uint32_t, uint32_t> node_p = std::make_pair(n1, n2);
  std::map<std::pair<uint32_t, uint32_t>, uint32_t>::iterator itr = weights.find(node_p);
  return itr->second;
}
#endif

struct Edge {
  uint32_t n1;
  uint32_t n2;
  uint32_t w;
  Edge(uint32_t _n1, uint32_t _n2, uint32_t _w):
    n1(_n1),
    n2(_n2),
    w(_w)
  {
    // do nothing
  }
};
std::list<Edge> edges;

inline void addWeight(uint32_t n1, uint32_t n2, uint32_t w) {
  edges.emplace_back(n1, n2, w);
}

inline void sortWeight() { 
  edges.sort([](const Edge &a, const Edge &b){return a.n1 < b.n1;});
}

inline uint32_t getWeight(uint32_t n1, uint32_t n2) {
  for(auto &e:edges) {
    if ((e.n1==n1) && (e.n2==n2)) {
      return e.w;
    }
  }
  return UINT_MAX;
}

std::ostream& operator<<(std::ostream &os, const Route &r)
{
  std::copy(r.vertex.begin(), r.vertex.end(), std::ostream_iterator<uint32_t>(os, " "));
  os << " weight=" << r.weight;
  return os;
}

Route shortest_route;

class Compare {
    public:
        bool operator() (Route &a, Route &b)
        {
            return a.weight < b.weight;
        }
};

std::vector<Route> routes;
std::vector<Route> routes_tmp;

void init(uint32_t n, uint32_t m)
{
    final = n;
    nodes.resize(n+1);
}


void add_(uint32_t n1, uint32_t n2, uint32_t w)
{
    nodes.at(n1).insert(n2);
    addWeight(n1, n2, w);

    #ifdef DEBUG
    std::cout << "node[" << n1 << "] ";
    for (auto &n: nodes.at(n1)) {
        std::cout << n;
    }
    std::cout << std::endl;
    #endif

    ////std::cout << "previous n1=" << n1 << " n2=" << n2 << " weight=" << weights[n1][n2] << " w=" << w << std::endl;
    //if (w < weights[n1][n2]) {
    //    //std::cout << "adding n1=" << n1 << " n2=" << n2 << " weight=" << w << std::endl;
    //    weights[n1][n2] = w;
    //}
    //std::cout << "added n1=" << n1 << " n2=" << n2 << " weight=" << weights[n1][n2] << std::endl;
}
void add(uint32_t n1, uint32_t n2, uint32_t w)
{
  if (n1 < n2) {
    add_(n1, n2, w);
  } else if (n1>n2) {
    add_(n2, n1, w);
  }
}

void compute()
{

    if (nodes.size() == 0) {
        return;
    }

    for (auto &v:nodes[1]) {
        Route r;
        r.vertex = {1, v};
	r.weight = getWeight(1, v);
        routes.push_back(r);
    }

    std::set<uint32_t> visited;

    bool completed = false;

    while (!completed) {
        bool routes_added = false;
        #ifdef DEBUG
        for (auto &r: routes) {
            std::cout << "Current Routes" << std::endl;
            std::cout << r << std::endl;
        }
        #endif
        for (auto &r: routes) {
            #ifdef DEBUG
            std::cout << " Evaluating " << r << std::endl;
            #endif
            uint32_t back = r.vertex.back();
            // copy if final is reached
            if (back == final) {
                if (r.weight < shortest_route.weight) {
                    shortest_route = r;
                }
            }

            if (!nodes[back].empty()) {
                // add new vertex
                #ifdef DEBUG
                std::cout << "back = " << back << std::endl;
                std::cout << "nodes size : " << nodes[back].size() << std::endl;
                for (auto itr=nodes[back].begin(); itr!= nodes[back].end(); ++itr) {
                    std::cout << " Will Learn node " << *itr << std::endl;
                }
                // for (auto &n: nodes[back]) {
                //     std::cout << " Will Learn node " << n << std::endl;
                // }
                #endif
                for (auto &n: nodes[back]) {
                    #ifdef DEBUG
                    std::cout << "  Learning node " << n << " back=" << back <<  std::endl;
                    #endif
                    if (std::find(r.vertex.begin(), r.vertex.end(), n) == r.vertex.end()) {
                        Route new_r = r;
                        new_r.vertex.push_back(n);
                        new_r.weight += getWeight(back, n);

                        // copy if final is reached
                        if (n == final) {
                            #ifdef DEBUG
                            std::cout << "   Evaluating final route=" << new_r << std::endl;
                            #endif
                            if (new_r.weight < shortest_route.weight) {
                                #ifdef DEBUG
                                std::cout << "   New final route=" << new_r << std::endl;
                                #endif
                                shortest_route = new_r;
                            }
                        } else {
                            #ifdef DEBUG
                            std::cout << "   Adding route=" << new_r << std::endl;
                            #endif
                            routes_tmp.push_back(new_r);
                            routes_added = true;
                        }
                    }
                }
            }

            if (!routes_added) {
                completed = true;
            }
        } // for (auto routes)
        #ifdef DEBUG
        std::cout << "-------------------------------------" << std::endl;
        #endif
        routes = std::move(routes_tmp);
        routes_tmp.clear();
    } // while (!completed)
}

int main() {
    shortest_route.weight = INT_MAX;
#ifdef TESTING
    // init(5, 6);
    // add(1, 2, 2);
    // add(2, 5, 5);
    // add(2, 3, 4);
    // add(1, 4, 1);
    // add(4, 3, 3);
    // add(3, 5, 1);
    init(10, 10);
    sortWeight();
    add(1, 5, 12);
    add(2, 4, 140);
    add(2, 10, 149);
    add(3, 6, 154);
    add(3, 7, 9);
    add(3, 8, 226);
    add(3, 10, 132);
    add(4, 10, 55);
    add(5, 8, 33);
    add(7, 8, 173);
    compute();
#else
    uint32_t n, m;
    std::cin >> n >> m;
    init(n, m);
    sortWeight();
    uint32_t a1, a2, w;
    for (uint32_t i = 0; i < m; ++i)
    {
        std::cin >> a1 >> a2 >> w;
        add(a1, a2, w);
    }
    compute();
#endif
    if (shortest_route.weight == INT_MAX)
        std::cout << -1 << std::endl;

    std::copy(shortest_route.vertex.begin(), shortest_route.vertex.end(),
            std::ostream_iterator<uint32_t>(std::cout, " "));
    std::cout << std::endl;

    //std::cout << "shortest path=" << shortest_route << std::endl;
}
