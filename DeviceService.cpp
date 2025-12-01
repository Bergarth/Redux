#include "DeviceService.h"
using namespace juce;

bool DeviceService::connectAndScan(const String& ip)
{
    snapshot = DeviceSnapshot();
    snapshot.ip = ip;

    if (!telnet.isConnected()) {
        if (!telnet.connect(ip, 23, 1500)) return false;
    }
    snapshot.connected = true;
    
    telnet.sendLine("CV?");
    String raw = telnet.readAll(1500);
    StringArray lines = StringArray::fromLines(raw);
    
    for (auto& line : lines) {
        line = line.trim();
        if (line.startsWith("CV")) {
            String code = line.substring(2).upToFirstOccurrenceOf(" ", false, false);
            if (code == "END") continue;
            
            if (code.startsWith("SW")) {
                snapshot.subs.addIfNotAlreadyThere(code);
            } 
            else if (code.startsWith("T") && (code.contains("AC") || code.contains("T"))) {
                // Tactile logic
                if (code.contains("FL") || code.contains("FR") || code.contains("RL") || code.contains("RR") || code.contains("S"))
                     snapshot.speakers.addIfNotAlreadyThere(code); 
                else snapshot.tactile.addIfNotAlreadyThere(code); 
            }
            else snapshot.speakers.addIfNotAlreadyThere(code);
        }
    }
    
    // Manual Alias
    if (snapshot.speakers.contains("RHL")) snapshot.speakers.addIfNotAlreadyThere("TRL");
    if (snapshot.speakers.contains("RHR")) snapshot.speakers.addIfNotAlreadyThere("TRR");

    return true;
}

bool DeviceService::loadFromFile(const File& f)
{
    var json = JSON::parse(f);
    if (json.isVoid()) return false;
    
    snapshot = DeviceSnapshot(); // clear
    snapshot.connected = true;
    snapshot.ip = json["ipAddress"].toString();
    
    auto list = json["detectedChannels"];
    if (list.isArray())
    {
        for (int i = 0; i < list.size(); ++i)
        {
            String id = list[i]["commandId"].toString();
            
            // Normalize SLA/SRA -> SL/SR for display, but keep original if needed
            if (id == "SLA") snapshot.speakers.add("SL");
            else if (id == "SRA") snapshot.speakers.add("SR");
            
            if (id.startsWith("SW")) snapshot.subs.add(id);
            else if (id.startsWith("T") && !id.contains("RL") && !id.contains("RR") && !id.contains("FL") && !id.contains("FR"))
                snapshot.tactile.add(id);
            else snapshot.speakers.add(id);
        }
    }
    return true;
}
