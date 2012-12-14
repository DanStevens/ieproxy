// Copyright © Dan Stevens 2012
// 
// Distributed under the terms of the Boost Software License, Version 1.0.
// See accompanying file *LICENSE.txt*.
//
// Dependancies:
//   - string.h
//   - iostream.n
//   - boost/program_options.hpp
//
#pragma once
#include "stdafx.h"
#include "ProxySettings.h"
#include "ieproxy_api.h"

#define VERSION "0.1.1"
#define YES "Yes"
#define NO  "No"
#define NOVAL "0"

namespace ieproxy
{

    namespace po = boost::program_options;

    /**
     * The command-line interface for ieproxy. In most cases it should be used as follows:
     * 
     *    #include <iostream>
     *    #include <CommandLineUI.h>
     *    
     *    int main(int argc, char* argv[])
     *    {
     *        ieproxy::CommandLineUI cl(std::cout, std::cerr);
     *        return cl.Run(argc, argv);
     *    }
     */
    class CommandLineUI
    {

    private:

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
        /**
         * Constructs a new CommandLineUI object.
         * \param cout A reference to an ostream object to which standard output messages will be sent.
         * \param cerr A reference to an ostream object to which error messages will be sent.
         */
        CommandLineUI(std::ostream& cout, std::ostream& cerr);
        ~CommandLineUI(void);
    
        /**
         * Runs command-line interface for ieproxy.
         * \param argc The count of command line arguments.
         * \param argv An array of command line arguments.
         * \return 2 if there was an retrieving or setting proxy setting, 1 if there was an error parsing the command
         *           line arguments or 0 if success.
         */
        int Run(int argc, char* argv[]);

        /**
         * The ProxySettings object created by Run().
         */
        const ProxySettings& GetProxySettings() { return _proxySettings; }
        
    };

}