// Copyright © Dan Stevens 2012
// 
// Distributed under the terms of the Boost Software License, Version 1.0.
// See accompanying file *LICENSE.txt*.
//
// Dependancies:
//   - string.h
//   - windows.h
//   - wininet.h
//
#pragma once
#include "stdafx.h"
#include "ProxySettings.h"

namespace ieproxy { namespace api
{

    #define BYPASS_LOCAL_STR "<local>"

    /**
     * Allocates character array using the contents of the given string object.
     * \param s The std::string object from which to copy to the newly newly allocated character array
     * \return A pointer to the newly allocated character array.
     */
    char* StringToCharPtr(std::string& s);

    /**
     * Populates the given ProxySettings object with values retrieved via tine WinINET API.
     * \param p The ProxySettings object to populate.
     * \return true if the WinINET API reports that the proxy settings were retrieved successfully or false if there 
     *              was an error.
     */
    bool LoadFromSystem(ProxySettings& p);

    /**
     * Updates the proxy settings for Internet Explorer using values contained within the given ProxySettings object.
     * \param The ProxySettings object to retrieve proxy settings from.
     * \return true if the WinINET API reports that the proxy settings were updated successfully or false if there was
     *              an error.
     */
    bool SaveToSystem(const ProxySettings& p);

} }