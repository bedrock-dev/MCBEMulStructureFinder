//
// Created by xhy on 2020/11/27.
//

#include "../hook/SymHook.h"
#include "../lib/mod.h"
#include "Sample.h"

#include <vector>

using namespace SymHook;
FILE *fp = nullptr;
int minX = 0;
int maxX = 0;
int minZ = 0;
int maxZ = 0;
int maxDistance = 0;

void initConfig() {
    printf("author: hhhxiao\nbilibili:_hhhxiao\n gihtub:https://github.com/hhhxiao/mcbe_mul_feature_finder\n");
    fp = fopen("mul_finder.txt", "rt+");
    if (!fp) {
        fprintf(stderr, "can't open config file mul_finder.txt\n");
        return;
    }
    size_t hasRead = fscanf(fp, "%d %d %d %d %d\n", &minX, &maxX, &minZ, &maxZ, &maxDistance);
    if (hasRead != 5) {
        fprintf(stderr, "can't not read config file,please check the config\n");
        fclose(fp);
        return;
    }
    printf("set config:\n minX=%d maxX=%d minZ=%d maxZ=%d maxDistance=%d\n", minX, maxX, minZ, maxZ, maxDistance);
    if (minX >= maxX || minZ >= maxZ || maxDistance <= 0) {
        fprintf(stderr, "invalid value(minX must < maxX ,minZ must < maxZ,maxDistance > 0)\n");
        fclose(fp);
        return;
    }
    printf("success set config file!!!!!!!!\n-------------------------------------------\n");
    fclose(fp);
}

std::string Biome::getBiomeName() {
    char *str = reinterpret_cast<char *>(this) + 8;
    return std::string(str);
}

int Biome::getBiomeType() {
    return SYM_CALL(
            int(*)(Biome * ),
            MSSYM_B1QE12getBiomeTypeB1AA5BiomeB2AAA4QEBAB1QE20AW4VanillaBiomeTypesB2AAA2XZ,
            this
    );
}

int findMUl(const std::vector<std::pair<int, int>> &list, long long int maxDis2) {
    size_t num = 0;
    size_t length = list.size();
    for (int i = 0; i < length - 1; ++i) {
        for (int j = i + 1; j < length; j++) {
            long long int dx = list[i].first - list[j].first;
            long long int dy = list[i].second - list[j].second;
            long long int d2 = dx * dx + dy * dy;
            if (d2 <= maxDis2) {
                printf("p1 :[%d %d]   p2:[%d %d]  ==> d: %.2f (/tp %d ~ %d)\n", list[i].first, list[i].second,
                       list[j].first,
                       list[j].second, sqrt(d2), list[i].first, list[i].second);
                ++num;
            }
        }
    }
    return num;
}

bool hasFindOceanMonument = false;

THook(
        bool,
        MSSYM_B1QE14isFeatureChunkB1AE20OceanMonumentFeatureB2AAA4UEAAB1UE16NAEBVBiomeSourceB2AAE10AEAVRandomB2AAE12AEBVChunkPosB2AAA1IB1AA1Z,
        int32_t *structure,
        char *biomeSource,
        void *rand,
        ChunkPos *pos,
        unsigned int v3
) {

    bool result = original(structure, biomeSource, rand, pos, v3);
    if (hasFindOceanMonument)return result;
    printf("begin find  OceanMonument\n");
    int blockSize = (maxX - minX) / 10 + 1;
    ChunkPos p = {0, 0};
    std::vector<std::pair<int, int>> list;
    for (int i = minX; i < maxX; ++i) {
        for (int j = minZ; j < maxZ; ++j) {
            p.x = i;
            p.z = j;
            if (original(structure, biomeSource, rand, &p, v3)) {
                list.emplace_back(i * 16, j * 16);
            }
        }
        if (i % blockSize == 0) {
            printf("%d0%% finished\n", 5 + i / blockSize);
        }

    }

    size_t length = list.size();
    printf("total %zu feature find\nbegin cal OceanMonument distance...\n", length);
    size_t num = findMUl(list, maxDistance * maxDistance);
    if (num == 0) {
        printf("no mul found\n");
    }
    hasFindOceanMonument = true;
    printf("finish\n---------------------------------------------------------\n");
    return result;
}

bool hasFindRandomScattered = false;

THook(
        bool,
        MSSYM_B1QE14isFeatureChunkB1AE27RandomScatteredLargeFeatureB2AAA4MEAAB1UE16NAEBVBiomeSourceB2AAE10AEAVRandomB2AAE12AEBVChunkPosB2AAA1IB1AA1Z,
        int32_t *structure,
        char *biomeSource,
        void *rand,
        ChunkPos *pos,
        unsigned int v3
) {

    bool result = original(structure, biomeSource, rand, pos, v3);
    if (hasFindRandomScattered) return result;
    printf("begin finder randomScattered\n");
    int blockSize = (maxX - minX) / 10 + 1;
    ChunkPos p = {0, 0};
    std::vector<std::pair<int, int>> list;
    for (int i = minX; i < maxX; ++i) {
        for (int j = minZ; j < maxZ; ++j) {
            p.x = i;
            p.z = j;
            if (original(structure, biomeSource, rand, &p, v3)) {
                Biome *biome = SYM_CALL(
                        Biome*(*)(void * , int, int),
                        MSSYM_B1QA8getBiomeB1AE16LayerBiomeSourceB2AAE13UEBAPEBVBiomeB2AAA2HHB1AA1Z,
                        biomeSource,
                        p.x * 16, p.z * 16
                );
                if (biome->getBiomeType() == 15) {
                    list.emplace_back(i * 16, j * 16);
                    //   printf("(%d %d) == %d %s\n", 16 * p.x, 16 * p.z, biome->getBiomeType(),biome->getBiomeName().c_str());
                }
            }
        }
        if (i % blockSize == 0) {
            printf("%d0%% finished\n", 5 + i / blockSize);
        }
    }

    size_t length = list.size();
    printf("total %zu feature find\nbegin cal ScatteredFeature distance...\n", length);
    size_t num = findMUl(list, maxDistance * maxDistance);
    if (num == 0) {
        printf("no mul found\n");
    }
    hasFindRandomScattered = true;
    printf("finish\n---------------------------------------------------------\n");
    return result;
}

