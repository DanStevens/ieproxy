#pragma once
#include "stdafx.h"

namespace ieproxy
{
	namespace algo = boost::algorithm;

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
		ProxySettings(void);
		ProxySettings(const char* serverAddress, bool bypassLocal = true, const char* bypassAddresses = "",
				const char* _autoConfigScriptAddress = "", bool autoDetectSettings = false);

		//~ProxySettings(void);

		bool ProxyEnabled() const { return _proxyEnabled; }
		void ProxyEnabled(const bool value) { _proxyEnabled = value; }

		std::string ProxyServer() const { return _proxyServer; }
		void ProxyServer(const char* value) { _proxyServer = value == nullptr ? "" : value; }
		void ProxyServer(std::string value) { ProxyServer(value.c_str()); }
	
		bool ProxyBypassLocalAddresses() const { return _proxyBypassLocal; }
		void ProxyBypassLocalAddresses(const bool value) { _proxyBypassLocal = value; }

		std::string ProxyBypassAddressList() const { return _proxyBypass; }
		void ProxyBypassAddressList(const char* value);
		void ProxyBypassAddressList(std::string value) { ProxyBypassAddressList(value.c_str()); }

		bool AutoConfigEnabled() const { return _autoConfigEnabled; }
		void AutoConfigEnabled(const bool value) { _autoConfigEnabled = value; }

		std::string AutoConfigScriptAddress() const { return _autoConfigScriptAddress; }
		void AutoConfigScriptAddress(const char* value) { _autoConfigScriptAddress = value == nullptr ? "" : value; }
		void AutoConfigScriptAddress(std::string value) { AutoConfigScriptAddress(value.c_str()); }

		bool AutoDetectSettings() const { return _autoDetect; }
		void AutoDetectSettings(const bool value) { _autoDetect = value; }
	};

}

