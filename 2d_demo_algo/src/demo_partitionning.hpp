
#include <cstdint>
#include <vector>

#include "types.hpp"
#include "demo.hpp"

class Enemy
{
public:
    float2 position;
    float dirAngle;

    void UpdateAndDraw();
};

class NearestSearcher
{
public:
    NearestSearcher(std::vector<Enemy>& enemies) : enemies(enemies) {}
    virtual ~NearestSearcher() {}

    virtual void Update() = 0;
    virtual void QueryNearestEnemies(float2 position, int count, std::vector<Enemy*>& nearest) = 0;

protected:
    std::vector<Enemy>& enemies;
};

class DemoPartitionning : public Demo
{
public:
    DemoPartitionning();
    ~DemoPartitionning();

    const char* GetName() override { return "Partitionning"; }
    void UpdateAndDraw() override;

private:
    std::vector<Enemy> enemies;
    NearestSearcher* nearestSearcher; // Searcher impl
    
    int strategyIndex = 0;
    int queryPoints = 1;

    int64_t updateTime; // In nanoseconds
    int64_t queryTime; // In nanoseconds

    void ShowUI();
};