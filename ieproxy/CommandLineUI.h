#pragma once
#include "stdafx.h"
#include "ProxySettings.h"


#define VERSION "0.1.0"
#define YES "Yes"
#define NO  "No"
#define IEPROXY "ieproxy"
#define NOVAL "0"

namespace ieproxy
{
	

	namespace po = boost::program_options;

	class CommandLineUI
	{

	private:
		//static const int _shortNameCode = 4;
		//static const int _longNameCode = 0;

		const char* _ieproxy;
		std::ostream& _cout;
		std::ostream& _cerr;
		po::options_description _optDesc;
		po::variables_map _optMap;
		ProxySettings _proxySettings;

		void procOptions(int argc, char* argv[]);
		void printHelp();
		void printVersion();
		void printAutoConfigSettings(const ieproxy::ProxySettings& p);
		void printProxySettings(const ieproxy::ProxySettings& p);

	public:
		CommandLineUI(std::ostream& cout, std::ostream& cerr);
		~CommandLineUI(void);
	
		int Run(int argc, char* argv[]);
		const ProxySettings& GetProxySettings() { return _proxySettings; }
		
	};

}