#include <stdint.h>
#include <patch.h>
#include <stdio.h>

#include <string.h>

// 0049bde2 = maybe timer?

typedef struct {
	uint32_t type;
	uint32_t score;
	char *unit;
	uint32_t cash;
	char *name;
	uint32_t maybe_hidden;
} SGoal;

typedef struct {
    char levelName[64];
    char *pCityName;
    char *pVab;
    char *isCompetition;
    char *pTRGName;
    uint32_t GoldScore;
    uint32_t SilverScore;
    uint32_t BronzeScore;
    uint32_t GoldCash;
    uint32_t SilverCash;
    uint32_t BronzeCash;
    char *pImageName;
    uint32_t STR;
    uint32_t map_loc[2];
    char *pGoalsImage;
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
    char **pOOBMessages;
} SLevel;

char *waretrig = "SkWare_T";
char *schltrig = "SkSchl_T";
char *schl2trig = "SkSchl2_T";	// no associated psx - crashes
char *malltrig = "SkMall_T";
char *vanstrig = "SkVans_T";
char *downtrig = "SkDown_T";
char *jamtrig = "SkJam_T";
char *burntrig = "SkBurn_T";
char *sftrig = "SkSF_T";
char *rostrig = "SkRos_T";
char *fwaytrig = "SkFway_T";	// crashes - old trig format maybe?
char *parktrig = "SkPark_T";	// loads but crashes - bad collision maybe?
char *B1trig = "SkB1_T";
char *B2trig = "SkB2_T";
char *hvnoldtrig = "SkHVN_T-old";

char *testVAB = "MALL";

// consts - levels

const SGoal dummygoal = {
    .type = 0,
    .name = "",
    .unit = "",
    .cash = 0,
    .score = 0,
    .maybe_hidden = 1,
};

