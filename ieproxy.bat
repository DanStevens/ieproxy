@echo off
REM | ieproxy.bat
REM | A Windows batch script for outputting or modifying proxy settings for Internet Explorer

setlocal EnableExtensions
setlocal EnableDelayedExpansion

REM | Configuration variables
SET ieproxy=%0
SET usage=Type "%ieproxy% /?" for usage.
SET keyname="HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings"
SET autocfg_name=AutoConfigURL
SET proxy_enable=ProxyEnable
SET proxy_server=ProxyServer
SET proxy_override=ProxyOverride

REM | Main program starts here
ECHO.
IF "%1" == "/?" (
    CALL :help
) ELSE IF "%1" == "autocfg" (
    CALL :op_autocfg %2 %3
) ELSE IF "%1" == "proxy" (
    CALL :op_proxy %2 %3
) ELSE IF [%1] == [] (
    ECHO %usage%
    ECHO.
    CALL :op_autocfg
    ECHO.
    CALL :op_proxy
) ELSE (
    CALL :error %1
)

GOTO:EOF

:help
REM | Display general help
    ECHO Output or modify proxy settings for Internet Explorer
    ECHO.
    ECHO %ieproxy% ^<operation^> [^<arguments^>]
    ECHO.
    ECHO The following operations are supported:
    ECHO.
    ECHO   autocfg [-e ^<url^> ^| -d]
    ECHO.
    ECHO     Display or change automatic proxy configuration script settings. If 
    ECHO     none of the following arguments are supplied, the status and
    ECHO     location of the automatic proxy configuration script are displayed.
    ECHO.
    ECHO     -e ^<url^>  Enable automatic proxy configuration using the script 
    ECHO               at the location given by ^<url^>.
    ECHO     -d        Disable automatic proxy configuration
    ECHO.
    ECHO   proxy [-s ^<server^>[:^<port^>] ^| -e [^<server^>[:^<port^>]] ^| -d]
    ECHO.
    ECHO     Display or configure proxy settings manually. If none of the 
    ECHO     following arguments are supplied, the proxy settings are displayed.
    ECHO.
    ECHO     -s ^<server^>[:^<port^>]    Sets the address and port of the proxy
    ECHO                             server.
    ECHO     -e [^<server^>[:^<port^>]]  Enables the use of a proxy server and 
    ECHO                             optionally sets the address and port.
    ECHO     -d                      Disables the use of a proxy server.
GOTO:EOF

:error
REM | Display error message
REM | %1  The argument that caused the error
    IF [%1] == [] (
        ECHO Error: Invalid syntax.
    ) ELSE (
        ECHO Error: Invalid Argument/Operation - '%1'.
    )
    ECHO %usage%
    EXIT /B 1
GOTO:EOF

:reg_query
REM | Queries a windows registry for a single key value
REM | Arguments:
REM |   %1  Reference to a variable, which is set to the value of the registry
REM |       entry
REM |   %2  The key name
REM |   %3  The value name
    setlocal
	FOR /F "tokens=1-3" %%A IN ('REG QUERY %2 /v %3 2^>nul') DO (
		set value=%%C
	)
	endlocal&set "%~1=%value%"
GOTO:EOF

:op_autocfg
REM | Function for the 'autocfg' operation
REM | Arguments:
REM |   %1  "-e" to enable automatic proxy configuration, or "-d" to disable it.
REM |       Displays status and URL if not set
REM |   %2  The URL of the script to use for proxy configuration.
    setlocal
    IF [%1] == [] (
        CALL :reg_query autocfg_url %keyname% %autocfg_name%

        IF DEFINED autocfg_url (
            CALL :display_autocfg Enabled !autocfg_url!
        ) else (
            CALL :display_autocfg Disabled !autocfg_url!
        )
    ) ELSE IF "%1" == "-e" (
        CALL :enable_autocfg %2
    ) ELSE IF "%1" == "-d" (
        CALL :disable_autocfg
    ) ELSE (
        CALL :error %1
    )
    endlocal
GOTO:EOF

REM :get_autocfg_url
REM | Retrieves the value registry key %keyname%\%autocfg_name% and stores it in
REM | the variable named %autocfg_url%
REM     FOR /F "tokens=1-3" %%A IN ('REG QUERY %keyname% /v %autocfg_name% 2^>nul') DO (
REM         set autocfg_url=%%C
REM     )
REM GOTO:EOF

