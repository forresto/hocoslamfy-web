/*
 * Hocoslamfy, main program file
 * Copyright (C) 2014 Nebuleon Fumika <nebuleon@gcw-zero.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdbool.h>
#include <stddef.h>

#include "SDL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "main.h"
#include "init.h"
#include "audio.h"
#include "platform.h"
#include "game.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

static bool         Continue                             = true;
static bool         Error                                = false;

       SDL_Surface* Screen                               = NULL;
       SDL_Surface* TitleScreenFrames[TITLE_FRAME_COUNT] = { NULL };
       SDL_Surface* BackgroundImages[BG_LAYER_COUNT]     = { NULL };
       SDL_Surface* CharacterFrames                      = NULL;
       SDL_Surface* ColumnImage                          = NULL;
       SDL_Surface* CollisionImage                       = NULL;
       SDL_Surface* GameOverFrame                        = NULL;

       TGatherInput GatherInput;
       TDoLogic     DoLogic;
       TOutputFrame OutputFrame;

#ifdef __EMSCRIPTEN__
static Uint32 em_last_ticks = 0;

static void em_loop(void) {
	Uint32 now = SDL_GetTicks();
	Uint32 duration = em_last_ticks ? (now - em_last_ticks) : 16;
	if (duration == 0) duration = 1;
	em_last_ticks = now;

	GatherInput(&Continue);
	if (!Continue) { emscripten_cancel_main_loop(); return; }
	DoLogic(&Continue, &Error, (Uint32)(duration * FieldScrollMultiplier));
	if (!Continue) { emscripten_cancel_main_loop(); return; }
	OutputFrame();
}

EMSCRIPTEN_KEEPALIVE void TriggerBoostDown(void) {
	SDL_Event e;
	SDL_zero(e);
	e.type = SDL_KEYDOWN;
	e.key.state = SDL_PRESSED;
	e.key.keysym.sym = SDLK_SPACE;
	SDL_PushEvent(&e);
}

EMSCRIPTEN_KEEPALIVE void TriggerBoostUp(void) {
	SDL_Event e;
	SDL_zero(e);
	e.type = SDL_KEYUP;
	e.key.state = SDL_RELEASED;
	e.key.keysym.sym = SDLK_SPACE;
	SDL_PushEvent(&e);
}

#endif

int main(int argc, char* argv[])
{
	Initialize(&Continue, &Error);
#ifdef __EMSCRIPTEN__
	SetMusicMuted(1);
	emscripten_set_main_loop(em_loop, 0, 1);
#else
	Uint32 Duration = 16;
	while (Continue)
	{
		GatherInput(&Continue);
		if (!Continue)
			break;
		DoLogic(&Continue, &Error, Duration);
		if (!Continue)
			break;
		OutputFrame();
		Duration = ToNextFrame();
	}
	Finalize();
	return Error ? 1 : 0;
#endif
}
