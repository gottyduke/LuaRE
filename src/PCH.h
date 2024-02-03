#pragma once

// clib
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REL/Relocation.h>

// lua
#define SOL_ALL_SAFETIES_ON 1
#include <sol/forward.hpp>

// winnt
#include <ShlObj_core.h>

using namespace std::literals;
using namespace REL::literals;

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)

// Plugin
#include "Plugin.h"

// DKUtil
#include "DKUtil/Logger.hpp"
#include "DKUtil/Utility.hpp"
