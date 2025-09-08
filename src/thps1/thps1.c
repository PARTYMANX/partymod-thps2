#include <stdint.h>
#include <patch.h>
#include <stdio.h>

#include <string.h>

#include <log.h>

#include <thps1/level.h>
#include <thps1/gap.h>
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
    uint32_t *cur_decks = skaterprof + 52;

    *cur_decks |= 0x02;

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

    // unlock a deck for each level
    for (int i = 0; i < level; i++) {
        *cur_decks |= 0x01 << (2 + i);
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

void career_init_wrapper() {
    void(__cdecl * career_init)() = 0x004138f0;

    career_init();

    uint32_t* skatermanager = 0x005656cc;
    for (int i = 0; i < 0x11; i++) {
        uint32_t skaterprof = 0x005656cc + (i * 0x104);

        uint32_t* cur_decks = skaterprof + 52;
        *cur_decks |= 0x02;
    }
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

uint32_t CountLevelGoalsWrapper(uint32_t a, uint32_t level) {
    int (__cdecl *CountLevelGoals)(uint32_t, uint32_t) = 0x00414130;

    uint32_t result = CountLevelGoals(a, level) * 2;   // double the goals so 5 goals would be 10 goals complete, also results in more accurate percentages

    if (result > 10) {
        result = 10;
    }

    return result;
}

// hides score bonus powerups
uint32_t *__fastcall create_powerup_wrapper(uint32_t *obj, void *pad, uint16_t a, void *b, void *c, uint8_t d, uint16_t e, uint16_t f) {
    uint32_t *(__fastcall *create_powerup)(uint32_t *, void *, uint16_t, void *, void *, uint8_t, uint16_t, uint16_t) = 0x004a5530;
    void (__fastcall *maybe_destroy_model)(uint32_t *) = 0x004a5b30;

    uint32_t *result = create_powerup(obj, pad, a, b, c, d, e, f);

    if (a == 21 || a == 22 || a == 23) {
        maybe_destroy_model(obj);
        *(uint8_t *)((uint32_t)obj + 0x6a) |= 0x40;
    }

    return result;
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

    // hook to unlock initial decks and stuff when starting a new career
    patchCall(0x00416612, career_init_wrapper);
    patchCall(0x00416667, career_init_wrapper);
    patchCall(0x00417eb8, career_init_wrapper);
    patchCall(0x00458794, career_init_wrapper);
    patchCall(0x0046c312, career_init_wrapper);

    patchCall(0x0049e7e1, SFX_PlayPos_Wrapper_Bounds);  // fix for sliding door in streets

    // only handle 6 (goals + 100%) goals
    // load screen checklist
    patchDWord(0x0045e0c0 + 2, 0x9c);
    patchDWord(0x00415fa7 + 2, 0x9c);

    // don't display cash on checklist
    patchNop(0x0045e0a8, 17);
    patchNop(0x00415f8f, 17);

    // post-play goals display
    patchNop(0x00450544, 5);    // don't display cash
    //patchDWord(0x00450565 + 1, 0x9c);    // don't display extra goals

    // award 100% goals after 5 goals complete (actually 10, but we multiplied it by 2)
    patchByte(0x0041463c + 2, 0x0a);

    // only check first five goals of a level for completion (checks the last goals first so not useful)
    //patchByte(0x00414192 + 2, 0x05);

    // double goal completion for purposes of percentage and detecting 100% completion
    patchCall(0x004141b8, CountLevelGoalsWrapper);
    patchCall(0x00459cbd, CountLevelGoalsWrapper);

    // change comp medal percentages
    patchDWord(0x00459cac + 1, 33);
    patchDWord(0x00459c98 + 1, 67);
    patchDWord(0x00459c84 + 1, 100);
    patchByte(0x00459cb4, 0xeb);    // skip last 10% for cash

    // game completion checks
    patchByte(0x0041673f + 2, 36);

    // don't jump for comp medal checks
    patchNop(0x00416753, 2);
    patchNop(0x00416759, 2);
    patchNop(0x0041675f, 2);

    // don't attempt to play level select fmvs
    patchByte(0x0045b642, 0xeb);
    patchByte(0x0045b73d, 0xeb);
    patchByte(0x0045b78d, 0xeb);
    patchByte(0x0045b3b1, 0xeb);
    patchJmp(0x0045b428, 0x0045b59c);

    // fix roswell on level select screen in career
    patchNop(0x00459a94, 2);
    patchNop(0x00459b74, 2);
    patchByte(0x00459ad2 + 2, 0x09);

    // hide score bonus powerups that would be invalid
    //patchNop(0x004a5a02, 2);
    patchCall(0x0046e674, create_powerup_wrapper);
    patchCall(0x0046e6b1, create_powerup_wrapper);
    patchCall(0x0046e6ea, create_powerup_wrapper);
    patchCall(0x004a6790, create_powerup_wrapper);

    writeTHPS1LevelData();
    writeTHPS1GapData();
    writeTHPS1VabData();
}

void patchTHPS1LevelFixes() {
    writeTHPS2VabData();
}