:display_autocfg
REM | Formats automatic proxy configuration settings, which are given as arguments.
REM | Arguments:
REM |   %1  The status for automatic proxy configuration. It is recommended
REM |       that this is 'Enabled' to indicate that an automatic configuration
REM |       script is in use, otherwise 'Disabled'
REM |   %2  The URL of the script to use for proxy configuration. It is 
REM |       recommended that this is omitted if not in use.
    ECHO Automatic configuration settings:
    ECHO   Status:      %1
    ECHO   Script URL:  %2
GOTO:EOF

:enable_autocfg
REM | Enable automatic configuration.
REM | Arguments:
REM |   %1  The URL of the script
    IF [%1] == [] (
        CALL :error
    ) ELSE (
        REG ADD %keyname% /v %autocfg_name% /t REG_SZ /d %~1 /f >nul 2>&1
    )
    ECHO Automatic configuration script set to '%~1'
GOTO:EOF

:disable_autocfg
REM | Disable automatic configuration.
    REG DELETE %keyname% /v %autocfg_name% /f >nul 2>&1
    if errorlevel 1 (
        ECHO Automatic proxy configuration is already disabled
    ) ELSE (
        ECHO Automatic proxy configuration disabled
    )
GOTO:EOF

:op_proxy
REM | Function for the 'proxy' operation
REM | Arguments:
REM |   %1  "-s" to set the proxy server location, "-e" to enable the use of the
REM |       proxy server or, or "-d" to disable the use of the proxy server.
REM |       Displays proxy settings if not set.
REM |   %2  The address of the proxy server to use
    setlocal
    IF [%1] == [] (
        CALL :reg_query status_code %keyname% %proxy_enable%
        CALL :reg_query server %keyname% %proxy_server%
        CALL :reg_query override %keyname% %proxy_override%
        
        IF "!status_code!" == "0x1" (
            SET status=Enabled
        ) ELSE (
            SET status=Disabled
        )

        CALL :display_proxy "!status!" "!server!" "!override!"
    ) ELSE IF "%1" == "-e" (
        CALL :set_proxy 1 %2
    ) ELSE IF "%1" == "-s" (
        IF [%2] == [] (
            ECHO Error: Server address/port expected.
            ECHO %usage%
            EXIT /B 1
        ) ELSE (
            CALL :set_proxy - %2
        )
    ) ELSE IF "%1" == "-d" (
        CALL :set_proxy 0
    ) ELSE (
        CALL :error %1
    )
    endlocal
GOTO:EOF

:display_proxy
REM | Formats manual proxy settings, which are given as arguments.
REM | Arguments:
REM |   %1  The status for manual proxy configuration. It is recommended
REM |       that this is 'Enabled' to indicate that a proxy server is in use,
REM |       otherwise 'Disabled'
REM |   %2  The address of the proxy server.
REM |   %3  A list of domains for which the proxy server is not used, seperated
REM |       by semicolons.
    setlocal
    set override=%3
    ECHO Proxy settings:
    ECHO   Status:        %~1
    ECHO   Server:        %~2
    ECHO   No proxy for:  !override:"=!
    endlocal
GOTO:EOF

:set_proxy
REM | Modifies manual proxy settings.
REM | Arguments:
REM |   %1  '1' to enabled the use of a proxy server and '0' to disable the
REM |       use of a proxy serve. Any other value leaves the status unchanged.
REM |   %2  The address and port of the proxy server. Omitt to leave the current
REM |       setting unchanged.
    IF "%1" == "1" (
        REG ADD %keyname% /v %proxy_enable% /t REG_DWORD /d 0x1 /f >nul 2>&1
        ECHO Proxy server enabled
    )
    IF "%1" == "0" (
        REG ADD %keyname% /v %proxy_enable% /t REG_DWORD /d 0x0 /f >nul 2>&1
        ECHO Proxy server disabled
    )
    
    IF [%2] NEQ [] (
        REG ADD %keyname% /v %proxy_server% /t REG_SZ /d %~2 /f >nul 2>&1
        ECHO Proxy server address set to '%~2'
    )

GOTO:EOF