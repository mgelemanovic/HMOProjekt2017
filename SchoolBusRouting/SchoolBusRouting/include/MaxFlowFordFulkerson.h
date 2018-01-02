#define UNMATCHED 0xffffffff

#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <functional>

namespace SBR
{

	struct Graph {
		typedef long long flow_type;
		struct edge {
			int src, dst;
			flow_type capacity, flow;
			size_t rev;
		};
		int n;
		std::vector<std::vector<edge>> adj;
		std::vector<std::vector<int>> matched;
		Graph(int n) : n(n), adj(n) {
			matched.resize(n);
			//std::fill(matched.begin(), matched.end(), UNMATCHED);
		}
		void AddEdge(int src, int dst, flow_type capacity);
		int MaxFlow(int s, int t);
	};

	int mainTest();
}