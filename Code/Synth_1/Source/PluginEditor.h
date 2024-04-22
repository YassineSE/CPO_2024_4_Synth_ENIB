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
class Synth_1AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Synth_1AudioProcessorEditor (Synth_1AudioProcessor&);
    ~Synth_1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Synth_1AudioProcessor& audioProcessor;
    
    // OSCILLATOR PARAMS
    juce::ComboBox oscillatorCombobox;
    //ADSR
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainkAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscillatorComboboxAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synth_1AudioProcessorEditor)
};
