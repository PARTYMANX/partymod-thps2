#include <patch.h>

#include <thps1/level.h>

char* waretrig = "SkWare_T";
char* schltrig = "SkSchl_T";
char* schl2trig = "SkSchl2_T";	// no associated psx - crashes
char* malltrig = "SkMall_T";
char* vanstrig = "SkVans_T";
char* downtrig = "SkDown_T";
char* jamtrig = "SkJam_T";
char* burntrig = "SkBurn_T";
char* sftrig = "SkSF_T";
char* rostrig = "SkRos_T";
char* fwaytrig = "SkFway_T";	// crashes - only references burnside?
char* parktrig = "SkPark_T";	// loads but crashes - too many polygons??
char* B1trig = "SkB1_T";
char* B2trig = "SkB2_T";
char* hvnoldtrig = "SkHVN_T-old";

// consts - levels

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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "school",
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
    .pGoalsImage = "s2levl11.bmp",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "mall",
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
    .pGoalsImage = "s2levl11.bmp",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "downtown",
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
    .pGoalsImage = "s2levl11.bmp",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "comp",
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
    .pGoalsImage = "s2levl11.bmp",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "sf",
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
    .pGoalsImage = "s2levl11.bmp",
    .CashToUnlock = 0,
    .MedalsToUnlock = 2,
    .Crowd = 0,
    .ExtraPolyBuffer = {
        // tons of extra poly buffer for the views that fit practically the whole level with max view distance
        0x20000,
        0x20000,
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
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
    .pVab = "roswell",
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
    .pGoalsImage = "s2levl11.bmp",
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
            .type = 7,
            .name = "100% Goals",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        },
        {
            .type = -1,
            .name = "",
            .unit = "",
            .cash = 0,
            .score = 0,
            .maybe_hidden = 0,
        }
    },
    .pOOBMessages = 0x0052bddc,
};

void writeTHPS1LevelData() {
    patchCpy(0x00538ff8, &warehouse, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 1), &school, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 2), &mall, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 3), &vans, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 4), &downtown, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 5), &jam, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 6), &burnside, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 7), &sf, sizeof(SLevel));
    patchCpy(0x00538ff8 + (sizeof(SLevel) * 8), &roswell, sizeof(SLevel));
}