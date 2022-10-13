#include <cstdio>
#include <cmath>
#include <chrono>

#include <raylib.h>

#include "calc.hpp"
#include "resources.hpp"

#include "ui.hpp"

#include "demo_partitionning.hpp"

// ##################################################### CONSTANTS #####################################################
constexpr int SOLDIER_TILE_ID = 245;
constexpr float ENEMY_SCALE = 0.5f;

// ##################################################### NOT WORKING NearestSearcher IMPL #####################################################

class NotWorkingNearestSearcher : public NearestSearcher
{
public:
    NotWorkingNearestSearcher(std::vector<Enemy>& enemies) : NearestSearcher(enemies) {}

    void Update() override {}
    void QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList) override;
};

void NotWorkingNearestSearcher::QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList)
{
    // Loose time by computing distances
    {
        for (size_t i = 0; i < enemies.size(); ++i)
        {
            calc::length(enemies[i].position);
        }
    }
    
    // Return random enemies
    for (int i = 0; i < count; ++i)
    {
        nearestList.push_back(&enemies[i]);
    }
}

// ##################################################### BRUTE FORCE NearestSearcher IMPL #####################################################

struct BruteForceElement
{
    float dist;
    Enemy* enemy;
};

class BruteForceNearestSearcher : public NearestSearcher
{
public:
    BruteForceNearestSearcher(std::vector<Enemy>& enemies);

    void Update() override {}
    void QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList) override;

private:
    // Keep list in memory to avoid useless allocation/deallocation
    // We known that this list will be the same size as enemies list
    std::vector<BruteForceElement> workingList;
};

template <typename T>
static void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

BruteForceNearestSearcher::BruteForceNearestSearcher(std::vector<Enemy>& enemies) 
    : NearestSearcher(enemies)
{
    // Pre-allocate the array
    workingList.resize(enemies.size());
}

static void sortInplace(BruteForceElement* list, int size)
{
    // TODO: Sort list
}

void BruteForceNearestSearcher::QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList)
{
    // Bubble sort
    std::vector<Enemy*> workList;
    for(size_t i = 0; i < enemies.size(); i++)
        workList.push_back(&enemies[i]);

    for (size_t i = 0; i < workList.size() - 1; ++i)
    {
        for(size_t j = 0; j < workList.size() - i - 1; j++)
        {
            float distA = calc::length(workList[j]->position - position);
            float distB = calc::length(workList[j+1]->position - position);
            if(distA > distB)
                swap(workList[j], workList[j+1]);
        }
    }
    
    for(int i = 0; i < count; i++)
        nearestList.push_back(workList[i]);
}

// ##################################################### SMART NearestSearcher IMPL #####################################################

class SmartNearestSearcher : public NearestSearcher
{
public:
    SmartNearestSearcher(std::vector<Enemy>& enemies);

    void Update() override {};
    void QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList) override;

private:
    int Partition(std::vector<Enemy*>& list, int lo, int hi, float2 position);
    void QuickSort(std::vector<Enemy*>& list, int lo, int hi, float2 position);
};


SmartNearestSearcher::SmartNearestSearcher(std::vector<Enemy>& enemies)
    : NearestSearcher(enemies)
{
}

int SmartNearestSearcher::Partition(std::vector<Enemy*>& list, int lo, int hi, float2 position)
{
    int i = lo - 1;

    for(int j = lo; j < hi; j++)
    {
        if(calc::lengthSq(list[j]->position - position) <= calc::lengthSq(list[hi]->position - position))
        {
            i++;
            swap(list[i], list[j]);
        }
    }
    i++;
    swap(list[i], list[hi]);
    return i;
}

void SmartNearestSearcher::QuickSort(std::vector<Enemy*>& list, int lo, int hi, float2 position)
{
    if(lo >= 0 && hi >= 0 && lo < hi)
    {
        int p = Partition(list, lo, hi, position);

        QuickSort(list, lo, p - 1, position);
        QuickSort(list, p + 1, hi, position);
    }
}

void SmartNearestSearcher::QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList)
{
    // quick sort
    std::vector<Enemy*> workList;
    for(size_t i = 0; i < enemies.size(); i++)
        workList.push_back(&enemies[i]);

    QuickSort(workList, 0, workList.size() - 1, position);

    for(int i = 0; i < count; i++)
        nearestList.push_back(workList[i]);
}

// ##################################################### Merge Sort NearestSearcher IMPL #####################################################

class MergeSortNearestSearcher : public NearestSearcher
{
public:
    MergeSortNearestSearcher(std::vector<Enemy>& enemies);

    void Update() override {};
    void QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList) override;

private:
    std::vector<BruteForceElement> A;
    std::vector<BruteForceElement> B;
};


MergeSortNearestSearcher::MergeSortNearestSearcher(std::vector<Enemy>& enemies)
    : NearestSearcher(enemies)
{
    A.resize(enemies.size());
    B.resize(enemies.size());
}

