#include "stdafx.h"
#include "ieproxy_api.h"

char* ieproxy::api::StringToCharPtr(std::string& s)
{
    char* result = new char[s.size() + 1];
    std::copy(s.begin(), s.end(), result);
    result[s.size()] = '\0';
    return result;
}

bool ieproxy::api::LoadFromSystem(ieproxy::ProxySettings& p)
{
    const int                        optionCount = 4;
    INTERNET_PER_CONN_OPTION_LIST    optionList;
    INTERNET_PER_CONN_OPTION         optionsArr[optionCount];
    unsigned long                    optionListSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

    optionsArr[0].dwOption = INTERNET_PER_CONN_FLAGS;            // ProxyEnabled(), AutoConfigEnabled()
    optionsArr[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;     // ProxyServer()
    optionsArr[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;     // ProxyBypassAddressList()
    optionsArr[3].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;   // AutoConfigScriptAddress()

    optionList.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    optionList.pszConnection = NULL;
    optionList.dwOptionCount = optionCount;
    optionList.dwOptionError = 0;
    optionList.pOptions = optionsArr;

    BOOL result = InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &optionList, &optionListSize);

    if (result) {
        p.ProxyEnabled((optionsArr[0].Value.dwValue & PROXY_TYPE_PROXY) == PROXY_TYPE_PROXY);
        p.ProxyServer(optionsArr[1].Value.pszValue);
        p.AutoConfigEnabled((optionsArr[0].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL) == PROXY_TYPE_AUTO_PROXY_URL);
        p.AutoConfigScriptAddress(optionsArr[3].Value.pszValue);
        p.AutoDetectSettings((optionsArr[0].Value.dwValue & PROXY_TYPE_AUTO_DETECT) == PROXY_TYPE_AUTO_DETECT);

        // Determine if INTERNET_PER_CONN_PROXY_BYPASS option contains string BYPASS_LOCAL_STR
        std::string bypassAddressList(
            optionsArr[2].Value.pszValue == nullptr ? "" : optionsArr[2].Value.pszValue);
        if (bypassAddressList.empty()) {
            p.ProxyBypassLocalAddresses(false);
            p.ProxyBypassAddressList("");
        } else {
            size_t findResult = bypassAddressList.find(BYPASS_LOCAL_STR);
            if (findResult == std::string::npos) {
                p.ProxyBypassLocalAddresses(false);
                p.ProxyBypassAddressList(bypassAddressList.c_str());
            } else {
                p.ProxyBypassLocalAddresses(true);
                p.ProxyBypassAddressList(bypassAddressList.erase(findResult, strlen(BYPASS_LOCAL_STR)).c_str());
            }
        }

        GlobalFree(optionsArr[1].Value.pszValue);
        GlobalFree(optionsArr[2].Value.pszValue);
        GlobalFree(optionsArr[3].Value.pszValue);
    }
    
    return result ? true : false;
}

bool ieproxy::api::SaveToSystem(const ieproxy::ProxySettings& p)
{
    const int                        optionCount = 4;
    INTERNET_PER_CONN_OPTION_LIST    optionList;
    INTERNET_PER_CONN_OPTION         optionsArr[optionCount];
    unsigned long                    optionListSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

    optionsArr[0].dwOption = INTERNET_PER_CONN_FLAGS;
    optionsArr[0].Value.dwValue = 0;

    // ProxyEnabled()
    if (p.ProxyEnabled())
        optionsArr[0].Value.dwValue |= PROXY_TYPE_PROXY;

    // AutoConfigEnabled()
    if (p.AutoConfigEnabled())
        optionsArr[0].Value.dwValue |= PROXY_TYPE_AUTO_PROXY_URL;

    // AutoDetect()
    if (p.AutoDetectSettings())
        optionsArr[0].Value.dwValue |= PROXY_TYPE_AUTO_DETECT;

    if (optionsArr[0].Value.dwValue == 0)
        optionsArr[0].Value.dwValue = PROXY_TYPE_DIRECT;

    LPSTR proxyServer = ieproxy::api::StringToCharPtr(p.ProxyServer());
    LPSTR autoCfgScript = ieproxy::api::StringToCharPtr(p.AutoConfigScriptAddress());
    LPSTR bypassAddrList = p.ProxyBypassLocalAddresses() ?
        ieproxy::api::StringToCharPtr(p.ProxyBypassAddressList() + ';' + BYPASS_LOCAL_STR) :
        ieproxy::api::StringToCharPtr(p.ProxyBypassAddressList());

    optionsArr[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;     // ProxyServer()
    optionsArr[1].Value.pszValue = proxyServer;

    optionsArr[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;     // ProxyBypassAddressList()
    optionsArr[2].Value.pszValue = bypassAddrList;

    optionsArr[3].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;   // AutoConfigScriptAddress()
    optionsArr[3].Value.pszValue = autoCfgScript;

    optionList.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    optionList.pszConnection = NULL;
    optionList.dwOptionCount = optionCount;
    optionList.dwOptionError = 0;
    optionList.pOptions = optionsArr;

    BOOL result = InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &optionList, optionListSize);

    GlobalFree(proxyServer);
    GlobalFree(autoCfgScript);
    GlobalFree(bypassAddrList);

    return result ? true : false;
}

