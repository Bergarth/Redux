#include "HomePage.h"

using namespace juce;

HomePage::HomePage(DeviceService& svcRef, ProjectSession& sessRef) 
    : svc(svcRef), session(sessRef)
{
    addAndMakeVisible(scanBtn);
    scanBtn.setButtonText("AUTO SCAN");
    scanBtn.setColour(TextButton::buttonColourId, Colour(0xff2a2a2a));
    scanBtn.onClick = [this] { runScan(); };

    addAndMakeVisible(ipField);
    ipField.setText("192.168.40.48");
    ipField.setJustification(Justification::centred);
    ipField.setColour(TextEditor::backgroundColourId, Colour(0xff222222));

    addAndMakeVisible(connectBtn);
    connectBtn.setButtonText("GET CONFIG");
    connectBtn.setColour(TextButton::buttonColourId, Colour(0xff2a2a2a));
    connectBtn.setColour(TextButton::textColourOffId, Colour(0xffffbf00));
    connectBtn.onClick = [this] { runManualConnect(); };

    startTimer(500);
}

bool HomePage::isInterestedInFileDrag (const StringArray& files) { return true; }
void HomePage::filesDropped (const StringArray& files, int x, int y)
{
    if (svc.loadFromFile(File(files[0]))) {
        statusMsg = "Loaded File.";
        syncToSession(); // <--- CRITICAL: Save to Brain
    } else statusMsg = "Parse Failed.";
    repaint();
}

void HomePage::runScan() {
    isScanning = true; statusMsg = "Scanning..."; repaint();
    Timer::callAfterDelay(100, [this] {
        auto results = svc.scan();
        if (!results.isEmpty()) { ipField.setText(results[0].ip); runManualConnect(); }
        else { statusMsg = "No Device Found."; isScanning = false; repaint(); }
    });
}

void HomePage::runManualConnect() {
    String ip = ipField.getText();
    isScanning = true; statusMsg = "Probing " + ip + "..."; repaint();
    Timer::callAfterDelay(100, [this, ip] {
        if (svc.connectAndScan(ip)) {
            statusMsg = "Config Loaded.";
            syncToSession(); // <--- CRITICAL: Save to Brain
        } else statusMsg = "Connection Failed.";
        isScanning = false; repaint();
    });
}

void HomePage::syncToSession() {
    session.setConfig(svc.getSnapshot());
}

void HomePage::timerCallback() { repaint(); }

void HomePage::drawItem(Graphics& g, String label, int cx, int cy, bool present, Colour activeCol) {
    Colour offCol = Colour(0xff222222);
    Colour textCol = present ? Colours::black : Colours::grey;
    auto bounds = Rectangle<int>(cx - 30, cy - 20, 60, 40);
    g.setColour(present ? activeCol : offCol);
    g.fillRoundedRectangle(bounds.toFloat(), 4.0f);
    if (present) {
        g.setColour(Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle(bounds.toFloat(), 4.0f, 1.0f);
    }
    g.setColour(textCol);
    g.setFont(Font("Segoe UI", 13.0f, Font::bold));
    g.drawText(label, bounds, Justification::centred);
}

void HomePage::paint(Graphics& g)
{
    g.fillAll(Colour(0xff121212)); 
    g.setColour(Colour(0xffffbf00));
    g.setFont(24.0f);
    g.drawText("SYSTEM CONFIGURATION", 20, 20, 300, 30, Justification::left);
    g.setColour(Colours::grey);
    g.setFont(16.0f);
    g.drawText(statusMsg, 20, 50, 400, 20, Justification::left);

    // Use LOCAL data from service to draw map (immediate feedback)
    auto r = getLocalBounds();
    int cx = r.getCentreX();
    int cy = r.getCentreY() + 30;
    
    g.setColour(Colour(0xff1a1a1a)); g.fillEllipse(cx-30, cy-30, 60, 60);
    g.setColour(Colour(0xff444444)); g.drawEllipse(cx-30, cy-30, 60, 60, 2.0f);
    
    auto snap = svc.getSnapshot();
    auto spk = snap.speakers;
    auto sub = snap.subs;
    auto tac = snap.tactile;
    
    auto has = [&](String id) { return spk.contains(id); };
    auto hasSub = [&](String id) { return sub.contains(id); };

    Colour spkCol = Colour(0xffffbf00); 
    Colour subCol = Colour(0xffbf4000); 
    Colour tacCol = Colour(0xff00bfff);

    drawItem(g, "FL",  cx - 120, cy - 120, has("FL"), spkCol);
    drawItem(g, "C",   cx,       cy - 140, has("C"), spkCol);
    drawItem(g, "FR",  cx + 120, cy - 120, has("FR"), spkCol);
    drawItem(g, "FWL", cx - 200, cy - 60,  has("FWL"), spkCol);
    drawItem(g, "FWR", cx + 200, cy - 60,  has("FWR"), spkCol);
    drawItem(g, "SL",  cx - 140, cy,       has("SL"), spkCol);
    drawItem(g, "SR",  cx + 140, cy,       has("SR"), spkCol);
    drawItem(g, "SBL", cx - 90,  cy + 120, has("SBL"), spkCol);
    drawItem(g, "SBR", cx + 90,  cy + 120, has("SBR"), spkCol);

    String FL_Lbl = has("TFL") ? "TFL" : "FHL";
    String FR_Lbl = has("TFR") ? "TFR" : "FHR";
    String RL_Lbl = has("TRL") ? "TRL" : "RHL";
    String RR_Lbl = has("TRR") ? "TRR" : "RHR";

    drawItem(g, FL_Lbl, cx - 60, cy - 80, has("FHL") || has("TFL"), spkCol);
    drawItem(g, FR_Lbl, cx + 60, cy - 80, has("FHR") || has("TFR"), spkCol);
    drawItem(g, RL_Lbl, cx - 60, cy + 80, has("RHL") || has("TRL"), spkCol);
    drawItem(g, RR_Lbl, cx + 60, cy + 80, has("RHR") || has("TRR"), spkCol);

    drawItem(g, "SW1", cx - 200, cy + 180, hasSub("SW1") || hasSub("SW"), subCol);
    drawItem(g, "SW2", cx + 200, cy + 180, hasSub("SW2"), subCol);
    drawItem(g, "SW3", cx - 260, cy + 180, hasSub("SW3"), subCol);
    drawItem(g, "SW4", cx + 260, cy + 180, hasSub("SW4"), subCol);

    bool tacPresent = !tac.isEmpty();
    drawItem(g, "TACTILE", cx, cy + 60, tacPresent, tacCol);
}

void HomePage::resized()
{
    auto r = getLocalBounds();
    int x = r.getWidth() - 320;
    scanBtn.setBounds(x, 20, 80, 30);
    ipField.setBounds(x + 90, 20, 100, 30);
    connectBtn.setBounds(x + 200, 20, 100, 30);
}
