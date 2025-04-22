#include "PathFinder.h"
#include "Level.h"

bool PathFinder::FindPath(FPOINT src, FPOINT dest, Level* level, vector<FPOINT>& path)
{
    if (src == dest) return false;
    map<FPOINT, float> gScore;
    map<FPOINT, FPOINT> parent;

    priority_queue<PQNode, vector<PQNode>, greater<PQNode>> openSet;

    // 8방향(대각 포함) 이동 벡터
    static const FPOINT dirs[8] = {
        {0,-TILE_SIZE},{0,TILE_SIZE},
        {-TILE_SIZE,0},{TILE_SIZE,0},
        {TILE_SIZE,TILE_SIZE},{TILE_SIZE,-TILE_SIZE},
        {-TILE_SIZE,TILE_SIZE},{-TILE_SIZE,-TILE_SIZE}
    };

    // 1) 초기화
    gScore[src] = 0;
    float h0 = Heuristic(src, dest);
    openSet.push(PQNode{ src, 0, h0 });
    parent[src] = src;

    bool found = false;
    PQNode current;

    // 2) A* 메인 루프
    while (!openSet.empty())
    {
        current = openSet.top();
        openSet.pop();

        // 이미 더 좋은 경로로 처리했다면 무시
        if (current.f > gScore[current.pos] + Heuristic(current.pos, dest))
            continue;

        // 목적지 도착
        if (current.pos.x == dest.x && current.pos.y == dest.y) {
            found = true;
            break;
        }

        // 3) 이웃 노드 검사
        for (auto& d : dirs)
        {
            FPOINT next = { current.pos.x + d.x, current.pos.y + d.y };

            // 이동 불가 타일이면 건너뛰기
            if (!level->GetMap(next.x, next.y)->CanGo())
                continue;
            if (level->GetActorAt({ next.x, next.y }) && next != dest)
                continue;

            bool isDiag = (d.x != 0 && d.y != 0);

            // 코너 자르기 방지
            if (isDiag) {
                FPOINT s1{ current.pos.x + d.x, current.pos.y };
                FPOINT s2{ current.pos.x, current.pos.y + d.y };
                if (!level->GetMap(s1.x, s1.y)->CanGo() &&
                    !level->GetMap(s2.x, s2.y)->CanGo())
                    continue;
            }

            // 이동 비용
            float moveCost = isDiag ? 1.41421356f : 1.0f;
            float nextG = current.g + moveCost;

            auto it = gScore.find(next);
            if (it == gScore.end() || nextG < it->second)
            {
                // 더 좋은 경로 발견
                gScore[next] = nextG;
                float fScore = nextG + Heuristic(next, dest);
                openSet.push({ next, nextG, fScore });
                parent[next] = current.pos;
            }
        }
    }

    // 4) 경로 재구성
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
    // 월드 좌표 → 타일 좌표
    float dx = abs(a.x - b.x) / TILE_SIZE;
    float dy = abs(a.y - b.y) / TILE_SIZE;

    // diagonal distance
    float diag = min(dx, dy);
    float straight = dx + dy;
    // 실제 최단 경로 비용
    return straight + (1.41421356f - 2.f) * diag;
}
