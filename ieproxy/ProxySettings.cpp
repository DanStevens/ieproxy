#include "stdafx.h"
#include "ProxySettings.h"

ieproxy::ProxySettings::ProxySettings(void) :
    _proxyEnabled(false),
    _proxyServer(""),
    _proxyBypass(""),
    _proxyBypassLocal(true),
    _autoConfigEnabled(false),
    _autoConfigScriptAddress(""),
    _autoDetect(false)
{ }

ieproxy::ProxySettings::ProxySettings(const char* serverAddress, bool bypassLocal, const char* bypassAddresses,
        const char* autoConfigScriptAddress, bool autoDetectSettings)
{
    ProxyServer(serverAddress);
    ProxyBypassLocalAddresses(bypassLocal);
    AutoConfigScriptAddress(autoConfigScriptAddress);
    AutoDetectSettings(autoDetectSettings);
    ProxyEnabled(!_proxyServer.empty());
    AutoConfigEnabled(!_autoConfigScriptAddress.empty());
    ProxyBypassAddressList(bypassAddresses);
}

void ieproxy::ProxySettings::ProxyBypassAddressList(const char* value)
{
    if (value == nullptr || strlen(value) == 0) {
        _proxyBypass = "";
    } else {
        _proxyBypass = value;
        // Trim spaces and separator characters
        algo::trim_if(_proxyBypass, algo::is_any_of(" ,;"));
        // Replace , with ;
        std::replace(_proxyBypass.begin(), _proxyBypass.end(), ',', ';');
    }
}

