#include <stdint.h>

#include <patch.h>
#include <log.h>
#include <util/sb.h>

// extremely complex patch to manage looping sounds created by vehicles:
// the game's original behavior meant only one vehicle could have working sound.
// if there were multiple vehicle, each vehicle would modify the same sound's 
// position, constantly toggling its playing state while moving its source back
// to the same position over and over, causing a single vehicle to repeat the 
// first 1/30th of a second over and over.
//
// our approach is to give each emitter of the same sound a unique ID, and only
// move the sound's position to the closest emitter of that sound, skipping any
// movement that could stop the sound from playing

struct SoundRef {
	uint32_t id;
	uint32_t count;
	int32_t active_emitter;
	uint32_t sound_handle;
};

struct SoundEmitterRef {
	uint32_t id;
	uint32_t sound;
	uint32_t dist;
};

struct CVector {
	int32_t x;
	int32_t y;
	int32_t z;
};

struct stretchyBuffer* car_sounds = NULL;
struct stretchyBuffer* car_sound_emitters = NULL;

uint32_t currentEmitterSerial;

uint32_t getSoundDist(struct CVector *pos) {
	int32_t (__cdecl *Utils_CrapDist)(struct CVector *, struct CVector *) = 0x004c6880;

	// get camera position
	uint32_t current_camera = *((uint32_t *)0x0055c69c);
	struct CVector* camera_pos = 0x0055c42c + (current_camera * 0x29);

	struct CVector tmp = *camera_pos;
	tmp.x <<= 0xc;
	tmp.y <<= 0xc;
	tmp.z <<= 0xc;

	return Utils_CrapDist(pos, &tmp);
}

uint32_t SFX_PlayPos_Wrapper(uint32_t sound_id, struct CVector *pos, uint32_t pitch_offset) {
	uint32_t (__cdecl *SFX_PlayPos)(uint32_t, struct CVector *, uint32_t) = 0x004ab0b0;

	// create lists if they don't exist yet
	if (!car_sounds) {
		car_sounds = sb_alloc(sizeof(struct SoundRef), 4);
	}

	if (!car_sound_emitters) {
		car_sound_emitters = sb_alloc(sizeof(struct SoundEmitterRef), 8);
	}

	// find sound id
	int32_t found_sound = -1;
	for (int i = 0; i < car_sounds->count; i++) {
		struct SoundRef *sound = ((struct SoundRef *)car_sounds->data) + i;
		if (sound->id == sound_id) {
			found_sound = i;
			break;
		}
	}

	// if not found, create a new entry
	if (found_sound == -1) {
		// create the sound
		uint32_t sound_handle = SFX_PlayPos(sound_id, pos, pitch_offset);

		struct SoundRef newref = {
			.id = sound_id,
			.count = 0,
			.active_emitter = -1,
			.sound_handle = sound_handle,
		};

		found_sound = car_sounds->count;
		sb_push_back(car_sounds, &newref);
	}

	struct SoundRef *p_found_sound = ((struct SoundRef *)car_sounds->data) + found_sound;

	// create entry for our new emitter
	struct SoundEmitterRef newemitter = {
		.id = currentEmitterSerial,
		.sound = found_sound,
		.dist = getSoundDist(pos),
	};

	currentEmitterSerial++;
	if (currentEmitterSerial & 0x80000000) {
		currentEmitterSerial = 0;	// don't let the serial number get to negatives to avoid the incredibly unlikely case it becomes -1
	}

	uint32_t emitter_idx = car_sound_emitters->count;
	sb_push_back(car_sound_emitters, &newemitter);

	// if this is the only emitter, set it as active
	if (p_found_sound->count == 0) {
		p_found_sound->active_emitter = emitter_idx;
	}
	else {
		// set this as active emitter if it's closer than current active
		struct SoundEmitterRef *current_active = ((struct SoundEmitterRef *)car_sound_emitters->data) + p_found_sound->active_emitter;
		if (current_active->dist > newemitter.dist) {
			p_found_sound->active_emitter = emitter_idx;
		}
	}

	p_found_sound->count++;

	return newemitter.id;
}

