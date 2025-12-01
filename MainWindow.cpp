#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(const juce::String& name)
: DocumentWindow(name,
                 juce::Desktop::getInstance().getDefaultLookAndFeel()
                   .findColour(ResizableWindow::backgroundColourId),
                 DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setResizable(true, true);
    setContentOwned(new MainComponent(), true);
    centreWithSize(1200, 800);
    setVisible(true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
