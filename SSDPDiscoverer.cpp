#include "SSDPDiscoverer.h"

using namespace juce;

static String getHeader (const String& blob, const String& key)
{
    auto lines = StringArray::fromLines (blob);
    for (auto& ln : lines)
    {
        auto s = ln.trim();
        if (s.isEmpty()) continue;
        if (s.startsWithIgnoreCase (key + ":"))
            return s.fromFirstOccurrenceOf (":", false, false).trim();
    }
    return {};
}

juce::Array<SSDPDiscoverer::Result> SSDPDiscoverer::discover (int timeoutMs)
{
    Array<Result> results;

    DatagramSocket sock (false);
    sock.setEnablePortReuse (true);
    sock.bindToPort (0);

    auto multicastAddr = IPAddress::getAllAddresses().isEmpty() ? IPAddress{} : IPAddress("239.255.255.250");
    const int ssdpPort = 1900;

    // Minimal M-SEARCH for all devices; most AVRs respond to this
    auto payload = String(
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "MAN: \"ssdp:discover\"\r\n"
        "MX: 1\r\n"
        "ST: ssdp:all\r\n"
        "\r\n");

    sock.write (multicastAddr.toString(), ssdpPort, payload.toRawUTF8(), (int)payload.getNumBytesAsUTF8());

    const auto endTime = Time::getMillisecondCounterHiRes() + timeoutMs;
    char buf[2048] = {0};

    while (Time::getMillisecondCounterHiRes() < endTime)
    {
        String senderIP;
        int senderPort = 0;
        const int readBytes = sock.read (buf, sizeof(buf)-1, false, senderIP, senderPort);
        if (readBytes <= 0)
        {
            Thread::sleep (25);
            continue;
        }
        buf[readBytes] = 0;
        String resp = String::fromUTF8 (buf, readBytes);

        Result r;
        r.raw      = resp;
        r.server   = getHeader (resp, "SERVER");
        r.st       = getHeader (resp, "ST");
        r.usn      = getHeader (resp, "USN");

        auto loc = getHeader (resp, "LOCATION");
        if (loc.isNotEmpty())
        {
            r.location = URL (loc);
            r.ip = r.location.getDomain();
        }
        if (r.ip.isEmpty())
            r.ip = senderIP;

        // Filter duplicates by IP + LOCATION
        bool dupe = false;
        for (auto& e : results)
            if (e.ip == r.ip && e.location.toString (true) == r.location.toString (true))
            { dupe = true; break; }

        if (!dupe && r.ip.isNotEmpty())
            results.add (std::move (r));
    }

    return results;
}
