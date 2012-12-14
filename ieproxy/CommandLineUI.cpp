#include "StdAfx.h"
#include "CommandLineUI.h"
#include "ieproxy_api.h"

ieproxy::CommandLineUI::CommandLineUI(std::ostream& cout, std::ostream& cerr) :
    _cout(cout), _cerr(cerr), _optDesc("Options")
{
    // Declare the supported options.
    _optDesc.add_options()
        ("auto-config,a",
            po::value<std::string>()->implicit_value(""),
            "Enable automatic proxy configuration and optionally specify the location of the configuration script.")
        ("no-auto-config",
            "Disable automatic proxy configuration. Overrides '--auto-config'.")
        ("auto-detect", "Enable automatic detection of proxy settings")
        ("no-auto-detect", "Disable automatic detection of proxy settings. Overrides '--auto-detect'.")
        ("bypass,b", po::value<std::string>()->implicit_value(""),
            "A comma separated list of address for which the proxy server will be bypassed. If no argument is given, "
            "no addresses will be bypassed, except local addresses if --bypass-local is set.")
        ("bypass-local,l", "Bypass local addresses")
        ("no-bypass-local", "Do not bypass local addresses. Overrides --bypass-local.")
        ("default,d",
            "Reset proxy settings to default settings. When combined with other options, proxy settings will be reset "
            "before applying the other options.")
        ("proxy-server,p", po::value<std::string>()->implicit_value(""),
            "Enable proxy server and optionally set the address and port of the proxy")
        ("no-proxy-server", "Disable proxy server. Overrides --proxy-server.")
        ("help", "Show help")
        ("version", "Show version information and attributions")
    ;
}


ieproxy::CommandLineUI::~CommandLineUI(void)
{
}

void ieproxy::CommandLineUI::printHelp()
{
    // Build a usage string from the options defined in _optDesc
    //std::ostringstream sstream;
    //for (size_t i = 0; i < _optDesc.options().size(); i++) {
    //    po::option_description* opt = _optDesc.options()[i].get();
    //    std::string shortName(opt->canonical_display_name(shortNameCode));
    //    std::string longName(opt->long_name());
    //    std::string arg(opt->format_parameter());

    //    bool isShortName = shortName != longName;
    //    bool isLongName = !longName.empty();
    //    bool isArg = !arg.empty();
    //    
    //    sstream << '[';
    //    if (isShortName)
    //        sstream << shortName;
    //    if (isShortName && isLongName)
    //        sstream << '|';
    //    if (isLongName)
    //        sstream << "--" << longName;
    //    if (isArg)
    //        sstream << " [" << arg << ']';
    //    sstream << "] ";
    //}

    // Create a options_description object from the _optDesc object that displays usage information nicely formatted
    //po::options_description usageDesc("Usage");
    //usageDesc.add_options()
    //    (_ieproxy, sstream.str().c_str())
    //;
    
    _cout << std::endl
          << "Output or modify proxy settings for Internet Explorer." << std::endl
          << std::endl
          << "Usage: " << _ieproxy << " [options]" << std::endl
          << "       " << _ieproxy << " --help"    << std::endl
          << "       " << _ieproxy << " --version" << std::endl
          << std::endl
          << "If called without any arguments, the current proxy settings are" << std::endl
          << "output." << std::endl
          << std::endl
          << _optDesc
          << std::endl
          << "Return values:" << std::endl
          << "  0:  No errors" << std::endl
          << "  1:  Error with command line arguments" << std::endl
          << "  2:  Error retrieving or setting proxy setting" << std::endl
          << std::endl
          << "Examples:" << std::endl
          << std::endl
          << "  ieproxy -a http://wpad/wpad.dat" << std::endl
          << std::endl
          << "  Enables automatic proxy configuration using the script located" << std::endl
          << "  at 'http://wpad/wpad.dat'" << std::endl
          << std::endl
          << "  ieproxy -p cache.example.com:8080" << std::endl
          << std::endl
          << "  Configures manual proxy settings to use the proxy server" << std::endl
          << "  'cache.example.com' on the port 8080." << std::endl
          << std::endl
          << "  ieproxy -p cache.example.com:8080 -l -b foo,bar.com" << std::endl
          << std::endl
          << "  Configures manual proxy settings to bypass proxy server for the" << std::endl
          << "  address 'foo', 'bar.com' and all local addresses." << std::endl;
}

