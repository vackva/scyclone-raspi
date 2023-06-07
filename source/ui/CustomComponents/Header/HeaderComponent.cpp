//
// Created by valentin.ackva on 16.03.2023.
//

#include "HeaderComponent.h"

HeaderComponent::HeaderComponent(AudioPluginAudioProcessor &p, juce::AudioProcessorValueTreeState &parameters) : detailButton("detailButton",
                                                                                                                              juce::DrawableButton::ButtonStyle::ImageFitted),
                                                                                                                 apvts(parameters),
                                                                                                                 audioProcessor(p){

    labels.vaeSynth.setText("Scyclone", juce::dontSendNotification);
    labels.vaeSynth.setFont(CustomFontLookAndFeel::getCustomFontBold().withHeight(30.f));
    labels.vaeSynth.setJustificationType(juce::Justification::centred);
    labels.vaeSynth.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(ColorPallete::WHITE));
    //addAndMakeVisible(labels.vaeSynth);

    labels.neutralTransfer.setText("Neural Transfer", juce::dontSendNotification);
    labels.neutralTransfer.setFont(CustomFontLookAndFeel::getCustomFont().withHeight(19.f));
    labels.neutralTransfer.setJustificationType(juce::Justification::centred);
    labels.neutralTransfer.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(ColorPallete::TEXT2));
    //addAndMakeVisible(labels.neutralTransfer);

    labels.inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    labels.inputGainLabel.setFont(CustomFontLookAndFeel::getCustomFont().withHeight(15.f));
    labels.inputGainLabel.setJustificationType(juce::Justification::centredRight);
    labels.inputGainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(ColorPallete::TEXT2));
    addAndMakeVisible(labels.inputGainLabel);

    inputGainSlider.setSliderStyle(juce::Slider::LinearBar);
    inputGainSlider.setLookAndFeel(&customLinearVolumeSliderLookAndFeel);
    inputGainSlider.setTextValueSuffix(" " + parameters.getParameter(PluginParameters::INPUT_GAIN_ID)->getLabel());
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, PluginParameters::INPUT_GAIN_ID, inputGainSlider);
    inputGainSlider.setTextBoxIsEditable(false);
    addAndMakeVisible(inputGainSlider);

    labels.outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    labels.outputGainLabel.setFont(CustomFontLookAndFeel::getCustomFont().withHeight(15.f));
    labels.outputGainLabel.setJustificationType(juce::Justification::centredRight);
    labels.outputGainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromString(ColorPallete::TEXT2));
    addAndMakeVisible(labels.outputGainLabel);

    outputGainSlider.setSliderStyle(juce::Slider::LinearBar);
    outputGainSlider.setLookAndFeel(&customLinearVolumeSliderLookAndFeel);
    outputGainSlider.setTextValueSuffix(" " + parameters.getParameter(PluginParameters::OUTPUT_GAIN_ID)->getLabel());
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, PluginParameters::OUTPUT_GAIN_ID, outputGainSlider);
    outputGainSlider.setTextBoxIsEditable(false);
    addAndMakeVisible(outputGainSlider);

    detailButton.setClickingTogglesState(true);
    detailButton.setImages(detailsButtonOff.get(),
                           detailsButtonOff.get(),
                           detailsButtonOn.get(),
                           detailsButtonOff.get(),
                           detailsButtonOn.get(),
                           detailsButtonOn.get(),
                           detailsButtonOn.get(),
                           detailsButtonOn.get());
    addAndMakeVisible(detailButton);
    detailButton.onClick = [this] () {
        //temporary bug fix - otherwise shape distortion
        outputGainSlider.repaint();
        inputGainSlider.repaint();
    };
    detailButton.onStateChange = [this](){
        bool buttonDown = detailButton.getToggleState();
        onParameterControlViewChange(buttonDown);
        audioProcessor.advancedParameterControlVisible = buttonDown;
        //audioProcessor.onUpdateUnautomatableParameters();
    };

    detailButton.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colours::transparentBlack);
    this->setInterceptsMouseClicks(true, true);
}

HeaderComponent::~HeaderComponent() {
    inputGainSlider.setLookAndFeel(nullptr);
    outputGainSlider.setLookAndFeel(nullptr);
}

void HeaderComponent::resized() {

    int width = getWidth();
    int height = getHeight();

    labels.vaeSynth.setBounds(width * 49 / 1400, height * 21 / 700, width * 127 / 1400, height * 30 / 700);
    labels.neutralTransfer.setBounds(width * 208 / 1400, height * 29 / 700, width * 121 / 1400, height * 19 / 700);
    labels.inputGainLabel.setBounds(width * 950 / 1400, height * 21 / 700, width * 95 / 1400, height * 24 / 700);
    inputGainSlider.setBounds(width * 1050 / 1400, height * 21 / 700, width * 73 / 1400, height * 24 / 700);
    labels.outputGainLabel.setBounds(width * 1125 / 1400, height * 21 / 700, width * 95 / 1400, height * 24 / 700);
    outputGainSlider.setBounds(width * 1225 / 1400, height * 21 / 700, width * 73 / 1400, height * 24 / 700);
    detailButton.setBounds(width - (width * 80 / 1400), height * 24 / 700, width * 35 / 1400, height * 19 / 700);
    scycloneTypoSection.setBounds(width * 49.f / 1400, height * 21.f / 700, width * 127.f / 1400, height * 30.f / 700);
    neuralTransferTypoSection.setBounds(width * 197.f / 1400, height * 21.f / 700, width * 121.f / 1400, height * 30.f / 700);
}

void HeaderComponent::paint(juce::Graphics &g) {
    //scycloneLogo1->drawAt(g, getWidth() - (int)((float)scycloneLogo1->getWidth())/2, 21, 100);
    scycloneTypo->drawWithin(g, scycloneTypoSection, juce::RectanglePlacement::yBottom, 100);
    neuralTransferTypo->drawWithin(g, neuralTransferTypoSection, juce::RectanglePlacement::yBottom, 100);
}
