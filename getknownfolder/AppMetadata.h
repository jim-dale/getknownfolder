#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define VERSION(x) "2.0.3-" STRINGIFY(x)

static const PTCHAR ProgramName = _T("getknownfolder");
static const PTCHAR ProgramVersion = _T(VERSION(BUILDNUM));
static const PTCHAR SourceVersion = _T(STRINGIFY(BUILDSRCVER));

#if defined(_M_IX86)
static const PTCHAR ProgramConfig = _T("x86");
#elif defined(_M_X64)
static const PTCHAR ProgramConfig = _T("x64");
#else
static const PTCHAR ProgramConfig = _T("Undefined");
#endif
