// houses graphics hacks for specific things like levels and menus.  stuff like manual Z-biasing lives here

#include <stdint.h>

#include <log.h>
#include <patch.h>
#include <util.h>
#include <gfx/gfx_global.h>

#include <gfx/gfx_hacks.h>

uint32_t current_level = 0;

void set_hacks_current_level(uint32_t level) {
	current_level = level;
}

void build_pushbacks() {
	int16_t *modelPushbacks = 0x0057d4fc;

	// build temporary buffer of pushbacks as written by the game
	int16_t *tmpPushbacks = malloc(sizeof(int16_t) * 30000);
	memcpy(tmpPushbacks, modelPushbacks, sizeof(int16_t) * 30000);

	// build the list of levels as they appear

	for (int i = 0; i < 13; i++) {
		uint32_t level_crc = get_level_crc(i);
		switch (level_crc) {
		case LEVEL_CRC_HANGAR:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 0), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_SCHOOL2:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 1), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_MARSEILLE:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 2), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_NYCITY:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 3), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_VENICEBEACH:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 4), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_SKATESTREET:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 5), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_PHILADELPHIA:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 6), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_BULLRING:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 7), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_CHOPPERDROP:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 8), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_SKATEHEAVEN:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 9), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_DOWNHILLJAM:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 10), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_SKATEPARK:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 11), sizeof(int16_t) * 2000);
			break;
		case LEVEL_CRC_WAREHOUSE:
			memcpy(modelPushbacks + (2000 * i), tmpPushbacks + (2000 * 12), sizeof(int16_t) * 2000);
			break;
		default:
			memset(modelPushbacks + (2000 * i), 0, sizeof(int16_t) * 2000);
			break;
		}
	}

	free(tmpPushbacks);

	// add our own pushbacks for levels now that they've been rearranged
	for (int i = 0; i < 13; i++) {
		uint32_t level_crc = get_level_crc(i);
		switch (level_crc) {
		case LEVEL_CRC_SCHOOL2:
		{
			modelPushbacks[(2000 * i) + 294] = 32;
			modelPushbacks[(2000 * i) + 925] = 32;
		}
		break;
		case LEVEL_CRC_NYCITY:
		{
			// park path
			modelPushbacks[(2000 * i) + 21] = 16;

			// tree planter needs raising a bit
			//modelPushbacks[(2000 * i) + 368] = -64;

			// banks bricks
			modelPushbacks[(2000 * i) + 881] = 16;
			modelPushbacks[(2000 * i) + 882] = -96;	// this one is reversed for some reason

			// brooklyn bridge water - push back to reveal reflection
			modelPushbacks[(2000 * i) + 32] = 8192;

			// joey's place interior
			modelPushbacks[(2000 * i) + 381] = 128;
			modelPushbacks[(2000 * i) + 1005] = 128;
		}
		break;
		case LEVEL_CRC_VENICEBEACH:
		{
			// tree planters - disable pushback to keep shadow stable/visible
			modelPushbacks[(2000 * i) + 33] = 0;
			modelPushbacks[(2000 * i) + 34] = 0;
			modelPushbacks[(2000 * i) + 35] = 0;
			modelPushbacks[(2000 * i) + 36] = 0;
			modelPushbacks[(2000 * i) + 37] = 0;
			modelPushbacks[(2000 * i) + 38] = 0;
			modelPushbacks[(2000 * i) + 66] = 0;
			modelPushbacks[(2000 * i) + 72] = 0;
			modelPushbacks[(2000 * i) + 86] = 0;
			modelPushbacks[(2000 * i) + 282] = 0;

			// push underlying surface back
			// seaside concrete
			modelPushbacks[(2000 * i) + 545] = 32;

			// pit concrete
			modelPushbacks[(2000 * i) + 120] = 32;
			modelPushbacks[(2000 * i) + 122] = 32;
			modelPushbacks[(2000 * i) + 125] = 32;
			modelPushbacks[(2000 * i) + 138] = 32;
			modelPushbacks[(2000 * i) + 147] = 32;
			modelPushbacks[(2000 * i) + 181] = 32;
			modelPushbacks[(2000 * i) + 183] = 32;
			modelPushbacks[(2000 * i) + 184] = 32;
			modelPushbacks[(2000 * i) + 186] = 32;
			modelPushbacks[(2000 * i) + 187] = 32;
			modelPushbacks[(2000 * i) + 188] = 32;
			modelPushbacks[(2000 * i) + 189] = 32;
			modelPushbacks[(2000 * i) + 190] = 32;
			modelPushbacks[(2000 * i) + 191] = 32;
			modelPushbacks[(2000 * i) + 193] = 32;
			modelPushbacks[(2000 * i) + 194] = 32;
			modelPushbacks[(2000 * i) + 197] = 32;
			modelPushbacks[(2000 * i) + 196] = 32;
			modelPushbacks[(2000 * i) + 198] = 32;
			modelPushbacks[(2000 * i) + 199] = 32;
			modelPushbacks[(2000 * i) + 201] = 32;
			modelPushbacks[(2000 * i) + 227] = 96;
			modelPushbacks[(2000 * i) + 228] = 96;
			modelPushbacks[(2000 * i) + 231] = 32;
			modelPushbacks[(2000 * i) + 268] = 32;
			modelPushbacks[(2000 * i) + 269] = 32;
			modelPushbacks[(2000 * i) + 272] = 32;
			modelPushbacks[(2000 * i) + 386] = 32;
			modelPushbacks[(2000 * i) + 442] = 32;
			modelPushbacks[(2000 * i) + 444] = 32;
			modelPushbacks[(2000 * i) + 682] = 32;
			modelPushbacks[(2000 * i) + 683] = 32;
			modelPushbacks[(2000 * i) + 686] = 32;
			modelPushbacks[(2000 * i) + 998] = 32;
		}
		break;
		case LEVEL_CRC_PHILADELPHIA:
		{
			// thps sign base and shadow
			modelPushbacks[(2000 * i) + 420] = -32;
			modelPushbacks[(2000 * i) + 838] = -32;
			modelPushbacks[(2000 * i) + 839] = -32;
			modelPushbacks[(2000 * i) + 840] = -32;
			modelPushbacks[(2000 * i) + 841] = -32;
			modelPushbacks[(2000 * i) + 842] = -32;
		}
		break;
		case LEVEL_CRC_SKATEHEAVEN:
		{
			// sidewalk near ramp behind hexbox
			modelPushbacks[(2000 * i) + 360] = 196;

			// playground dirt
			modelPushbacks[(2000 * i) + 361] = 64;
			modelPushbacks[(2000 * i) + 355] = 64;

			// tony ramp ground
			modelPushbacks[(2000 * i) + 222] = 32;
			modelPushbacks[(2000 * i) + 223] = 32;
			modelPushbacks[(2000 * i) + 224] = 32;
			modelPushbacks[(2000 * i) + 197] = 32;	// surface
		}
		break;
		case LEVEL_CRC_WAREHOUSE:
		{
			// remove pushback on starting wall
			modelPushbacks[(2000 * i) + 113] = 0;
			modelPushbacks[(2000 * i) + 162] = 0;
		}
		break;
		case LEVEL_CRC_MALL:
		{
			// space available signs
			modelPushbacks[(2000 * i) + 390] = -32;
			modelPushbacks[(2000 * i) + 391] = -32;
			modelPushbacks[(2000 * i) + 392] = -32;
			modelPushbacks[(2000 * i) + 393] = -32;
			modelPushbacks[(2000 * i) + 394] = -32;

			// push sky back near end
			modelPushbacks[(2000 * i) + 682] = 4096;
			modelPushbacks[(2000 * i) + 683] = 4096;
			modelPushbacks[(2000 * i) + 684] = 4096;
			modelPushbacks[(2000 * i) + 685] = 4096;
		}
		break;
		case LEVEL_CRC_SKATEPARK:
		{
			// push transworld ads forward
			modelPushbacks[(2000 * i) + 227] = -64;
			modelPushbacks[(2000 * i) + 229] = -64;

			// crowd in judge's box
			modelPushbacks[(2000 * i) + 37] = 256;
			modelPushbacks[(2000 * i) + 38] = 128;
			modelPushbacks[(2000 * i) + 39] = 256;
			modelPushbacks[(2000 * i) + 37] = 0;
			modelPushbacks[(2000 * i) + 38] = 0;
			modelPushbacks[(2000 * i) + 39] = 0;

			// glass on judge's booth
			modelPushbacks[(2000 * i) + 154] = 0;

			// wall around judge's booth glass
			modelPushbacks[(2000 * i) + 155] = 0;
			modelPushbacks[(2000 * i) + 178] = 0;
		}
		break;
		case LEVEL_CRC_DOWNTOWN:
		{
			// push video signs forward
			modelPushbacks[(2000 * i) + 256] = -64;
			modelPushbacks[(2000 * i) + 257] = -64;
		}
		break;
		case LEVEL_CRC_STREETS:
		{
			// push 'NOEL' sign forward
			modelPushbacks[(2000 * i) + 348] = -64;
			modelPushbacks[(2000 * i) + 349] = -64;

			// push wall back to reveal graffiti
			modelPushbacks[(2000 * i) + 546] = 128;
			modelPushbacks[(2000 * i) + 555] = 128;
		}
		break;
		default:
			break;
		}
	}
}

