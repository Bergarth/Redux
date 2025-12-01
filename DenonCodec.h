
#pragma once
#include <juce_core/juce_core.h>

namespace DenonCodec
{
    // Convert MVxx or MVxxx to dB string (e.g., "-22.0 dB")
    inline juce::String mvToDb (const juce::String& payload)
    {
        // payload is everything after "MV"
        if (payload.isEmpty()) return "n/a";
        bool ok = true;
        int iv = payload.getIntValue(); // stops at non-digits
        if (payload.containsOnly ("0123456789"))
            ok = true;
        if (! ok) return payload;

        double v = (payload.length() >= 3 ? (iv / 10.0) : (double) iv);
        double db = v - 80.0; // empirical mapping on Denon: 80.0 => 0.0 dB
        return juce::String (db, 1) + " dB";
    }

    inline juce::String normalizeOnOff (const juce::String& token, const juce::String& onWord = "ON", const juce::String& offWord = "OFF")
    {
        if (token.endsWithIgnoreCase ("ON"))  return onWord;
        if (token.endsWithIgnoreCase ("OFF")) return offWord;
        return token;
    }
}
