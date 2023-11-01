#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"

#define BEEP PBout(8)	// PB9


void BEEP_Init(void);	//≥ı ºªØ

void Sound(u16 frq);

void play_music(void);

		 				    
#endif
