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
    //Attack
    setSliderParams(attackSlider, attackLabel, "Attack");
    setSliderParams(decaySlider, decayLabel, "Decay");
    setSliderParams(sustainSlider, sustainLabel, "Sustain");
    setSliderParams(releaseSlider, releaseLabel, "Release");

    setSize (400, 300);
}

Synth_1AudioProcessorEditor::~Synth_1AudioProcessorEditor()
{
}

//==============================================================================
void Synth_1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::cadetblue);

}

void Synth_1AudioProcessorEditor::resized()
{   
    const auto bounds = getLocalBounds().reduced (10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight() / 2 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 2);

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    
    


}

void Synth_1AudioProcessorEditor::setSliderParams(juce::Slider& slider, juce::Label& label, juce::String text)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::seagreen); 
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    label.setText (text, juce::dontSendNotification);
    label.attachToComponent (&slider, false);
    label.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}