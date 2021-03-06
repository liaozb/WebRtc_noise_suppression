// WebRtc_official.cpp: main() demo funciton.
//	Description:
//		This project is a standalone noise suppression module which reference from google's official webrtc project.
//		I rewraped it to used in other places.
//		The files used in this project in sync with  official webrtc project respository.(branch de10eea, 2018.12).
//		For more details, please find https://chromium.googlesource.com/external/webrtc/+/master .
//
//	Update history:
//		ver 1.0			2018-12-6
//			1. Update the dependent files in sync with official webrtc project (branch de10eea, 2018.12).
//			2. Wraped noise suppression interface and support 32khz,48khz sample rete.
//			3. Mono supported.Stereo supported can be modify easily.
//		ver 0.1			2018-7
//			1. Capture the code of noise suppression module in webrtc project.
//			2. A simple implement of noise suppression alorithm,supported 8khz and 16khz.
//		
//	TODO List:			
//		-1. noise suppression in hight sample rate							- DONE supproted 32khz and 48 khz.
//		2. Explaining the principle of Webrtc's noise suppression algorithm.- TODO
//
//	Author:		Jagger		jagger.huang@qq.com

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "AudioFile.h"	
#include "WebRtc_NS.h"
#include <windows.h>
using namespace std;	
int main()
{
	AudioFile<float> af;
	 //Testing wav files located in assets directory
	if (SetCurrentDirectory(TEXT("D:\\Repository\\NoiseSuppression\\WebRtc_official\\assets"))) {
		cout << "Changed current path to assets patch to load wav file." << endl;
	}
	//if (!af.load("music_with_noise_16k_21db.wav")) {
	//if (!af.load("music_with_noise_48k_16bit_21db.wav")) {
	//if (!af.load("speech with noise 32k.wav")) {
	if (!af.load("babble_15dB.wav")) {
		return -1;																// Cannot load wav file
	};
	af.printSummary();
	if (af.getNumChannels()>1)
	{
		for (size_t i = 0; i < af.getNumSamplesPerChannel(); i++)
		{
			af.samples[0][i] =float( 0.5*(af.samples[0][i] + af.samples[1][i]) );
		}
	}

	// Usage:
	// step 0: read the resting wav file
	// Step 1: initialize the noise suppression class, set sample rate and ns level.
	WebRtc_NS ns;
	ns.Init(af.getSampleRate(), ns.kHigh);		
	// Capture input date for a test.
	vector<float> output(af.getNumSamplesPerChannel());
	vector<float> input(af.samples[0]);
	// Step 2: process
	ns.Process(&input[0], &output[0],af.getNumSamplesPerChannel());

	// output the processed data.
	vector<vector<float>> output_buffer{ output };
	af.setAudioBuffer(output_buffer);
	af.save("webrtc_ns_kHigh_babble_15dB.wav");
    return 0;
}
