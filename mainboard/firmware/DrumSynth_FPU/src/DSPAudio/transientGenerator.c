/*
 * transientGenerator.c
 *
 *  Created on: 29.06.2012
 *      Author: Julian
 */


#include "transientGenerator.h"

//---------------------------------------------------------------
void transient_init(TransientGenerator* transient)
{
	transient->pitch 	= 1.f;
	transient->output 	= 0;
	transient->phase	= 0;
	transient->waveform	= 0;
	transient->volume	= 1.f;


};
//---------------------------------------------------------------
void transient_trigger(TransientGenerator* transient)
{
	//reset the phase to the beginning when the generator is triggered
	transient->phase	= 0;
}
//---------------------------------------------------------------
void transient_calcBlock(TransientGenerator* transient, int16_t* buf, const uint8_t size)
{
	uint8_t i;
	for(i=0;i<size;i++)
	{
		uint32_t phase = transient->phase;
		//if phase is < than table size we increment it
		//phase += (phase<2311061504u) * (transient->pitch*(1<<20)); //2311061504 => 2204<<20
		//transient->phase = phase;
		phase = phase >> 20;
		/*
		if(transient->waveform==0)
		{
			//click short
			if(phase == 0) {
				buf[i] = transient->volume * 32512;
			}
			else if(phase == 1) {
				buf[i] = transient->volume * -32512;
			} else {
				buf[i] = 0;
			}
		} else if(transient->waveform==1)

		{
			//click long
			if((phase>>5) <= 5) {
				buf[i] = transient->volume * 32512;
			}
			else {
				buf[i] = 0;
			}
			//buf[i] = transient->volume * 32512 * transientVolumeTable[phase>>5];

		}
		else*/
		{
			buf[i] = transient->volume*(transientData[transient->waveform][phase]<<8) * transientVolumeTable[phase>>5];
		}
		//if phase is < than table size we increment it
		transient->phase += (transient->phase<2311061504u) * (transient->pitch*(1<<20)); //2311061504 => 2204<<20
		//transient->phase = phase;
	}
}
//---------------------------------------------------------------
void transient_calc(TransientGenerator* transient)
{
	uint32_t phase = transient->phase;
	//if phase is < than table size we increment it
	phase += (phase<2311061504u) * (transient->pitch*(1<<20)); //2311061504 => 2204<<20
	transient->phase = phase;
	phase = phase >> 20;
	if(transient->waveform==0)
	{
		//click
		transient->output = transient->volume * 32512 * transientVolumeTable[phase>>5];

	}
	else
	{
		transient->output = transient->volume*(transientData[transient->waveform-1][phase]<<8) * transientVolumeTable[phase>>5];
	}




};
//---------------------------------------------------------------
