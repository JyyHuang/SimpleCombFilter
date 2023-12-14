/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleCombFilterAudioProcessorEditor::SimpleCombFilterAudioProcessorEditor (SimpleCombFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(delaySlider);
    delaySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delaySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    delaySlider.setRange(0.0, 1.0, 0.01);
    delaySlider.setValue(0.5);
    delaySlider.addListener(this);

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    feedbackSlider.setRange(0.0, 1.0, 0.01);
    feedbackSlider.setValue(0.5);
    feedbackSlider.addListener(this);

    setSize(400, 200);
}

SimpleCombFilterAudioProcessorEditor::~SimpleCombFilterAudioProcessorEditor()
{
}

//==============================================================================
void SimpleCombFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void SimpleCombFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int margin = 10;
    const int sliderWidth = getWidth() - 2 * margin;

    delaySlider.setBounds(margin, margin, sliderWidth, 20);
    feedbackSlider.setBounds(margin, 2 * margin + 20, sliderWidth, 20);
}

void SimpleCombFilterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
  if (slider == &delaySlider)
    {
        // Handle delay slider change
        processor.setDelayLength(static_cast<int>(delaySlider.getValue() * processor.getMaxDelayInSamples()));
    }
    else if (slider == &feedbackSlider)
    {
        // Handle feedback slider change
        processor.setFeedback(static_cast<float>(feedbackSlider.getValue()));
    }
}