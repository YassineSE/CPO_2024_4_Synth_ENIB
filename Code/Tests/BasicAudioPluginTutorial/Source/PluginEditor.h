/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicAudioPluginTutorialAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener // inheritance of slider listener functionnality
{
public:
    BasicAudioPluginTutorialAudioProcessorEditor (BasicAudioPluginTutorialAudioProcessor&);
    ~BasicAudioPluginTutorialAudioProcessorEditor() override;

    //==============================================================================
    // This is just a standard paint method
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This is a callback funtion of the slider listener
    void sliderValueChanged(juce::Slider* slider) override;

    //==================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BasicAudioPluginTutorialAudioProcessor& audioProcessor;

    juce::Slider midiVolume; // Creating a new slider object

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicAudioPluginTutorialAudioProcessorEditor)
};
