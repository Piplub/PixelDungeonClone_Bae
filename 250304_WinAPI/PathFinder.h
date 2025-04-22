#pragma once
#include "config.h"

class Level;
struct PQNode
{
    // 우선순위 큐에서 f가 작은 순서로
    bool operator>(const PQNode& o) const { return f > o.f; }
    bool operator<(const PQNode& o) const { return f < o.f; }

    FPOINT pos;
    float g;  // 시작점부터 이 노드까지 비용
    float f;  // g + heuristic
};
class PathFinder
{
public:
	bool FindPath(FPOINT src, FPOINT dest, Level* level, vector<FPOINT>& path);
    float Heuristic(FPOINT& a, FPOINT b);
private :
	//vector<FPOINT> path;
};

