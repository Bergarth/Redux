#pragma once
#include <juce_core/juce_core.h>

class TelnetClient
{
public:
    TelnetClient() = default;
    ~TelnetClient() { close(); }

    bool connect(const juce::String& host, int port = 23, int timeoutMs = 2000) {
        socket = std::make_unique<juce::StreamingSocket>();
        return socket->connect(host, port, timeoutMs);
    }
    
    void close() { socket.reset(); }
    bool isConnected() const { return socket && socket->isConnected(); }

    bool sendLine(const juce::String& cmd) {
        if (!isConnected()) return false;
        auto msg = cmd + "\r"; 
        return socket->write(msg.toRawUTF8(), (int)msg.getNumBytesAsUTF8()) > 0;
    }

    juce::String readAll(int timeoutMs) {
        if (!isConnected()) return {};
        juce::MemoryBlock mb;
        int waited = 0;
        char buf[512];
        while (waited < timeoutMs) {
            if (socket->waitUntilReady(true, 50) > 0) {
                int n = socket->read(buf, sizeof(buf), false);
                if (n > 0) mb.append(buf, n);
            }
            waited += 50;
        }
        return mb.toString().trim();
    }

private:
    std::unique_ptr<juce::StreamingSocket> socket;
};