uint8_t should_texture_write_depth(struct texture *tex) {
	// hack for masked textures that have depth issues - draw them with depth write
	uint8_t write_depth = 0;

	switch (current_level) {
	case LEVEL_CRC_HANGAR: // Hangar
		if (tex->tex_checksum == 0x9863a474 ||	// wrecked plane wheel
			tex->tex_checksum == 0x2fdba2c8 ||	// wrecked plane tag
			tex->tex_checksum == 0x59d9434c ||	// wrecked plane landing gear
			tex->tex_checksum == 0x4c716d9e) {	// wrecked plane body
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_SCHOOL2:	// School II
		if (tex->tex_checksum == 0x8758d767) {	// bike rack
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_NYCITY: // NYC
		if (tex->tex_checksum == 0x7ae94072 ||	// blue awning
			tex->tex_checksum == 0xa10d8d59 ||	// green awning
			tex->tex_checksum == 0x8a321362 ||	// road closed sign
			tex->tex_checksum == 0x540a6ef2 ||	// bench back rest
			tex->tex_checksum == 0xd5840538) {	// bench legs

			write_depth = 1;
		}
		break;
	case LEVEL_CRC_SKATESTREET:	// Skate Street
		if (tex->tex_checksum == 0xab803fa8 ||	// chair side
			tex->tex_checksum == 0xd67c8173 ||	// chair side
			tex->tex_checksum == 0xfffffd4d ||	// table legs
			tex->tex_checksum == 0x70ec04da ||	// under cone
			tex->tex_checksum == 0xa2c2c95a) {	// ramp support
			write_depth = 1;
		}
	case LEVEL_CRC_PHILADELPHIA:	// Philadelphia
		if (tex->tex_checksum == 0x3290a444 ||	// bus side
			tex->tex_checksum == 0xb500ae75 ||	// bus side
			tex->tex_checksum == 0xc1ff0913) {	// bus side
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_SKATEHEAVEN:	// Skate Heaven
		if (tex->tex_checksum == 0x94cbeff8) {	// bleachers
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_MALL:
		if (tex->tex_checksum == 0x5f19b863 ||	// tree
			tex->tex_checksum == 0x791da2c6 ||	// shutter
			tex->tex_checksum == 0x2ce6579b ||	// books sign
			tex->tex_checksum == 0x5e00ea09 ||	// drugstore
			tex->tex_checksum == 0xddb91f1c ||	// skidmark cards
			tex->tex_checksum == 0x8f6af549 ||	// skateshop deck
			tex->tex_checksum == 0x725a0557 ||	// car wheel
			tex->tex_checksum == 0x7ff460e6 ||	// atrium awning supports
			tex->tex_checksum == 0x712a4eff) {	// boards b us
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_SKATEPARK: 
		if (tex->tex_checksum == 0x4190e88b) {	// crowd
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_DOWNHILLJAM:	// DHJ
		if (tex->tex_checksum == 0x8358c7e1) {	// truss
			write_depth = 1;
		}
		break;
	case LEVEL_CRC_STREETS:
		if (tex->tex_checksum == 0x133533d0 ||	// noel
			tex->tex_checksum == 0x3444bb20 ||	// big ramp truss
			tex->tex_checksum == 0x20855154 ||	// door
			tex->tex_checksum == 0x59080df8) {	// truss
			write_depth = 1;
		}
		break;
	default:
		break;
	}

	return write_depth;
}

uint8_t should_background_write_depth() {
	// hack for ny city exclusively to disable depth writes on its background
	// avoids situations where the sky dome draws over the brooklyn bridge

	return current_level != LEVEL_CRC_NYCITY;
}

void __fastcall fixChecklistFont(void *font, void *pad, int a, int b, int c, int d) {
	void (__fastcall *Font_SetRGB)(void *, void *, uint8_t, uint8_t, uint8_t) = 0x0044ab10;
	void (__fastcall *Font_Draw)(void *, void *, int, int, int, int) = 0x0044a010;

	Font_SetRGB(font, NULL, 0x60, 0x60, 0x60);

	Font_Draw(font, NULL, a, b, c, d);
}

void print_if_found_texture(uint32_t tex, uint32_t target) {
	if (tex == target) {
		uint32_t* model_id = 0x005606d8;

		log_printf(LL_DEBUG, "FOUND TEXTURE ON MODEL ID %d (0x%08x)\n", *model_id, 0x0057d4fc + (((12 * 2000) + *model_id) * 2));
	}
}

int setDepthWrapper(int face, int unk, float bias, float unk2) {
	// this function is a big hack to disable/enable depth biasing for individual textures
	int (__cdecl *setDepthOrig)(int, int, float, float) = 0x004cf8c0;
	int *faceflags = 0x0058bf5c;
	uint32_t *renderModelFlags = 0x0057b4d4;


	uint8_t modified_tex_flags = 0;
	uint32_t orig_tex_flags = 0;
	uint32_t orig_face_flags = *faceflags;
	int32_t additional_ot_pushback = 0;

	// make sure poly is textured
	if (*faceflags & 1) {

		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)unk + 0x10) + 0x14);
		uint32_t *model_id = 0x005606d8;

		if (tex) {
			switch (current_level) {
			case LEVEL_CRC_HANGAR: // Hangar
				// do not bias
				if (tex->tex_checksum == 0xf4b4432d ||	// concrete texture - used on the light beams... weird
					tex->tex_checksum == 0x9863a474 ||	// wrecked plane wheel
					tex->tex_checksum == 0x2fdba2c8 ||	// wrecked plane tag
					tex->tex_checksum == 0x59d9434c ||	// wrecked plane landing gear
					tex->tex_checksum == 0x4c716d9e) {	// wrecked plane body
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_SCHOOL2: // School II
				// do not bias
				if (tex->tex_checksum == 0x52a2c280 ||	// leap of faith rail chainlink
					tex->tex_checksum == 0x8758d767) {	// bike rack
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_NYCITY:	// NYC
				// 0x808f40af - coffee awning logo
				// 0x89d3240f - cleaners awning logo

				// force bias
				if (tex->tex_checksum == 0xb03f60e7 ||	// street line
					tex->tex_checksum == 0x3877e2c1 ||	// snack bar sign
					tex->tex_checksum == 0x89d3240f ||	// blue awning's logo
					*model_id == 368 ||	// missing tree planter
					tex->tex_checksum == 0xd4f60d61) {	// illuminated window
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}

				// do not bias
				if (tex->tex_checksum == 0x7ae94072 ||	// blue awning
					tex->tex_checksum == 0xa10d8d59 ||	// green awning
					tex->tex_checksum == 0x540a6ef2 ||	// bench back rest
					tex->tex_checksum == 0xd5840538 ||	// bench legs
					tex->tex_checksum == 0xf9656ebd) {	// construction light bulb

					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}

				// extremely targeted one for subway darkness - do not bias
				if (*model_id >= 83 && *model_id <= 86) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_VENICEBEACH:	// Venice Beach
				// do not bias
				if (tex->tex_checksum == 0xfc0fe079) {	// roof window

					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_SKATESTREET:	// Skate Street
				// do not bias
				if (tex->tex_checksum == 0xab803fa8 ||	// chair side
					tex->tex_checksum == 0xd67c8173 ||	// chair back
					tex->tex_checksum == 0xfffffd4d ||	// table legs
					tex->tex_checksum == 0x70ec04da ||	// under cone
					tex->tex_checksum == 0xa2c2c95a) {	// ramp support

					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_PHILADELPHIA:	// Philadelphia
				// do not bias
				if (tex->tex_checksum == 0x3290a444 ||	// bus side
					tex->tex_checksum == 0xb500ae75 ||	// bus side
					tex->tex_checksum == 0xc1ff0913) {	// bus side

					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
				}

				// force bias
				if (tex->tex_checksum == 0x390b37d9) {	// street line
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}
				break;
			case LEVEL_CRC_SKATEHEAVEN:	// Skate Heaven
				// do not bias
				if (tex->tex_checksum == 0x94cbeff8) {	// bleachers
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
				}
				break;
			case LEVEL_CRC_WAREHOUSE:	// Warehouse
				// do not bias
				if (tex->tex_checksum == 0x221c0004) {	// chainlink
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}

				// force bias
				if (tex->tex_checksum == 0x559f8a4b) {	// transworld skateboarding ad
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}
				break;
			case LEVEL_CRC_MALL:
				// do not bias
				if (tex->tex_checksum == 0x7ff460e6 ||	// atrium awning supports
					tex->tex_checksum == 0x791da2c6) {	// shutter
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_SKATEPARK:
				// do not bias
				if (tex->tex_checksum == 0xd98ff9d1 ||	// judge's booth glass
					tex->tex_checksum == 0x337a0802	||	// crowd - low res
					tex->tex_checksum == 0x4190e88b) {	// crowd
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}

				// pull forward in ordering table to avoid getting overwritten by surroundings
				if (tex->tex_checksum == 0xd98ff9d1) {	// judge's booth glass
					additional_ot_pushback = -128;
				}
				break;
			case LEVEL_CRC_DOWNTOWN:
				// do not bias bus stop glass
				if (*model_id == 678 || *model_id == 679 ||
					*model_id == 763 || *model_id == 764 || *model_id == 765) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}
				break;
			case LEVEL_CRC_DOWNHILLJAM:	// DHJ
				// do not bias
				if (tex->tex_checksum == 0x8358c7e1) {	// truss
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
				}

				// force bias
				if (tex->tex_checksum == 0xd8706aaa) {	// emerica ad
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}

				// target spout model ID 690 and force bias
				if (*model_id == 690) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}
				break;
			case LEVEL_CRC_STREETS:
				// do not bias
				if (tex->tex_checksum == 0x10b9b22c ||	// chinatown statue
					tex->tex_checksum == 0x133533d0 ||	// noel
					tex->tex_checksum == 0x3444bb20 ||	// big ramp truss
					tex->tex_checksum == 0x20855154 ||	// door
					tex->tex_checksum == 0x59080df8) {	// truss
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
				}

				// prevent biasing on pagoda glass to avoid seeing it through the pagoda itself
				if (*model_id == 193 || *model_id == 194 || *model_id == 195 || *model_id == 211 ||
					*model_id == 212 || *model_id == 213 || *model_id == 214 || *model_id == 215) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
				}

				// prevent biasing on fountain water to avoid seeing it through the fountain itself
				if (*model_id == 183 || *model_id == 207 || *model_id == 208) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags |= 0x10;
					*faceflags &= ~0x40;
					additional_ot_pushback = -128;
				}

				// force bias on hubba hideout drains and fountain spouts
				if (*model_id == 649 || *model_id == 650 || *model_id == 651 ||
					*model_id == 440 || *model_id == 441 || *model_id == 442 || *model_id == 443) {
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}
				
				break;
			case LEVEL_CRC_ROSWELL:
				// force bias
				if (tex->tex_checksum == 0xf531c535 ||	// restricted area warning sign
					tex->tex_checksum == 0xa4fbc26e) {	// top secret warning sign
					modified_tex_flags = 1;
					orig_tex_flags = tex->flags;

					tex->flags &= ~0x10;
				}
				break;
			default:
				break;
			}

		}
	}

	int result = setDepthOrig(face, unk, bias, unk2);

	// return modified flags to normal
	if (modified_tex_flags) {
		if (*faceflags & 1) {
			struct texture* tex = *(uint32_t **)(*(int *)((uint8_t *)unk + 0x10) + 0x14);
			tex->flags = orig_tex_flags;
		}
		*faceflags = orig_face_flags;
	}

	if (additional_ot_pushback != 0) {
		result += additional_ot_pushback;

		if (result > 4095) {
			result = 4095;
		} else if (result < 0) {
			result = 0;
		}
	}

	return result;
}

void *__fastcall PromptScreenElementWrapper(void *self, void *pad, void *img, void *text, void *font, int32_t homeX, int32_t homeY, int32_t screenX, int32_t screenY) {
	void *(__fastcall *PromptScreenElement)(void *, void *, void *, void *, void *, int32_t, int32_t, int32_t, int32_t) = 0x0049f420;

	// hacky fix for goals percentage positioning: just shift the text creation offset for this call only
	patchByte(0x0049f62a + 1, 3);

	void *result = PromptScreenElement(self, pad, img, text, font, homeX, homeY, screenX, screenY);

	patchByte(0x0049f62a + 1, 0);

	return result;
}

void patchGoalPercentagePosition() {
	// when creating the goals percentage graphic, redirect to a hack for just this call
	patchCall(0x004593f3, PromptScreenElementWrapper);
}

void installGraphicsHackPatches() {
	patchCall(0x0045dfee, fixChecklistFont);
	patchCall(0x00415ed5, fixChecklistFont);

	// fix stats menu font size

	patchDWord(0x004b5336 + 6, 0x1000);	// font 1
	patchDWord(0x004b541a + 6, 0x1000);	// font 2

	// target certain textures for fixing depth biasing
	patchCall(0x004cf4b4, setDepthWrapper);

	patchGoalPercentagePosition();
}