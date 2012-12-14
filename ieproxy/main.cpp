#include "stdafx.h"
#include "ieproxy_api.h"
#include "ProxySettings.h"
#include "CommandLineUI.h"

int main(int argc, char* argv[])
{	
	ieproxy::CommandLineUI cl(std::cout, std::cerr);
	return cl.Run(argc, argv);
}

