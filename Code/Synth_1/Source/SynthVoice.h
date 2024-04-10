/*
  ==============================================================================

    SynthVoice.h
    Created: 7 Apr 2024 3:40:49pm
    Author:  yassine

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
  bool canPlaySound (juce::SynthesiserSound* sound) override;
  void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
  void stopNote (float velocity, bool allowTailOff) override;
  void controllerMoved (int controllerNumber, int newControllerValue) override;
  void pitchWheelMoved (int newPitchWheelValue) override;
  void prepareToPlay( double sampleRate, int samplesPerBlock, int outputChannels);
  void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;


private:
  //ADSR
  juce::ADSR adsr;
  juce::ADSR::Parameters adsrParams;
  
  //OSCILLATOR
  //juce::dsp::Oscillator<float> osc { [](float x) { return std::sin(x); } };
  juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; }};
  juce::dsp::Gain<float> gain;

  bool isPrepared { false};
};