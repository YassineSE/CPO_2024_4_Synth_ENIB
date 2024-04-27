/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <JucePluginDefines.h>

using namespace std;

//==============================================================================
Oscillator_TAPAudioProcessor::Oscillator_TAPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

Oscillator_TAPAudioProcessor::~Oscillator_TAPAudioProcessor()
{
}

//==============================================================================
const juce::String Oscillator_TAPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Oscillator_TAPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Oscillator_TAPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Oscillator_TAPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Oscillator_TAPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Oscillator_TAPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Oscillator_TAPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Oscillator_TAPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Oscillator_TAPAudioProcessor::getProgramName (int index)
{
    return {};
}

void Oscillator_TAPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Oscillator_TAPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{   
    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    oscSine.prepare (spec);
    oscSaw.prepare (spec);
    oscSquare.prepare (spec);
    gain.prepare (spec);
    

}

void Oscillator_TAPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Oscillator_TAPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Oscillator_TAPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> audioBlock { buffer };
    

    if (playing)
    {   
        auto& freq = *apvts.getRawParameterValue("FREQ");
        auto& gainValue = *apvts.getRawParameterValue("GAIN");
        switch (waveId)
        {
        case 1:
            oscSine.setFrequency (freq);
            oscSine.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
            break;
        case 2:
            oscSaw.setFrequency (freq);
            oscSaw.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
        case 3:
            oscSquare.setFrequency (freq);
            oscSquare.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
        default:
            break;
        }
        gain.setGainLinear (gainValue.load());
        gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));

        if (applyADSR){    
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            //cout << "attack: " << attack.load() << "decay: " <<  decay.load() << "sustain: " << sustain.load() << "release: " << release.load() << endl;
            adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
        }    
    }
    
    /*
            for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            {
                for (auto numSample = 0; numSample < buffer.getNumSamples(); ++numSample)
                {
                    buffer.setSample(i, numSample, buffer.getSample(i, numSample) * adsr.getNextSample()); 
                }
            }
    }
    if (!adsr.isActive()){
        playing = false;
        applyADSR = false;
    }
    */
    
}

//==============================================================================
bool Oscillator_TAPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Oscillator_TAPAudioProcessor::createEditor()
{
    return new Oscillator_TAPAudioProcessorEditor (*this);
}

//==============================================================================
void Oscillator_TAPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Oscillator_TAPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Oscillator_TAPAudioProcessor();
}

//Parameters

juce::AudioProcessorValueTreeState::ParameterLayout Oscillator_TAPAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ", "Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f), 440.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.2f), 0.1f));

    //Attack
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    //Decay
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 0.1f));
    //Sustain
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f, 0.1f}, 1.0f));
    //Release
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 0.4f));
    //ADSR note duration
    params.push_back(std::make_unique<juce::AudioParameterFloat>("NOTE", "NoteDuration", juce::NormalisableRange<float> {0.1f, 3.0f, 0.1f}, 1.0f));

    return {params.begin(), params.end()};
}


//Update ADSR Params
void Oscillator_TAPAudioProcessor::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
  adsrParams.attack = attack;
  adsrParams.decay = decay;
  adsrParams.sustain = sustain;
  adsrParams.release = release;

  adsr.setParameters(adsrParams);
}