const SLevel warehouse = {
    .levelName = "Warehouse",
    .pCityName = "Woodland Hills",
    .pVab = "ware",
    .isCompetition = 0,
    .pTRGName = "SkWare_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend14.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2levl14.bmp",
    .CashToUnlock = 0,
    .MedalsToUnlock = 0,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 5,000 Points",
            .unit = NULL,
            .score = 5000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 15,000 Points",
            .unit = NULL,
            .score = 15000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Smash the Boxes",
            .unit = "BOXES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel school = {
    .levelName = "School",
    .pCityName = "Miami",
    .pVab = "school2",
    .isCompetition = 0,
    .pTRGName = "SkSchl_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend14.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 2,
    .MedalsToUnlock = 0,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 7,500 Points",
            .unit = NULL,
            .score = 7500,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 25,000 Points",
            .unit = NULL,
            .score = 25000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Grind 5 Tables",
            .unit = "LUNCH TABLES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel mall = {
    .levelName = "Mall",
    .pCityName = "New York",
    .pVab = "school2",
    .isCompetition = 0,
    .pTRGName = "SkMall_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend14.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 5,
    .MedalsToUnlock = 0,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 10,000 Points",
            .unit = NULL,
            .score = 10000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 30,000 Points",
            .unit = NULL,
            .score = 30000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Destroy 5 Directories",
            .unit = "MALL DIRECTORIES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel vans = {
    .levelName = "Skate Park",
    .pCityName = "Chicago",
    .pVab = "comp",
    .isCompetition = 1,
    .pTRGName = "SkVans_T",
    .GoldScore = 30000,
    .SilverScore = 12000,
    .BronzeScore = 5000,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend13.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2levl13.bmp",
    .CashToUnlock = 8,
    .MedalsToUnlock = 0,
    .Crowd = 1,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = -3500,
    .IntroStartPosY = -1500,
    .IntroStartPosZ = 900,
    .IntroStartAngX = 0,
    .IntroStartAngY = 2048,
    .IntroStartAngZ = 300,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 10,000 Points",
            .unit = NULL,
            .score = 10000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 30,000 Points",
            .unit = NULL,
            .score = 30000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Destroy 5 Directories",
            .unit = "DIRECTORIES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel downtown = {
    .levelName = "Downtown",
    .pCityName = "Minneapolis",
    .pVab = "ny",
    .isCompetition = 0,
    .pTRGName = "SkDown_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend14.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 0,
    .MedalsToUnlock = 1,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 15,000 Points",
            .unit = NULL,
            .score = 15000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 40,000 Points",
            .unit = NULL,
            .score = 40000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Break 5 No Skating Signs",
            .unit = "SIGNS",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel jam = {
    .levelName = "Downhill Jam",
    .pCityName = "Phoenix",
    .pVab = "JAM",
    .isCompetition = 0,
    .pTRGName = "SkJam_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend12.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2levl12.bmp",
    .CashToUnlock = 13,
    .MedalsToUnlock = 1,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 20,000 Points",
            .unit = NULL,
            .score = 20000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 40,000 Points",
            .unit = NULL,
            .score = 40000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Open 5 Valves",
            .unit = "VALVES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel burnside = {
    .levelName = "Burnside",
    .pCityName = "Portland",
    .pVab = "ware",
    .isCompetition = 1,
    .pTRGName = "SkBurn_T",
    .GoldScore = 40000,
    .SilverScore = 22000,
    .BronzeScore = 15000,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend13.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 18,
    .MedalsToUnlock = 1,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = -3500,
    .IntroStartPosY = -1200,
    .IntroStartPosZ = 1500,
    .IntroStartAngX = 0,
    .IntroStartAngY = -2024,
    .IntroStartAngZ = 30,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 10,000 Points",
            .unit = NULL,
            .score = 10000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 30,000 Points",
            .unit = NULL,
            .score = 30000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Destroy 5 Directories",
            .unit = "DIRECTORIES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel sf = {
    .levelName = "Streets",
    .pCityName = "San Francisco",
    .pVab = "school2",
    .isCompetition = 0,
    .pTRGName = "SkSF_T",
    .GoldScore = 0,
    .SilverScore = 0,
    .BronzeScore = 0,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend12.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 0,
    .MedalsToUnlock = 2,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0xFFFFAA10,
        0xFFFFB1E0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = 0,
    .IntroStartPosY = 0,
    .IntroStartPosZ = 0,
    .IntroStartAngX = 0,
    .IntroStartAngY = 0,
    .IntroStartAngZ = 0,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 25,000 Points",
            .unit = NULL,
            .score = 25000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 50,000 Points",
            .unit = NULL,
            .score = 50000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Wreck 5 Cop Cars",
            .unit = "COP CARS",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

const SLevel roswell = {
    .levelName = "Roswell",
    .pCityName = "New Mexico",
    .pVab = "ware",
    .isCompetition = 1,
    .pTRGName = "SkRos_T",
    .GoldScore = 40000,
    .SilverScore = 30000,
    .BronzeScore = 25000,
    .GoldCash = 0,
    .SilverCash = 0,
    .BronzeCash = 0,
    .pImageName = "s2rend13.bmp",
    .STR = 0,
    .map_loc = {
           0x1b,
           0xffffffff
    },
    .pGoalsImage = "s2rend13.bmp",
    .CashToUnlock = 26,
    .MedalsToUnlock = 2,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        0x0,
        0x0
    },
    .TrickTipMask = {
        0x0,
        0x0
    },
    .FMV = 0,
    .IntroStartPosX = -3500,
    .IntroStartPosY = -1500,
    .IntroStartPosZ = 900,
    .IntroStartAngX = 0,
    .IntroStartAngY = 2048,
    .IntroStartAngZ = 300,
    .ActiveGapLower = 0xa8c,
    .ActiveGapUpper = 0xaef,
    .goals = {
        {
            .type = 0,
            .name = "High Score - 10,000 Points",
            .unit = NULL,
            .score = 10000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "Pro Score - 30,000 Points",
            .unit = NULL,
            .score = 30000,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 1,
            .name = "Collect S-K-A-T-E",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 4,
            .name = "Destroy 5 Directories",
            .unit = "DIRECTORIES",
            .score = 5,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 6,
            .name = "Find the Hidden Tape",
            .unit = NULL,
            .score = 0,
            .cash = 1,
            .maybe_hidden = 0
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 0,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

void fixPushbacks() {
    uint32_t *modelPushbacks = 0x0057d4fc;

    memset(modelPushbacks, 0, sizeof(uint32_t) * 15000);
}

uint32_t statLUT[10][7] = {
    {1, 2, 4, 5, 7, 8, 10},
    {2, 3, 5, 6, 7, 9, 10},
    {3, 4, 5, 6, 8, 9, 10},
    {4, 5, 6, 7, 8, 9, 10},
    {5, 6, 7, 7, 8, 9, 10},
    {6, 7, 8, 8, 9, 9, 10},
    {7, 8, 8, 9, 9, 10, 10},
    {8, 9, 9, 9, 10, 10, 10},
    {9, 10, 10, 10, 10, 10, 10},
    {10, 10, 10, 10, 10, 10, 10},
};

void updateSkaterStats() {
    uint32_t *profile = 0x005674b8;
    //printf("SKATER aaa: 0x%08x\n", *profile);
    uint32_t* skater_id = (*profile) + 0x2cc0;

    //printf("SKATER ID: %d\n", *skater_id);

    uint32_t *skatermanager = 0x005656cc;
    uint32_t skaterprof = 0x005656cc + ((*skater_id) * 0x104);   // + 4 is money, + 56 is stats (stored as bytes)

    uint32_t *goalcount = skaterprof + 4;
    uint8_t *cur_stats = skaterprof + 56;

    uint8_t *orig_stats = 0x005372fc + ((*skater_id) * 368);

    //printf("UPDATING STATS - ORIGINAL STATS: %d, CURRENT STATS: %d, goals = %d\n", orig_stats[0], cur_stats[0], *goalcount);

    int level = 0;
    if (*goalcount >= 24) {
        level = 6;
    } else if (*goalcount >= 21) {
        level = 5;
    } else if (*goalcount >= 15) {
        level = 4;
    } else if (*goalcount >= 10) {
        level = 3;
    } else if (*goalcount >= 7) {
        level = 2;
    } else if (*goalcount >= 3) {
        level = 1;
    }

    //printf("LEVEL %d (%d tapes)\n", level, *goalcount);
    for (int i = 0; i < 10; i++) {
        //printf("STAT %d: %d -> %d\n", i, orig_stats[i], statLUT[orig_stats[i] - 1][level]);
        cur_stats[i] = statLUT[orig_stats[i]][level];
    }

    //00537328 - 005371b8
}

void completeDummyGoals() {
    uint32_t *level_id = 0x0055e8f0;

    uint32_t *profile = 0x005674b8;
    //printf("SKATER aaa: 0x%08x\n", *profile);
    uint32_t* skater_id = (*profile) + 0x2cc0;

    //printf("SKATER ID: %d\n", *skater_id);

    uint32_t *skatermanager = 0x005656cc;
    uint32_t skaterprof = 0x005656cc + ((*skater_id) * 0x104);   // + 4 is money, + 56 is stats (stored as bytes)
}

void __cdecl updateSkaterThenPlayAway() {
    void (*__cdecl PlayAway)() = 0x0046a330;

    updateSkaterStats();

    PlayAway();

    updateSkaterStats();
}

char *c_van = "c_van";

// TODOS:
// stats upgrades
// sound banks
// disable powerups
// mark dummy goals as complete when loading in to prevent loud goal completion sound at start
void patchTHPS1Career() {
    // use thps1 gaps
    patchNop(0x0045452a, 2);    // use thps1 gaps
    patchNop(0x0049c0fc, 2);    // set big drop to appropriate thps1 height

    // no idea what these two are, but it's some thps1 check - i think it fixes rail nodes?
    patchNop(0x004a7881, 2);    
    patchNop(0x004a7901, 2);    

    // disable texture replacement
    patchByte(0x00453a41, 0xe9);
    patchDWord(0x00453a41 + 1, 0xe1);

    patchNop(0x004a95b6, 2);    // disable ambient stream

    patchByte(0x0045d968 + 2, 3);   // fix first comp check

    patchCpy(0x00538ff8, &warehouse, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 1), &school, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 2), &mall, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 3), &vans, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 4), &downtown, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 5), &jam, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 6), &burnside, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 7), &sf, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 8), &roswell, sizeof(SLevel));

    patchDWord(0x004126df + 1, c_van);  // fix loading streets

    patchJmp(0x004cffb0, fixPushbacks); // remove z biases meant for the thps2 levels
    patchNop(0x004cf4c2, 5); // fix burnside transparency

    patchCall(0x0046ad11, updateSkaterThenPlayAway);    // hook to update stuff on level start

}