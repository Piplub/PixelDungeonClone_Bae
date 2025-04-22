#pragma once
#include "config.h"

class Level;
struct PQNode
{
    // �켱���� ť���� f�� ���� ������
    bool operator>(const PQNode& o) const { return f > o.f; }
    bool operator<(const PQNode& o) const { return f < o.f; }

    FPOINT pos;
    float g;  // ���������� �� ������ ���
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