void TopDownMerge(BruteForceElement* A, BruteForceElement* B, int iBegin, int iMiddle, int iEnd, float2 position)
{
    int i = iBegin;
    int j = iMiddle;

    for(int k = iBegin; k < iEnd; k++)
    {
        if(i < iMiddle && (j >= iEnd || A[i].dist <= A[j].dist))
        {
            B[k] = A[i];
            i++;
        }
        else
        {
            B[k] = A[j];
            j++;
        }
    }
}

void TopDownSplitMerge(BruteForceElement* B, BruteForceElement* A, int iBegin, int iEnd, float2 position)
{
    if(iEnd - iBegin <= 1)
        return;

    TopDownSplitMerge(A, B, iBegin, (iEnd + iBegin) / 2, position);
    TopDownSplitMerge(A, B, (iEnd + iBegin) / 2, iEnd, position);
    TopDownMerge(B, A, iBegin, (iEnd + iBegin) / 2, iEnd, position);
}

void MergeSortNearestSearcher::QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearestList)
{
    // Merge sort
    for(size_t i = 0; i < enemies.size(); i++)
    {
        A[i].enemy = &enemies[i];
        A[i].dist = calc::lengthSq(enemies[i].position - position);
        B[i].enemy = &enemies[i];
        B[i].dist = calc::lengthSq(enemies[i].position - position);
    }

    TopDownSplitMerge(B.data(), A.data(), 0, A.size(), position);

    for(int i = 0; i < count; i++)
        nearestList.push_back(A[i].enemy);
}

// ##################################################### VARIOUS FUNCS #####################################################

static void DrawTile(int tileId, float2 position, float rotation, float scale)
{
    int tileX = tileId % Resources::TILES_PER_LINE;
    int tileY = tileId / Resources::TILES_PER_LINE;
    float tileSize = Resources::TILE_SIZE;

    DrawTexturePro(gRes->tiles, 
        { tileSize * tileX, tileSize * tileY, tileSize, tileSize },
        { position.x, position.y, tileSize * scale, tileSize * scale },
        { tileSize / 2.f * scale, tileSize / 2.f * scale},
        rotation * RAD2DEG,
        WHITE
    );
}

static void DrawSoldier(float2 position, float rotation, float scale)
{
    DrawTile(SOLDIER_TILE_ID, position, rotation, scale);
}

static const char* GetTimeStr(int64_t nanoSeconds)
{
    static char buffer[256];

    const char* magnitudeStr[] = { "ns", "us", "ms" };
    int currentMag = 0;

    float value = nanoSeconds;
    while (value > 1.f && currentMag < 2)
    {
        value /= 1000.f;
        currentMag++;
    }

    sprintf(buffer, "%.3f %s", value, magnitudeStr[currentMag]);

    return buffer;
}

// ##################################################### DemoPartitionning #####################################################

static void PushBackRandomEnemies(std::vector<Enemy>& enemies, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Enemy enemy = {};
        enemy.dirAngle = (rand() / (float)RAND_MAX) * calc::TAU;
        enemy.position.x = (rand() / (float)RAND_MAX) * GetScreenWidth();
        enemy.position.y = (rand() / (float)RAND_MAX) * GetScreenHeight();
        enemies.push_back(enemy);
    }
}

static NearestSearcher* nearestSearcherFactory(int strategyIndex, std::vector<Enemy>& enemies)
{
    switch (strategyIndex)
    {
        default: case 0: return new NotWorkingNearestSearcher(enemies);
        case 1:          return new BruteForceNearestSearcher(enemies);
        case 2:          return new SmartNearestSearcher(enemies);
        case 3:          return new MergeSortNearestSearcher(enemies);
    }
}

DemoPartitionning::DemoPartitionning()
{
    PushBackRandomEnemies(enemies, 10000);

    strategyIndex = 3;
    nearestSearcher = nearestSearcherFactory(strategyIndex, enemies);
}

DemoPartitionning::~DemoPartitionning()
{
    delete nearestSearcher;
}

