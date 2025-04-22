#include "PathFinder.h"
#include "Level.h"

bool PathFinder::FindPath(FPOINT src, FPOINT dest, Level* level, vector<FPOINT>& path)
{
    if (src == dest) return false;
    map<FPOINT, float> gScore;
    map<FPOINT, FPOINT> parent;

    priority_queue<PQNode, vector<PQNode>, greater<PQNode>> openSet;

    // 8����(�밢 ����) �̵� ����
    static const FPOINT dirs[8] = {
        {0,-TILE_SIZE},{0,TILE_SIZE},
        {-TILE_SIZE,0},{TILE_SIZE,0},
        {TILE_SIZE,TILE_SIZE},{TILE_SIZE,-TILE_SIZE},
        {-TILE_SIZE,TILE_SIZE},{-TILE_SIZE,-TILE_SIZE}
    };

    // 1) �ʱ�ȭ
    gScore[src] = 0;
    float h0 = Heuristic(src, dest);
    openSet.push(PQNode{ src, 0, h0 });
    parent[src] = src;

    bool found = false;
    PQNode current;

    // 2) A* ���� ����
    while (!openSet.empty())
    {
        current = openSet.top();
        openSet.pop();

        // �̹� �� ���� ��η� ó���ߴٸ� ����
        if (current.f > gScore[current.pos] + Heuristic(current.pos, dest))
            continue;

        // ������ ����
        if (current.pos.x == dest.x && current.pos.y == dest.y) {
            found = true;
            break;
        }

        // 3) �̿� ��� �˻�
        for (auto& d : dirs)
        {
            FPOINT next = { current.pos.x + d.x, current.pos.y + d.y };

            // �̵� �Ұ� Ÿ���̸� �ǳʶٱ�
            if (!level->GetMap(next.x, next.y)->CanGo())
                continue;
            if (level->GetActorAt({ next.x, next.y }) && next != dest)
                continue;

            bool isDiag = (d.x != 0 && d.y != 0);

            // �ڳ� �ڸ��� ����
            if (isDiag) {
                FPOINT s1{ current.pos.x + d.x, current.pos.y };
                FPOINT s2{ current.pos.x, current.pos.y + d.y };
                if (!level->GetMap(s1.x, s1.y)->CanGo() &&
                    !level->GetMap(s2.x, s2.y)->CanGo())
                    continue;
            }

            // �̵� ���
            float moveCost = isDiag ? 1.41421356f : 1.0f;
            float nextG = current.g + moveCost;

            auto it = gScore.find(next);
            if (it == gScore.end() || nextG < it->second)
            {
                // �� ���� ��� �߰�
                gScore[next] = nextG;
                float fScore = nextG + Heuristic(next, dest);
                openSet.push({ next, nextG, fScore });
                parent[next] = current.pos;
            }
        }
    }

    // 4) ��� �籸��
    path.clear();
    if (!found) return false;

    FPOINT step = dest;
    while (true)
    {
        path.push_back(step);
        if (step.x == parent[step].x && step.y == parent[step].y)
            break;
        step = parent[step];
    }
    reverse(path.begin(), path.end());
    return true;
}

float PathFinder::Heuristic(FPOINT& a, FPOINT b)
{
    // ���� ��ǥ �� Ÿ�� ��ǥ
    float dx = abs(a.x - b.x) / TILE_SIZE;
    float dy = abs(a.y - b.y) / TILE_SIZE;

    // diagonal distance
    float diag = min(dx, dy);
    float straight = dx + dy;
    // ���� �ִ� ��� ���
    return straight + (1.41421356f - 2.f) * diag;
}
