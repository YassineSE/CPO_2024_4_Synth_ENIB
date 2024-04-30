/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicAudioPluginTutorialAudioProcessorEditor::BasicAudioPluginTutorialAudioProcessorEditor (BasicAudioPluginTutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // This is where our plugin’s editor size is set.
    setSize (200, 200);

    // These define the parameters of our slider object
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Volume");
    midiVolume.setValue(1.0);
    midiVolume.addListener(this); // add slider listener to our volume slider

    // This function adds the slider to the editor
    addAndMakeVisible(&midiVolume);
}

BasicAudioPluginTutorialAudioProcessorEditor::~BasicAudioPluginTutorialAudioProcessorEditor()
{
}

//==============================================================================
void BasicAudioPluginTutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("MIDI Volume", 0,0, getWidth(), 30, juce::Justification::centred, 1);
}

void BasicAudioPluginTutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

void BasicAudioPluginTutorialAudioProcessorEditor::sliderValueChanged (juce::Slider* slider) 
{
    audioProcessor.noteOnVel = midiVolume.getValue(); // match noteOnVel value with the slider value

}