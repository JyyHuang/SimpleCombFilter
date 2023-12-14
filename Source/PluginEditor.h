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
class SimpleCombFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleCombFilterAudioProcessorEditor (SimpleCombFilterAudioProcessor&);
    ~SimpleCombFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleCombFilterAudioProcessor& audioProcessor;
    
    juce::Slider delaySlider;
    juce::Slider feedbackSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleCombFilterAudioProcessorEditor)
};
