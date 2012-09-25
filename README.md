ieproxy
=======

A Windows batch script for outputting or modifying proxy settings for Internet
Explorer. This is achieved by using the Windows REG command to modify registry
settings.

Usage
-----

    C:\>ieproxy /?

    Output or modify proxy settings for Internet Explorer

    ieproxy <operation> [<arguments>]

    The following operations are supported:

      autocfg [-e <url> | -d]

        Display or change automatic proxy configuration script settings. If
        none of the following arguments are supplied, the status and
        location of the automatic proxy configuration script are displayed.

        -e <url>  Enable automatic proxy configuration using the script
                  at the location given by <url>.
        -d        Disable automatic proxy configuration

      proxy [-s <server>[:<port>] | -e [<server>[:<port>]] | -d]

        Display or configure proxy settings manually. If none of the
        following arguments are supplied, the proxy settings are displayed.

        -s <server>[:<port>]    Sets the address and port of the proxy
                                server.
        -e [<server>[:<port>]]  Enables the use of a proxy server and
                                optionally sets the address and port.
        -d                      Disables the use of a proxy server.

