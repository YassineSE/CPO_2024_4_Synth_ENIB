/*
  ==============================================================================

    SynthVoice.cpp
    Created: 7 Apr 2024 3:40:49pm
    Author:  yassine

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
  return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;

}
void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
  osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
  adsr.noteOn();
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
  adsr.noteOff();
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{

}
void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{

}
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
  //ADSR
  adsr.setSampleRate(sampleRate);

  
  //OSCILLATOR 
  juce::dsp::ProcessSpec spec;
  spec.maximumBlockSize = samplesPerBlock;
  spec.sampleRate = sampleRate;
  spec.numChannels = outputChannels;

  osc.prepare(spec);
  gain.prepare(spec);

  gain.setGainLinear(0.8f);

  isPrepared = true;

}
void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
  // Make sure PrepareToPlay is ready
  jassert(isPrepared);

  //Oscillator
  juce::dsp::AudioBlock<float> audioBlock {outputBuffer} ;
  osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
  gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));

  //ADSR
  adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);

}