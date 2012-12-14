// Copyright © Dan Stevens 2012
// 
// Distributed under the terms of the Boost Software License, Version 1.0.
// See accompanying file *LICENSE.txt*.
//
// Dependancies:
//   - string.h
//   - boost/algorithm/string.hpp
//
#pragma once
#include "stdafx.h"

namespace ieproxy
{
    namespace algo = boost::algorithm;

    /**
     * Represents proxy settings for Internet Explorer. Use this instances of this class to retrieve individual
     * settings after calling ieproxy::api::LoadFromSystem() or to prepare proxy settings and commit them as a batch
     * using ieproxy::api::SaveToSystem();
     */
    class ProxySettings
    {

    // Instance members
    private:
        bool _proxyEnabled;
        std::string _proxyServer;
        bool _proxyBypassLocal;
        std::string _proxyBypass;
        bool _autoConfigEnabled;
        std::string _autoConfigScriptAddress;
        bool _autoDetect;

    public:
        
        /**
         * Constructs a new ProxySettings object with default settings.
         */
        ProxySettings(void);

        /**
         * Constructs a new ProxySettings object. 
         * \param serverAddress The address and port of the proxy server in the form "{server}:{port}" e.g.
         *                      "cache.example.com:8080". If this argument is an empty string, ProxyEnabled() will
         *                      return false.
         * \param bypassLocal If set to true, the proxy server will be bypassed for local addresses.
         * \param bypassAddresses A list of address for which the proxy server will be bypassed, separated by commas
         *                        or semicolons.
         * \param autoConfigScriptAddress The location of the script for automatic proxy configuration If this argument 
         *                                is an empty string, AutoConfigScriptAddress() will return false.
         * \param autoDetectSettings Set to true to enable automatic detection of proxy settings.
         */
        ProxySettings(const char* serverAddress, bool bypassLocal = true, const char* bypassAddresses = "",
                const char* autoConfigScriptAddress = "", bool autoDetectSettings = false);

        //! Returns true if use of a proxy server is enabled, otherwise false.
        bool ProxyEnabled() const { return _proxyEnabled; }

        //! Set whether or not a proxy server is used.
        //! \param value true to enable the user of a proxy server, otherwise false.
        void ProxyEnabled(const bool value) { _proxyEnabled = value; }

        //! Get the address of the proxy server.
        std::string ProxyServer() const { return _proxyServer; }

        //! Set the address and port number of the proxy server.
        //! \param value The address and port of the proxy server in the form "{server}:{port}" e.g. "cache.example.com:8080".
        void ProxyServer(const char* value) { _proxyServer = value == nullptr ? "" : value; }

        //! Set the address and port number of the proxy server.
        //! \param value The address and port of the proxy server in the form "{server}:{port}" e.g. "cache.example.com:8080".
        void ProxyServer(std::string value) { ProxyServer(value.c_str()); }
    
        
        //! Returns true if the proxy server will be bypassed for local addresses, otherwise false.
        bool ProxyBypassLocalAddresses() const { return _proxyBypassLocal; }

        //! Set whether or not the proxy server will be bypassed for local addresses.
        //! \param value true to bypass local addresses, otherwise false.
        void ProxyBypassLocalAddresses(const bool value) { _proxyBypassLocal = value; }

        //! Returns a semicolon separated list of addresses for which the proxy server will bypassed.
        std::string ProxyBypassAddressList() const { return _proxyBypass; }

        //! Set the list of addresses for which the proxy server will be bypassed.
        //! \param The list of address, separated by colons or semicolons.
        void ProxyBypassAddressList(const char* value);

        //! Set the list of addresses for which the proxy server will be bypassed.
        //! \param The list of address, separated by colons or semicolons.
        void ProxyBypassAddressList(std::string value) { ProxyBypassAddressList(value.c_str()); }

        //! Returns true if an automatic configuration script is to be used.
        bool AutoConfigEnabled() const { return _autoConfigEnabled; }

        //! Set whether or not an automatic configuration script is to be used.
        //! \param value true to enable the user of an automatic configuration script, otherwise false.
        void AutoConfigEnabled(const bool value) { _autoConfigEnabled = value; }

        //! Returns the location of the automatic configuration script
        std::string AutoConfigScriptAddress() const { return _autoConfigScriptAddress; }

        //! Set the location of the automatic configuration script
        //! \param value The location of the automatic configuration script e.g. "http://wpad/wpad.dat"
        void AutoConfigScriptAddress(const char* value) { _autoConfigScriptAddress = value == nullptr ? "" : value; }

        //! Set the location of the automatic configuration script
        //! \param value The location of the automatic configuration script e.g. "http://wpad/wpad.dat"
        void AutoConfigScriptAddress(std::string value) { AutoConfigScriptAddress(value.c_str()); }

        //! Return true if automatic detection of proxy settings is enabled.
        bool AutoDetectSettings() const { return _autoDetect; }

        //! Set whether or not to attempt automatic detection of proxy settings.
        //! \param value true to enable automatic detection of proxy settings, otherwise false.
        void AutoDetectSettings(const bool value) { _autoDetect = value; }
    };

}

