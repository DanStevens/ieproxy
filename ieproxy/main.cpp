// Copyright © Dan Stevens 2012
// 
// Distributed under the terms of the Boost Software License, Version 1.0.
// See accompanying file *LICENSE.txt*.
//
#include "stdafx.h"
#include "CommandLineUI.h"

int main(int argc, char* argv[])
{
    ieproxy::CommandLineUI cl(std::cout, std::cerr);
    return cl.Run(argc, argv);
}