void DemoPartitionning::UpdateAndDraw()
{
    for (Enemy& enemy : enemies)
    {
        enemy.UpdateAndDraw();
    }

    int curUpdateTime;
    int curQueryTime;

    // UPDATE PARTIONNER
    auto updateStart = std::chrono::high_resolution_clock::now();
    nearestSearcher->Update();
    curUpdateTime = (int)(std::chrono::high_resolution_clock::now() - updateStart).count();

    // Find and display X nearest enemies around mouse
    {
        int searchEnemyCount = 20;

        // QUERY NEAREST ENEMIES
        std::vector<Enemy*> nearestEnemies;
        auto queryStart = std::chrono::high_resolution_clock::now();
        {
            float2 mousePosition = { (float)GetMouseX(), (float)GetMouseY() };
            nearestSearcher->QueryNearestEnemies(mousePosition, searchEnemyCount, nearestEnemies);

            for (int i = 1; i < queryPoints; ++i)
            {
                float angle = (i-1) / (float)(queryPoints-1) * calc::TAU;
                float radius = 300.f;
                float2 queryPoint = { mousePosition.x + radius * cosf(angle), mousePosition.y + radius * sinf(angle) };

                nearestSearcher->QueryNearestEnemies(queryPoint, searchEnemyCount, nearestEnemies);
            }
        }
        curQueryTime = (int)(std::chrono::high_resolution_clock::now() - queryStart).count();

        // Draw nearest enemie
        for (Enemy* nearest : nearestEnemies)
        {
            DrawCircleLines(nearest->position.x, nearest->position.y, calc::lerp(4.f, 14.f, (1.f + 0.5f * sinf(2.f * GetTime() * calc::TAU))), RED);
        }
    }
    
    // Draw mouse
    DrawCircle(GetMouseX(), GetMouseY(), 5, BLUE);
    
    // Update timer values
    {
        static float updateTimer = 0.2f; // Update every X sec
        updateTimer -= GetFrameTime();
        if (updateTimer < 0.f)
        {
            updateTimer = 1.f;
            updateTime = curUpdateTime;
            queryTime = curQueryTime;
        }
    }
    
    this->ShowUI();
}

void DemoPartitionning::ShowUI()
{
    int penY = 100;
    int lineHeight = 30;
    int windowWidth = 350;
    DrawRectangle(5, penY - 5, windowWidth + 10, 220, LIGHTGRAY);
    // Display timer texts
    {
        DrawText(TextFormat("Update time: %s", GetTimeStr(updateTime)), 10, penY, GetFontDefault().baseSize * 2, BLACK);
        penY += lineHeight;
        DrawText(TextFormat("Query time: %s", GetTimeStr(queryTime)), 10, penY, GetFontDefault().baseSize * 2, BLACK);
        penY += lineHeight;
        DrawText(TextFormat("Per query: %s (avg)", GetTimeStr(queryTime / queryPoints)), 10, penY, GetFontDefault().baseSize * 2, BLACK);
        penY += lineHeight;
    }

    // Strategy selector
    {
        const char* strategyStrs[] = {
            "NotWorking",
            "BubbleSort",
            "QuickSort",
            "MergeSort",
        };

        // Change search strategy
        if (Button(10, penY, windowWidth, TextFormat("Change strategy: %s", strategyStrs[strategyIndex])))
        {
            strategyIndex = (strategyIndex + 1) % 4;
            delete nearestSearcher;
            nearestSearcher = nearestSearcherFactory(strategyIndex, enemies);
        }
        penY += lineHeight + 20;
    }

    // Query points sliders
    {
        float queryPointsFloat = queryPoints;
        if (SliderFloat("Query points", 10, penY, windowWidth, 30, 1.f, 12.f, &queryPointsFloat))
            queryPoints = (int)roundf(queryPointsFloat);
        penY += lineHeight + 10;
    }

    // Change enemy count
    {
        float enemyCountFloat = enemies.size();
        if (SliderFloat("Enemy count", 10, penY, windowWidth, 30, 1.f, 30000.f, &enemyCountFloat))
        {
            int newEnemyCount = enemyCountFloat;
            delete nearestSearcher;
            if (newEnemyCount > (int)enemies.size())
                PushBackRandomEnemies(enemies, newEnemyCount - enemies.size());
            else
                enemies.resize(newEnemyCount);
            nearestSearcher = nearestSearcherFactory(strategyIndex, enemies);
        }
        penY += lineHeight + 10;
    }
}

// ##################################################### Enemy behavior #####################################################

void Enemy::UpdateAndDraw()
{
    float speed = 15.f;
    float dt = GetFrameTime();
    float2 dir = { cosf(dirAngle), sinf(dirAngle) };
    position += dir * speed * dt;

    // Bounce on screen borders
    {
        float2 left  = {-1.f, 0.f };
        float2 right = { 1.f, 0.f };
        float2 up    = { 0.f,-1.f };
        float2 down  = { 0.f, 1.f };

        float width  = GetScreenWidth();
        float height = GetScreenHeight();
        if (position.x < 0 && calc::dot(left, dir) > 0.f)
            dir.x *= -1;
        else if (position.x > width && calc::dot(right, dir) > 0.f)
            dir.x *= -1;
        else if (position.y < 0 && calc::dot(up, dir) > 0.f)
            dir.y *= -1;
        else if (position.y > height && calc::dot(down, dir) > 0.f)
            dir.y *= -1;

        dirAngle = atan2f(dir.y, dir.x);
    }

    DrawSoldier(position, dirAngle, ENEMY_SCALE);
}