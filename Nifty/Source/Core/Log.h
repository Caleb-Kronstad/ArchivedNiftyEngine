#pragma once

#include "nfpch.h"

namespace Nifty
{
    class Log
    {
    public:
        static void Info(const std::string& msg)
        {
            std::cout << "\x1b[37m " << "[INFO] " << msg << "\x1b[37m\n";
        }

        static void Warning(const std::string& msg)
        {
            std::cout << "\x1b[33m " << "[WARNING] " << msg << "\x1b[37m\n";
        }

        static void Error(const std::string& msg)
        {
            std::cout << "\x1b[31m " << "[ERROR] " << msg << "\x1b[37m\n";
        }
    };
}