
#define _WIN32_WINNT _WIN32_WINNT_WIN6

#include <SDKDDKVer.h>
#include <windows.h>
#include <ShlObj.h>

#include <atlbase.h>
#include <atlcom.h>

#include <limits.h>
#include <stdio.h>
#include <tchar.h>

#include <string>
#include <vector>

#include "AppMetadata.h"
#include "KnownFolder.h"
#include "SearchBy.h"
#include "KnownFolderManager.h"

void ShowHelp()
{
    wprintf(L"Displays the path for the given known folder name.\n\n");
    wprintf(L"Usage: %s [-?] [-v] [-a|c|d] [Name] [...]\n\n", ProgramName);

    wprintf(L"  -?  Display this help information.\n");
    wprintf(L"  -v  Display version information.\n");
    wprintf(L"  -a  [Default] Searches for the name by display name. If the name does not match a display name searches by canonical name.\n");
    wprintf(L"  -c  Searches for the name by canonical name.\n");
    wprintf(L"  -d  Searches for the name by display name.\n\n");
    wprintf(L"If no parameters are supplied a list of known folders will be displayed in the format:-\n");
    wprintf(L"Display Name [Canonical Name]=\"Path\"\n");
}

void ShowVersion()
{
    wprintf(L"%s %s-%s\n", ProgramName, ProgramVersion, ProgramConfig);
}

struct AppCommand
{
    SearchBy        m_searchBy;
    std::wstring    m_name;
};

struct AppContext
{
    bool                    m_showHelp;
    bool                    m_showVersion;
    bool                    m_listKnownFolders;
    std::vector<AppCommand> m_commands;
};

AppContext ParseCommandLine(int argc, PWSTR argv[])
{
    AppContext result {};

    result.m_listKnownFolders = (1 == argc);

    SearchBy searchBy = SearchBy::Any;
    for (int i = 1; i < argc; i++)
    {
        PCWSTR arg = argv[i];
        size_t argLength = wcslen(arg);

        if (arg[0] == '-' && argLength > 1)
        {
            WCHAR option = towlower(arg[1]);

            switch (option)
            {
            case L'a':
                searchBy = SearchBy::Any;
                break;
            case L'c':
                searchBy = SearchBy::CanonicalName;
                break;
            case L'd':
                searchBy = SearchBy::DisplayName;
                break;
            case L'?':
                result.m_showHelp = true;
                break;
            case L'v':
                result.m_showVersion = true;
                break;
            default:
                break;
            }
        }
        else
        {
            AppCommand command;

            command.m_searchBy = searchBy;
            command.m_name = arg;

            result.m_commands.push_back(command);
        }
    }
    return result;
}

void ShowKnownFolder(const KnownFolder& kf)
{
    wprintf(L"%s [%s]=\"%s\"\n", kf.m_displayName.c_str(), kf.m_canonicalName.c_str(), kf.m_path.c_str());
}

void ShowKnownFolders(const KnownFolderCollection& items)
{
    for (const auto& item : items)
    {
        ShowKnownFolder(item);
    }
}

int wmain(int argc, PWSTR argv[])
{
    int result = 0;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        AppContext appContext = ParseCommandLine(argc, argv);

        KnownFolderManager manager;
        KnownFolderCollection items = manager.GetAll();

        if (appContext.m_showHelp)
        {
            ShowHelp();
        }
        else if (appContext.m_showVersion)
        {
            ShowVersion();
        }
        else if (appContext.m_listKnownFolders)
        {
            ShowKnownFolders(items);
        }
        else
        {
            for (const auto& command : appContext.m_commands)
            {
                const KnownFolder* item = manager.FindByName(items, command.m_name.c_str(), command.m_searchBy);

                if (item != nullptr && item->m_path.empty() == false)
                {
                    _putts(item->m_path.c_str());
                }
                else
                {
                    result = 1;
                }
            }
        }
        CoUninitialize();
    }
    else
    {
        result = 255;
    }
    return result;
}
