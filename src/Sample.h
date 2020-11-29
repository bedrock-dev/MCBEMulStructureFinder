//
// Created by xhy on 2020/11/27.
//

#ifndef BDS_MOD_SAMPLE_SAMPLE_H
#define BDS_MOD_SAMPLE_SAMPLE_H

#include <string>

struct ChunkPos {
    int x;
    int z;
};

class Biome {
public:
    int getBiomeType();

    std::string getBiomeName();
};

extern FILE *fp;
extern int minX;
extern int maxX;
extern int minZ;
extern int maxZ;
extern int maxDistance;

void initConfig();

#endif //BDS_MOD_SAMPLE_SAMPLE_H
