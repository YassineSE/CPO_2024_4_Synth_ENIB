/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Oscillator_TAPAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif 
{
public:
    //==============================================================================
    Oscillator_TAPAudioProcessor();
    ~Oscillator_TAPAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts; 
    bool playing = false;
    int waveId = 1;

    //ADSR
    juce::ADSR adsr;
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    bool applyADSR;

   

private:
    //juce::dsp::Oscillator<float> osc { [](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200 };
    juce::dsp::Oscillator<float> osc;
    juce::dsp::Oscillator<float> oscSine {[] (float x) { return std::sin (x); }, 200} ;
    juce::dsp::Oscillator<float> oscSaw {[] (float x){ return x / juce::MathConstants<float>::pi; }, 200};
    juce::dsp::Oscillator<float> oscSquare {[](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200 };
    
    juce::dsp::Oscillator<float> lfo {[] (float x) {return std::sin (x); }, 200};
    
    juce::dsp::Gain<float> gain;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //ADSR
    
    juce::ADSR::Parameters adsrParams;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator_TAPAudioProcessor)
};
