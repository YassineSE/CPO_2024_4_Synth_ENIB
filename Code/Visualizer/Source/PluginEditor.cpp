/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VisualizerAudioProcessorEditor::VisualizerAudioProcessorEditor (VisualizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    setSize(600, 700);
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    thumbnail.addChangeListener(this);        

    //setAudioChannels(2, 2);
}

VisualizerAudioProcessorEditor::~VisualizerAudioProcessorEditor()
{
}

//==============================================================================
void VisualizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Rectangle<int> thumbnailBounds(10, 110, getWidth() - 20, (getHeight() - 130) / 2.0);
    juce::Rectangle<int> thumbnailBounds2(10, (getHeight() / 2) + 55, getWidth() - 20, (getHeight() - 130) / 2.0);

    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds, thumbnailBounds2);
    else
        paintIfFileLoaded(g, thumbnailBounds, thumbnailBounds2);

}

void VisualizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
}

void VisualizerAudioProcessorEditor::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
        juce::File{},
        "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

    if (file != juce::File{})
    {
        auto* reader = formatManager.createReaderFor(file);

        if (reader != nullptr)
        {
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled(true);
            thumbnail.setSource(new juce::FileInputSource(file));                            // [7]
            readerSource.reset(newSource.release());
        }
    }
        });
}

void VisualizerAudioProcessorEditor::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused)) {
        changeState(Starting);
    }
    else if (state == Playing) {
        changeState(Pausing);
    }
}

void VisualizerAudioProcessorEditor::stopButtonClicked()
{
    if (state == Paused)
        changeState(Stopped);
    else
        changeState(Stopping);
}

void VisualizerAudioProcessorEditor::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:
            playButton.setButtonText("Play state: Stopped");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(false);
            transportSource.setPosition(0.0);
            break;

        case Starting:
            playButton.setButtonText("state: Starting");
            transportSource.start();
            break;

        case Playing:
            playButton.setButtonText("Pause state: Playing");
            stopButton.setButtonText("Stop");
            stopButton.setEnabled(true);
            break;

        case Pausing:
            playButton.setButtonText("state: Pausing");
            transportSource.stop();
            changeState(Paused);
            break;

        case Paused:
            playButton.setButtonText("Resume state: Paused");
            stopButton.setButtonText("Return to Zero");
            break;

        case Stopping:
            playButton.setButtonText("state: Stopping");
            transportSource.stop();
            changeState(Stopped);
            break;
        }
    }
}

void VisualizerAudioProcessorEditor::paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, const juce::Rectangle<int>& thumbnailBounds2)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds2);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No Fourier transform to show", thumbnailBounds2, juce::Justification::centred, 1);
}

void VisualizerAudioProcessorEditor::paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, const juce::Rectangle<int>& thumbnailBounds2)
{
    startTimer(40);                                  // [11]
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.fillRect(thumbnailBounds2);

    g.setColour(juce::Colours::black);                               // [8]

    auto audioPosition = (float)transportSource.getCurrentPosition();
    float shownDuration = 1.0;

    thumbnail.drawChannels(g,                                      // [9]
        thumbnailBounds,
        audioPosition,                                    // start time
        audioPosition + shownDuration,             // end time
        1.0f);                                  // vertical zoom
}

void VisualizerAudioProcessorEditor::thumbnailChanged()
{

    repaint();

}

void VisualizerAudioProcessorEditor::timerCallback()
{
    repaint();
}
