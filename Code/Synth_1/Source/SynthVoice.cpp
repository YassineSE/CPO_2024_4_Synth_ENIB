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
  if (! allowTailOff || ! adsr.isActive())
  {
    clearCurrentNote();
  }
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

  gain.setGainLinear(0.1f);

  

  isPrepared = true;

}

// Public method to update adsr params
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
  adsrParams.attack = attack;
  adsrParams.decay = decay;
  adsrParams.sustain = sustain;
  adsrParams.release = release;

  adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
  // Make sure PrepareToPlay is ready
  jassert(isPrepared);

  if (!isVoiceActive()){
    return;
  }

  synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
  synthBuffer.clear();

  //Oscillator
  juce::dsp::AudioBlock<float> audioBlock {synthBuffer} ;
  osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
  gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));

  //ADSR
  adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

  // Add synth buffer to output Buffer
  for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel){
    outputBuffer.addFrom(channel, startSample, synthBuffer, channel,0, numSamples);

    if (! adsr.isActive())
    {
      clearCurrentNote();
    }
  }

}