#ifndef _THPS1_LEVEL_H_
#define _THPS1_LEVEL_H_

#include <stdint.h>

typedef struct {
    uint32_t type;
    uint32_t score;
    char* unit;
    uint32_t cash;
    char* name;
    uint32_t maybe_hidden;
} SGoal;

typedef struct {
    char levelName[64];
    char* pCityName;
    char* pVab;
    char* isCompetition;
    char* pTRGName;
    uint32_t GoldScore;
    uint32_t SilverScore;
    uint32_t BronzeScore;
    uint32_t GoldCash;
    uint32_t SilverCash;
    uint32_t BronzeCash;
    char* pImageName;
    uint32_t STR;
    uint32_t map_loc[2];
    char* pGoalsImage;
    uint32_t CashToUnlock;
    uint32_t MedalsToUnlock;
    uint32_t Crowd;
    uint32_t ExtraPolyBuffer[2];
    uint32_t TrickTipMask[2];
    uint32_t FMV;
    int32_t IntroStartPosX;
    int32_t IntroStartPosY;
    int32_t IntroStartPosZ;
    int16_t IntroStartAngX;
    int16_t IntroStartAngY;
    int16_t IntroStartAngZ;
    uint8_t pad[2];
    uint32_t ActiveGapLower;
    uint32_t ActiveGapUpper;
    SGoal goals[10];
    char** pOOBMessages;
} SLevel;

void writeTHPS1LevelData();

#endif