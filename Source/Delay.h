
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TapeDelay
{

	int p[258];
	float d[44100];
	int gcount;
	int delay;
	int maxdelay;
	int chase;
	long double fpNShape;

	int nSampleFrames = 512;

	float dry = 1.0;
	float wet = 1.0;


	float leanfat;
	float fatwet;
	float storedelay;
	int fatness;
	float sum;
	float floattotal;
	int sumtotal;
	int count;

public:

	float feedback = (0.5 * 1.3);
	int targetdelay = (int)(44000 * 0.5);;


	TapeDelay()
	{

	}
	~TapeDelay()
	{
		// Emtyp destructor
	}

	void Reset()
	{

//		targetdelay = (int)(44000 * 0.5);
//		feedback = (0.5 * 1.3);
		leanfat = 1.0;
		fatwet = fabs(leanfat);
		fatness = (int)8;
		sum = 0.0;
		floattotal = 0.0;
		sumtotal = 0;



		for (int count = 0; count < 257; count++) { p[count] = 0; }
		for (delay = 0; delay < 44100; delay++) { d[delay] = 0.0; }
		maxdelay = 0;
		delay = 0;
		gcount = 0;
		chase = 0;
		fpNShape = 0.0;
	}

	void process(float &inputSample)
	{

		static int noisesource = 0;
		int residue;
		double applyresidue;
		noisesource = noisesource % 1700021; noisesource++;
		residue = noisesource * noisesource;
		residue = residue % 170003; residue *= residue;
		residue = residue % 17011; residue *= residue;
		residue = residue % 1709; residue *= residue;
		residue = residue % 173; residue *= residue;
		residue = residue % 17;
		applyresidue = residue;
		applyresidue *= 0.00000001;
		applyresidue *= 0.00000001;
		inputSample += applyresidue;
		if (inputSample < 1.2e-38 && -inputSample < 1.2e-38) {
			inputSample -= applyresidue;
		}
		//for live air, we always apply the dither noise. Then, if our result is 
		//effectively digital black, we'll subtract it again. We want a 'air' hiss

		if (gcount < 0 || gcount > 128) { gcount = 128; }
		count = gcount;
		if (delay < 0 || delay > maxdelay) { delay = maxdelay; }
		sum = inputSample + (d[delay] * feedback);
		p[count + 128] = p[count] = sumtotal = (int)(sum * 8388608.0);
		switch (fatness)
		{
		case 32: sumtotal += p[count + 127]; //note NO break statement.
		case 31: sumtotal += p[count + 113]; //This jumps to the relevant tap
		case 30: sumtotal += p[count + 109]; //and then includes all smaller taps.
		case 29: sumtotal += p[count + 107];
		case 28: sumtotal += p[count + 103];
		case 27: sumtotal += p[count + 101];
		case 26: sumtotal += p[count + 97];
		case 25: sumtotal += p[count + 89];
		case 24: sumtotal += p[count + 83];
		case 23: sumtotal += p[count + 79];
		case 22: sumtotal += p[count + 73];
		case 21: sumtotal += p[count + 71];
		case 20: sumtotal += p[count + 67];
		case 19: sumtotal += p[count + 61];
		case 18: sumtotal += p[count + 59];
		case 17: sumtotal += p[count + 53];
		case 16: sumtotal += p[count + 47];
		case 15: sumtotal += p[count + 43];
		case 14: sumtotal += p[count + 41];
		case 13: sumtotal += p[count + 37];
		case 12: sumtotal += p[count + 31];
		case 11: sumtotal += p[count + 29];
		case 10: sumtotal += p[count + 23];
		case 9: sumtotal += p[count + 19];
		case 8: sumtotal += p[count + 17];
		case 7: sumtotal += p[count + 13];
		case 6: sumtotal += p[count + 11];
		case 5: sumtotal += p[count + 7];
		case 4: sumtotal += p[count + 5];
		case 3: sumtotal += p[count + 3];
		case 2: sumtotal += p[count + 2];
		case 1: sumtotal += p[count + 1];
		}

		floattotal = (float)(sumtotal / fatness + 1);
		floattotal /= 8388608.0;
		floattotal *= fatwet;
		if (leanfat < 0) { storedelay = sum - floattotal; }
		else { storedelay = (sum * (1 - fatwet)) + floattotal; }

		chase += abs(maxdelay - targetdelay);
		if (chase > 9000)
		{
			if (maxdelay > targetdelay) { d[delay] = storedelay; maxdelay -= 1; delay -= 1; if (delay < 0) { delay = maxdelay; } d[delay] = storedelay; }
			if (maxdelay < targetdelay) { maxdelay += 1; delay += 1; if (delay > maxdelay) { delay = 0; } d[delay] = storedelay; }
			chase = 0;
		}
		else
		{
			d[delay] = storedelay;
		}

		gcount--;
		delay--;
		if (delay < 0 || delay > maxdelay) { delay = maxdelay; }
		//yes this is a second bounds check. it's cheap, check EVERY time
		inputSample = (inputSample * dry) + (d[delay] * wet);

		//32 bit dither, made small and tidy.
		int expon; std::frexpf((float)inputSample, &expon);
		long double dither = (rand() / (RAND_MAX * 7.737125245533627e+25)) * pow(2, expon + 62);
		inputSample += (dither - fpNShape); fpNShape = dither;
		//end 32 bit dither

	}

	void calcParams(int sampleRate)
	{
		nSampleFrames = sampleRate;
//		dry = pow(0.0, 2);
//		wet = pow(1.0, 2);
//		targetdelay = (int)(44000 * 0.5);
//		feedback = (0.0 * 1.3);
		leanfat =1.0;
		fatwet = fabs(leanfat);
		fatness = (int)8;
		sum = 0.0;
		floattotal = 0.0;
		sumtotal = 0;
		
	}






private:





};

