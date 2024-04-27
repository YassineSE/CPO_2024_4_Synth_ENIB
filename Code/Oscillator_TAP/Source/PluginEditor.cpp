/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Oscillator_TAPAudioProcessorEditor::Oscillator_TAPAudioProcessorEditor (Oscillator_TAPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    //Play Button
    addAndMakeVisible (startButton);
    startButton.setButtonText ("Play");
    startButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    startButton.addListener (this); // [7] 
    addAndMakeVisible (startLabel);
    startLabel.setColour (juce::Label::backgroundColourId, juce::Colours::black);
    startLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    startLabel.setJustificationType (juce::Justification::centred);


    //Frequency Controls
    freqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ", freqSlider);
    setFreqSliderParams(freqSlider, freqLabel, "Frequency");



    //Combobox Controls
    addAndMakeVisible(waveType);
    waveType.addItemList(juce::StringArray {"Sine", "Saw", "Square"}, 1);
    waveType.setTextWhenNothingSelected("Sine");
    waveType.onChange = [this] {waveTypeChanged(); };
    waveLabel.setText ("Select WaveForm", juce::dontSendNotification);
    waveLabel.attachToComponent (&waveType, false);
    waveLabel.setJustificationType(juce::Justification::centred);

    //Gain Controls
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(gainLabel);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::seagreen); 
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    gainLabel.setText ("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent (&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);


    //ADSR Controls
    addAndMakeVisible(adsrActivateButton);
    addAndMakeVisible(adsrActivateLabel);
    adsrActivateButton  .onClick = [this] { updateToggleState (&adsrActivateButton);   };
    adsrActivateLabel.setText ("Toggle ADSR", juce::dontSendNotification);
    adsrActivateLabel.attachToComponent (&adsrActivateButton, true);
    adsrActivateLabel.setJustificationType(juce::Justification::centred);

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainkAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    setADSRSliderParams(attackSlider, attackLabel, "Attack");
    setADSRSliderParams(decaySlider, decayLabel, "Decay");
    setADSRSliderParams(sustainSlider, sustainLabel, "Sustain");
    setADSRSliderParams(releaseSlider, releaseLabel, "Release");

    //ADSR Note Duration
    addAndMakeVisible(noteSlider);
    addAndMakeVisible(noteLabel);
    noteSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "NOTE", noteSlider);
    noteSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    noteSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 30, 20);
    noteSlider.setTextValueSuffix(" s");
    noteSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    noteSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    noteSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    noteSlider.setColour(juce::Slider::thumbColourId, juce::Colours::seagreen); 
    noteSlider.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    noteLabel.setText ("Note Duration", juce::dontSendNotification);
    noteLabel.attachToComponent (&noteSlider, false);
    noteLabel.setJustificationType(juce::Justification::centredBottom);

    setSize (400, 300);
}

Oscillator_TAPAudioProcessorEditor::~Oscillator_TAPAudioProcessorEditor()
{
}

//==============================================================================
void Oscillator_TAPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void Oscillator_TAPAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    const auto bounds = getLocalBounds().reduced (10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight() / 2 - padding;
    const auto sliderStartX = 0;
    const auto sliderStartY = 3*padding;
    const auto adsrStartX = bounds.getWidth() / 3 + 3*padding;
    const auto adsrWidth = 60;
    const auto adsrHeight = 70;
    freqSlider.setBounds(sliderStartX + 1.5*padding, sliderStartY, sliderWidth, sliderHeight);
    waveType.setBounds(sliderStartX +padding, freqSlider.getBottom() + 3*padding, 100, 30);
    startButton.setBounds(sliderStartX + padding, waveType.getBottom() + padding, 100,40);
    gainSlider.setBounds(bounds.getWidth() - 2*padding, 2*padding, 30, bounds.getHeight() - 2*padding);


    adsrActivateButton.setBounds(bounds.getWidth() /2 + 6*padding, 0, 40,40);
    noteSlider.setBounds(adsrStartX, adsrActivateButton.getBottom() + padding, 130, 50);
    attackSlider.setBounds(adsrStartX, noteSlider.getBottom() + padding, adsrWidth, adsrHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, noteSlider.getBottom() + padding, adsrWidth, adsrHeight);
    sustainSlider.setBounds(attackSlider.getX(), attackSlider.getBottom() + 2*padding, adsrWidth, adsrHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, attackSlider.getBottom() + 2*padding, adsrWidth, adsrHeight);
    


}
void Oscillator_TAPAudioProcessorEditor::buttonClicked (juce::Button* button) // [2]
{
    if (button == &startButton)                                                      // [3]
    {   
        if (!audioProcessor.playing) 
        {   
            audioProcessor.playing = true;
            auto& noteDuration = *audioProcessor.apvts.getRawParameterValue("NOTE");
            //std::cout << (int) (noteDuration.load() * 1000.0 )<< std::endl;
            startTimer((int) (noteDuration.load() * 1000.0 ));
            audioProcessor.adsr.noteOn();
        }
        else
        {
            audioProcessor.playing = !audioProcessor.playing;
        }
        verifyPlayStop();      // [6]
    }
}


// Freq Slider Params Function
void Oscillator_TAPAudioProcessorEditor::setFreqSliderParams(juce::Slider& slider, juce::Label& label, juce::String text)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::seagreen); 
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    slider.setTextValueSuffix(" Hz");
    label.setText (text, juce::dontSendNotification);
    label.attachToComponent (&slider, false);
    label.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

// ADSR Slider Params Function
void Oscillator_TAPAudioProcessorEditor::setADSRSliderParams(juce::Slider& slider, juce::Label& label, juce::String text)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 15);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::palegreen);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::seagreen);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::seagreen); 
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::palegreen);
    slider.setTextValueSuffix(" s");
    label.setText (text, juce::dontSendNotification);
    label.attachToComponent (&slider, false);
    label.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

//waveTypeCHanged
void Oscillator_TAPAudioProcessorEditor::waveTypeChanged()
{   
    auto tmpWaveId = waveType.getSelectedId();
    audioProcessor.waveId = tmpWaveId;
    audioProcessor.playing = false;
    verifyPlayStop();
}

void Oscillator_TAPAudioProcessorEditor::verifyPlayStop()
{
    if (audioProcessor.playing == true)
    {
        startButton.setButtonText("Stop");
        startButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        
        
    }
    else 
    {
        startButton.setButtonText("Play");
        startButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        
    }    
}


// update Toggle State
void Oscillator_TAPAudioProcessorEditor::updateToggleState(juce::Button* Button)
{
    auto state = Button->getToggleState();
    juce::String stateString = state ? "ON" : "OFF";
    juce::Logger::writeToLog (" ADSR Button changed to " + stateString);
    audioProcessor.applyADSR = state;

}

