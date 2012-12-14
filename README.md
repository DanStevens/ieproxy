ieproxy
=======

ieproxy is command-line interface for Windows for outputting or modifying proxy
settings for Internet Explorer. This is achieved by using the Windows WinINET
API.


Install
-------

Download *Release/ieproxy.exe* and call directly from the Windows Command Line
or from a Batch or Cygwin script. If you wish to be able to call it using
`ieproxy` from any location, copy the file to a location referenced in the 
PATH` environment variable.


Usage
-----

    C:\>ieproxy --help
    
    Output or modify proxy settings for Internet Explorer.

    Usage: ieproxy [options]
           ieproxy --help
           ieproxy --version

    If called without any arguments, the current proxy settings are
    output.

    Options:
      -a [ --auto-config ] arg  Enable automatic proxy configuration and optionally
                                specify the location of the configuration script.
      --no-auto-config          Disable automatic proxy configuration. Overrides 
                                '--auto-config'.
      --auto-detect             Enable automatic detection of proxy settings
      --no-auto-detect          Disable automatic detection of proxy settings. 
                                Overrides '--auto-detect'.
      -b [ --bypass ] arg       A comma separated list of address for which the 
                                proxy server will be bypassed. If no argument is 
                                given, no addresses will be bypassed, except local 
                                addresses if --bypass-local is set.
      -l [ --bypass-local ]     Bypass local addresses
      --no-bypass-local         Do not bypass local addresses. Overrides 
                                --bypass-local.
      -d [ --default ]          Reset proxy settings to default settings. When 
                                combined with other options, proxy settings will be
                                reset before applying the other options.
      -p [ --proxy-server ] arg Enable proxy server and optionally set the address 
                                and port of the proxy
      --no-proxy-server         Disable proxy server. Overrides --proxy-server.
      --help                    Show help
      --version                 Show version information and attributions

    Return values:
      0:  No errors
      1:  Error with command line arguments
      2:  Error retrieving or setting proxy setting


Examples
--------

    ieproxy -a http://wpad/wpad.dat

Enables automatic proxy configuration using the script located
at 'http://wpad/wpad.dat'

    ieproxy -p cache.example.com:8080

Configures manual proxy settings to use the proxy server
'cache.example.com' on the port 8080.

    ieproxy -p cache.example.com:8080 -l -b foo,bar.com

Configures manual proxy settings to bypass proxy server for the
address 'foo', 'bar.com' and all local addresses.


Compilation
------------

This software uses the Boost C++ library, and references the following headers:

 * boost/program_options.hpp
 * boost/algorithm/string.hpp

In order to compile in Visual Studio 2010, follow these instructions:

 1.  Download and launch the *BoostPro 1.51.0 (32-bit) Installer* from
     http://www.boostpro.com/download/.
 2.  In the installation wizard proceed through the pages until the *Select
     default varients* page.
 3.  Set the *Visual C++ 10.0 (Visual Studio 2010)* checkbox and click *Next*.
 4.  In the list of components, set the *Boost ProgramOptions*/*VC10.0* node and
     click *Next*.
 5.  Make a note of the destination folder and click *Install*.
 6.  After installation is complete, go to the Windows desktop, right-click
     *My Computer* and click *Properties*.
 7.  Click *Advanced system settings* and click *Environment Variables...*
 8.  Within the *System variables* group, click *New...* and create a new
     environment variable with the name `BOOST_X86` and set the value to
     location that Boost was installed to.
 9.  Download or check out the source from github and open ieproxy.sln in
     Visual Studio.
 10. Wait for Visual Studio to parse included files. The project properties
     already include a reference the `BOOST_X86` environment variable.
 11. When Visual Studio has parsing files, the project can be built via
     *Build* > *Build Solution*. You may wish to change the build configuration
     to *Release* first.


Copyright & License
-------

Copyright © Dan Stevens 2012

The software and its source code is licensed under the terms of the
Boost Software License, Version 1.0. See accompanying file *LICENSE.txt*.


Credits
-------

Developed by Dan "Doomy" Stevens  
Contact: dan.stevens@doomy.co.uk  
Website: https://github.com/DanStevens/ieproxy  
