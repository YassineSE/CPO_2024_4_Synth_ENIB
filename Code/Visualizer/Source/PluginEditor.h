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
class VisualizerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        //public juce::AudioAppComponent,
                                        public juce::ChangeListener,
                                        private juce::Timer
{
public:
    VisualizerAudioProcessorEditor (VisualizerAudioProcessor&);
    ~VisualizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };

    void changeState(TransportState newState);

    void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, const juce::Rectangle<int>& thumbnailBounds2);
    void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds, const juce::Rectangle<int>& thumbnailBounds2);

    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();

    void thumbnailChanged();
    void timerCallback();

    VisualizerAudioProcessor& audioProcessor;

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;

    std::unique_ptr<juce::FileChooser> chooser;

    juce::AudioFormatManager formatManager;                
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    juce::AudioThumbnailCache thumbnailCache;         
    juce::AudioThumbnail thumbnail;                         

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizerAudioProcessorEditor)
};