void ieproxy::CommandLineUI::printVersion()
{
    _cout << std::endl
          << _ieproxy << ", version " VERSION << std::endl
          << "Copyright Dan Stevens 2012" << std::endl
          << "Distributed under the Boost Software License, Version 1.0" << std::endl
          << "(See accompanying file LICENSE.txt or copy at" << std::endl
          << "http://www.boost.org/LICENSE_1_0.txt)" << std::endl
          << std::endl
          << "Website: https://github.com/DanStevens/ieproxy" << std::endl
          << "Contact: dan.stevens@doomy.co.uk" << std::endl;
}

void ieproxy::CommandLineUI::procOptions(int argc, char* argv[])
{    
    po::store(po::parse_command_line(argc, argv, _optDesc), _optMap);
    po::notify(_optMap);
}

void ieproxy::CommandLineUI::printProxySettings(const ieproxy::ProxySettings& p)
{
    
    _cout << std::endl
          << "Type '" << _ieproxy << " --help' for usage" << std::endl
          << std::endl
          << "Automatic configuration:" << std::endl
           << "  Automatically detect settings?      : " << (p.AutoDetectSettings() ? YES : NO) << std::endl
          << "  Use automatic configuration script? : " << (p.AutoConfigEnabled() ?  YES : NO) << std::endl
          << "  Script URL                          : " << p.AutoConfigScriptAddress() << std::endl
          << std::endl
          << "Proxy server:" << std::endl
          << "  Use proxy server?       : " << (p.ProxyEnabled() ? YES : NO) << std::endl
          << "  Server                  : " << p.ProxyServer() << std::endl
          << "  No proxy for            : " << p.ProxyBypassAddressList() << std::endl
          << "  Bypass local addresses? : " << (p.ProxyBypassLocalAddresses() ? YES : NO) << std::endl;
}

int ieproxy::CommandLineUI::Run(int argc, char* argv[])
{
    _ieproxy = argv[0];
    
    // Process options and handle invalid options
    try {
        procOptions(argc, argv);
    } catch (po::error& e) {
        _cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Check for --help option
    if (_optMap.count("help")) {
        printHelp();
        return 0;
    }

    // Check for --version option
    if (_optMap.count("version")) {
        printVersion();
        return 0;
    }

    // Load system settings except when --default option is given
    if (!_optMap.count("default") && !api::LoadFromSystem(_proxySettings)) {
        _cerr << std::endl
              << "Error retrieving current proxy settings." << std::endl
              << "Use --default option to reset proxy settings to default." << std::endl;
        return 1;
    }

    // Check for zero arguments
    if (argc <= 1) {
        printProxySettings(_proxySettings);
        return 0;
    }

    // Process --auto-config option
    if (_optMap.count("auto-config")) {
        _proxySettings.AutoConfigEnabled(true);
        std::string autoConfig = _optMap["auto-config"].as<std::string>();
        if (!autoConfig.empty())
            _proxySettings.AutoConfigScriptAddress(autoConfig);
    }

    // Process --no-auto-config option
    if (_optMap.count("no-auto-config"))
        _proxySettings.AutoConfigEnabled(false);

    // Process --auto-detect option
    if (_optMap.count("auto-detect"))
        _proxySettings.AutoDetectSettings(true);

    // Process --no-auto-detect option
    if (_optMap.count("no-auto-detect"))
        _proxySettings.AutoDetectSettings(false);

    // Process --bypass option
    if (_optMap.count("bypass")) {
        _proxySettings.ProxyBypassAddressList(_optMap["bypass"].as<std::string>());
    }

    // Process --bypass-local option
    if (_optMap.count("bypass-local"))
        _proxySettings.ProxyBypassLocalAddresses(true);

    // Process --no-auto-detect option
    if (_optMap.count("no-bypass-local"))
        _proxySettings.ProxyBypassLocalAddresses(false);
    
    // Process --proxy-server option
    if (_optMap.count("proxy-server")) {
        _proxySettings.ProxyEnabled(true);
        std::string proxyServer = _optMap["proxy-server"].as<std::string>();
        if (!proxyServer.empty())
            _proxySettings.ProxyServer(proxyServer);
    }

    // Process --no-proxy-server option
    if (_optMap.count("no-proxy-server"))
        _proxySettings.ProxyEnabled(false);

    // Save _proxySettings to system
    if (!api::SaveToSystem(_proxySettings)) {
        _cerr << std::endl << "Error saving proxy settings" << std::endl;
        return 2;
    }

    return 0;
}