void __cdecl SFX_ModifyPos_Wrapper(uint32_t id, struct CVector *pos, uint32_t delta_dist) {
	uint32_t (__cdecl *SFX_ModifyPos)(uint32_t, struct CVector *, uint32_t) = 0x004ab190;

	// get emitter ref
	uint32_t self_idx = 0;
	struct SoundEmitterRef *p_emitter = NULL;
	for (int i = 0; i < car_sound_emitters->count; i++) {
		struct SoundEmitterRef *emitter = ((struct SoundEmitterRef *)car_sound_emitters->data) + i;
		if (emitter->id == id) {
			self_idx = i;
			p_emitter = emitter;
			break;
		}
	}

	if (!p_emitter) {
		return;
	}

	struct SoundRef *p_sound = ((struct SoundRef *)car_sounds->data) + p_emitter->sound;

	// update distance
	p_emitter->dist = getSoundDist(pos);

	// if this sound is not active but now closer, make it active
	if (p_sound->active_emitter != self_idx) {
		struct SoundEmitterRef *active_emitter = ((struct SoundEmitterRef *)car_sound_emitters->data) + p_sound->active_emitter;
		if (active_emitter->dist > p_emitter->dist) {
			p_sound->active_emitter = self_idx;
		}
	}

	if (p_sound->active_emitter == self_idx) {
		SFX_ModifyPos(p_sound->sound_handle, pos, delta_dist);
	}
}

void __fastcall BaddyDeleteWrapper(int self) {
	void (__fastcall *BaddyDeleteWrapper)(int) = 0x004128c0;

	// find our sound, if it exists
	uint32_t emitter_id = *(uint32_t *)(self + 0x1d0);

	if (emitter_id) {
		*(uint32_t *)(self + 0x1d0) = -1;	// make our sound handle invalid so it doesn't delete a sound used by another emitter

		// find sound in list
		uint32_t self_idx = 0;
		struct SoundEmitterRef *p_emitter = NULL;
		for (int i = 0; i < car_sound_emitters->count; i++) {
			struct SoundEmitterRef *emitter = ((struct SoundEmitterRef *)car_sound_emitters->data) + i;
			if (emitter->id == emitter_id) {
				self_idx = i;
				p_emitter = emitter;
				break;
			}
		}

		// if any active sounds have a higher index, decrement those to account for deletion
		for (int i = 0; i < car_sounds->count; i++) {
			struct SoundRef *sound = ((struct SoundRef *)car_sounds->data) + i;
			if (sound->active_emitter > self_idx) {
				sound->active_emitter--;
			}
		}

		// before deletion, get sound index
		uint32_t self_sound_idx = p_emitter->sound;
		struct SoundRef *p_self_sound = ((struct SoundRef *)car_sounds->data) + self_sound_idx;
		uint32_t self_sound_id = p_self_sound->id;

		// shift other emitters, deleting ourself
		memmove(((struct SoundEmitterRef *)car_sound_emitters->data) + self_idx, ((struct SoundEmitterRef *)car_sound_emitters->data) + self_idx + 1, car_sound_emitters->count - self_idx);
		car_sound_emitters->count--;

		// now decrement the sound's count and delete if needed
		p_self_sound->count--;

		if (p_self_sound->count == 0) {
			// this is the last emitter, forward the sound's handle to the object to be deleted
			*(uint32_t*)(self + 0x1d0) = p_self_sound->sound_handle;

			// if any active sounds refer to a higher sound index, decrement those to account for deletion
			for (int i = 0; i < car_sound_emitters->count; i++) {
				struct SoundEmitterRef *emitter = ((struct SoundEmitterRef *)car_sound_emitters->data) + i;
				if (emitter->sound > self_sound_idx) {
					emitter->sound--;
				}
			}

			// shift other sounds, deleting our sound
			memmove(((struct SoundRef *)car_sounds->data) + self_sound_idx, ((struct SoundRef *)car_sounds->data) + self_sound_idx + 1, car_sounds->count - self_sound_idx);
			car_sounds->count--;
		}
	}

	BaddyDeleteWrapper(self);
}

void SFX_ModifyVol_Wrapper(uint32_t id, uint32_t l, uint32_t r) {
	void(__cdecl * SFX_ModifyVol)(uint32_t, uint32_t, uint32_t) = 0x004ab230;

	// get emitter ref
	uint32_t self_idx = 0;
	struct SoundEmitterRef* p_emitter = NULL;
	for (int i = 0; i < car_sound_emitters->count; i++) {
		struct SoundEmitterRef *emitter = ((struct SoundEmitterRef *)car_sound_emitters->data) + i;
		if (emitter->id == id) {
			self_idx = i;
			p_emitter = emitter;
			break;
		}
	}

	if (!p_emitter) {
		log_printf(LL_ERROR, "FAILED TO FIND SOUND EMITTER 0x%08x\n", id);
		return;
	}

	struct SoundRef *p_sound = ((struct SoundRef *)car_sounds->data) + p_emitter->sound;

	SFX_ModifyVol(p_sound->sound_handle, l, r);
}

void patchCarSFX() {
	patchCall(0x0041276a, SFX_PlayPos_Wrapper);
	patchCall(0x004129c3, SFX_ModifyPos_Wrapper);
	patchCall(0x004128a3, BaddyDeleteWrapper);
	patchCall(0x00412789, SFX_ModifyVol_Wrapper);
}

void installSfxPatches() {
	patchCarSFX();
}
