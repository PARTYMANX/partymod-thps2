#include <stdint.h>
#include <patch.h>
#include <stdio.h>

#include <string.h>

#include <log.h>

#include <thps1/vab.h>

// 0049bde2 = maybe timer?

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

// AUDIO STUFF

void dumpAudioBanks() {
    struct vab_bank *VABLookup = 0x00549730;

    log_printf(LL_DEBUG, "BEGIN AUDIO DUMP\n\n\n\n\n");

    log_printf(LL_DEBUG, "struct vab_bank VABLookup[19] = {\n");

    for (int i = 0; i < 18; i++) {
        log_printf(LL_DEBUG, "\t{\n\t\t// bank %d\n\t\t.sounds = {\n", i);

        for (int j = 0; j < 66; j++) {
            log_printf(LL_DEBUG, "\t\t\t{ .index = %d, .vol = %ff, .unk2 = 0x%08x, .name = \"%s\", .loop = %d, .unk4 = 0x%08x },\n",
                VABLookup[i].sounds[j].index, 
                VABLookup[i].sounds[j].vol,
                VABLookup[i].sounds[j].tuning,
                VABLookup[i].sounds[j].name,
                VABLookup[i].sounds[j].loop,
                VABLookup[i].sounds[j].unk4);
        }

        log_printf(LL_DEBUG, "\t\t},\n\t},\n");
    }

    log_printf(LL_DEBUG, "};\n");

    log_printf(LL_DEBUG, "\n\n\n\nEND AUDIO DUMP\n");
}

uint32_t SFX_PlayPos_Wrapper_Bounds(uint32_t sound_id, struct CVector* pos, uint32_t pitch_offset) {
    uint32_t(__cdecl * SFX_PlayPos)(uint32_t, struct CVector*, uint32_t) = 0x004ab0b0;

    // the streets sliding door gives a 16-bit -1 for the sound, so treat that like you'd treat 32-bit -1
    if (sound_id == 65535) {
        return -1;
    }

    SFX_PlayPos(sound_id, pos, pitch_offset);
}

char *c_van = "c_van";

// TODOS:
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

    // disable ambient stream
    patchNop(0x004a95b6, 2);
    patchByte(0x004a962c, 0xeb);
    patchNop(0x004a97cc, 2);

    // disable gap checks
    patchNop(0x00414c5a, 2);
    patchJmp(0x00414c5f, 0x00414d85);

    patchByte(0x0045d968 + 2, 3);   // fix first comp check

    // change save names for thps1 mode
    patchByte(0x0052fa30 + 4, '1'); // THPS2 CAREER -> THPS1 CAREER
    patchByte(0x0052fa20 + 4, '1'); // THPS2 REPLAY -> THPS1 REPLAY

    patchDWord(0x004126df + 1, c_van);  // fix loading streets

    patchJmp(0x004cffb0, fixPushbacks); // remove z biases meant for the thps2 levels
    patchNop(0x004cf4c2, 5); // fix burnside transparency

    patchCall(0x0046ad11, updateSkaterThenPlayAway);    // hook to update stuff on level start

    patchCall(0x0049e7e1, SFX_PlayPos_Wrapper_Bounds);  // fix for sliding door in streets

    writeTHPS1LevelData();
    writeTHPS1VabData();
}

void patchTHPS1LevelFixes() {
    writeTHPS2VabData();
}
