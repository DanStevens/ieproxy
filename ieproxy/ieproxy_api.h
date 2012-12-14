#pragma once
#include "stdafx.h"
#include "ProxySettings.h"

namespace ieproxy { namespace api
{

#define BYPASS_LOCAL_STR "<local>"

char* StringToCharPtr(std::string& s);
bool LoadFromSystem(ProxySettings& p);
bool SaveToSystem(const ProxySettings& p);

} }