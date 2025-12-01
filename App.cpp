#include "App.h"
#include "MainWindow.h"

void App::initialise (const juce::String&)
{
    mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void App::shutdown()
{
    mainWindow.reset();
}

START_JUCE_APPLICATION (App)
