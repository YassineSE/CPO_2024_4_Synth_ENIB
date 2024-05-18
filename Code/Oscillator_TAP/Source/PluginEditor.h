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
class Oscillator_TAPAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, private juce::Timer
{
public:
    Oscillator_TAPAudioProcessorEditor (Oscillator_TAPAudioProcessor&);
    ~Oscillator_TAPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* Button) override;
    void updateToggleState(juce::Button* Button);
    void setFreqSliderParams(juce::Slider& slider, juce::Label& label, juce::String text);
    void setADSRSliderParams(juce::Slider& slider, juce::Label& label, juce::String text); 
    void waveTypeChanged();
    void verifyPlayStop();
    void getLFOActivationState(juce::Button* Button);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Oscillator_TAPAudioProcessor& audioProcessor;

    juce::TextButton startButton;
    juce::Label startLabel;
    
    //Frequency Control
    juce::Slider freqSlider;
    juce::Label freqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;


    //Combobox Control
    juce::ComboBox waveType;
    juce::Label waveLabel;

    //Gain Control
    juce::Slider gainSlider;
    juce::Label gainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;


    //ADSR
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    // ADSR Activate Button
    juce::ToggleButton adsrActivateButton;
    juce::Label adsrActivateLabel;

    // ADSR Note Duration
    juce::Slider noteSlider;
    juce::Label noteLabel;


    // ADSR Componants slider attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainkAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    std::unique_ptr<SliderAttachment> noteSliderAttachment;


    // LFO + Slider Attachment
    juce::Slider lfoSlider;
    juce::Label lfoLabel; 
    std::unique_ptr<SliderAttachment> lfoSliderAttachment;
    
    // LFO Activate Button 
    juce::ToggleButton lfoActivateButton;
    juce::Label lfoActivateLabel;

    //Timer
    void timerCallback() final{
        stopTimer();
        audioProcessor.adsr.noteOff();
        verifyPlayStop();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator_TAPAudioProcessorEditor)
};
