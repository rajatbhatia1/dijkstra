#include <iostream>
#include <vector>
#include <set>
#include <climits>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <map>
#include <list>
#include <queue>
#include <cassert>

uint32_t absolutefinal = 0;
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
    absolutefinal = n;
    nodes.resize(n+1);
}


void add_(uint32_t n1, uint32_t n2, uint32_t w)
{
    nodes.at(n1).insert(n2);
    addWeight(n1, n2, w);

    #ifdef DEBUG
    std::cout << "nodes[" << n1 << "] ";
    for (auto &n: nodes.at(n1)) {
      std::cout << n << " ";
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
    add_(n1, n2, w);
    add_(n2, n1, w);
}

#if 0
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
            // copy if absolutefinal is reached
            if (back == absolutefinal) {
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

                        // copy if absolutefinal is reached
                        if (n == absolutefinal) {
                            #ifdef DEBUG
                            std::cout << "   Evaluating absolutefinal route=" << new_r << std::endl;
                            #endif
                            if (new_r.weight < shortest_route.weight) {
                                #ifdef DEBUG
                                std::cout << "   New absolutefinal route=" << new_r << std::endl;
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
#endif

struct Node {
  bool solved = false;
  bool all_solved = false;
  uint32_t n = 0;
  uint32_t current_weight = 0;
  uint32_t back;
    Node(bool _solved, uint32_t _n, uint32_t w, uint32_t _back):
      solved(_solved),
      n(_n),
      current_weight(w),
      back(_back)
  {
    // do nothing
  }
  Node(const Node &old) = default;

  Node() = default;
};
std::vector<Node> solved;
void compute() {
  

  bool completed = false;


 
  
  class NodeWeightCompare {
  public:
    bool operator() (Node &n1, Node &n2) {
      return n1.current_weight < n2.current_weight;
    }
  };

  std::priority_queue<Node, std::vector<Node>, NodeWeightCompare> current_nodes;
  solved.emplace_back(true, 1, 0, UINT_MAX);
  while (!completed) {
    if (absolutefinal==solved.back().n) {
      return;
    }
    bool added_something = false;
    for (auto &solved_node:solved) {
      for (auto &next_node:nodes[solved_node.n]) {
	current_nodes.push(Node(false, next_node, getWeight(solved_node.n, next_node)+solved_node.current_weight, solved_node.n));
	added_something = true;
      }
    }
    const Node chosen = current_nodes.top();
    nodes.at(chosen.n).erase(chosen.back);
    nodes.at(chosen.back).erase(chosen.n);
    solved.push_back(chosen);
    current_nodes.pop();
    while ((current_nodes.size()>0) && 
	   (current_nodes.top().n==solved.back().n)) {
      current_nodes.pop();
    }
    completed = !added_something;
  }
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
    // if (shortest_route.weight == INT_MAX)
    //     std::cout << -1 << std::endl;

    // std::copy(shortest_route.vertex.begin(), shortest_route.vertex.end(),
    //         std::ostream_iterator<uint32_t>(std::cout, " "));
    // std::cout << std::endl;

    //std::cout << "shortest path=" << shortest_route << std::endl;


    if (solved.back().n==absolutefinal) {
      std::vector<Node> allnodes(absolutefinal);
      for (auto &s:solved) {
	allnodes[s.n]=s;
      }
      uint32_t idx = absolutefinal;
      std::list<uint32_t> rpath;
      rpath.push_front(absolutefinal);
      do {
	idx = allnodes[idx].back;
	rpath.push_front(idx);
      } while (idx!=1);
      std::copy(rpath.begin(), rpath.end(), 
		std::ostream_iterator<uint32_t>(std::cout, " "));
    } else {
      std::cout << -1 << std::endl;
    }
    return 0;



}
