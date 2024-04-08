/*
  ==============================================================================

    SynthSound.h
    Created: 7 Apr 2024 3:41:15pm
    Author:  yassine

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class SynthSound : public juce::SynthesiserSound
{
public:
  bool appliesToNote (int midiNoteNumber) override { return true;}
  bool appliesToChannel (int midiChannel) override { return true;}
};