/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synth_1AudioProcessorEditor::Synth_1AudioProcessorEditor (Synth_1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    //Attachments
    oscillatorComboboxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", oscillatorCombobox);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainkAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    
    //MakeVisible
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    attackLabel.setText ("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent (&attackSlider, false);
    attackLabel.setJustificationType(juce::Justification::centred);

    
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    decayLabel.setText ("Decay", juce::dontSendNotification);
    decayLabel.attachToComponent (&decaySlider, false);
    decayLabel.setJustificationType(juce::Justification::centred);
    
    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    sustainLabel.setText ("Sustain", juce::dontSendNotification);
    sustainLabel.attachToComponent (&sustainSlider, false);
    sustainLabel.setJustificationType(juce::Justification::centred);
    
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    releaseLabel.setText ("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent (&releaseSlider, false);
    releaseLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackLabel);
    addAndMakeVisible (attackLabel);

    setSize (800, 300);
}

Synth_1AudioProcessorEditor::~Synth_1AudioProcessorEditor()
{
}

//==============================================================================
void Synth_1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawFittedText ("Synth ENIB", getLocalBounds(), juce::Justification::centredTop, 1);
}

void Synth_1AudioProcessorEditor::resized()
{
    attackSlider.setBounds(getWidth()/4 + 100, getHeight()/2 - 50, 200, 100);
    decaySlider.setBounds(getWidth()/4 + 200 ,getHeight()/2 - 50, 200, 100);
    sustainSlider.setBounds(getWidth()/4 + 300, getHeight()/2 - 50, 200, 100);
    releaseSlider.setBounds(getWidth()/4 + 400, getHeight()/2 - 50, 200, 100);

}
