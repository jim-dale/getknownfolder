// getknownfolder.cpp : 
//

#define _WIN32_WINNT _WIN32_WINNT_WIN6

#include <SDKDDKVer.h>
#include <windows.h>
#include <ShlObj.h>

#include <limits.h>
#include <stdio.h>
#include <tchar.h>

#include <string>

#include "KnownFolder.h"
#include "KnownFolderDefinitions.h"


std::wstring GetFolderPath(const KNOWNFOLDERID& rfid)
{
    std::wstring result;

    PWSTR pszPath = NULL;
    if (S_OK == SHGetKnownFolderPath(rfid, KF_FLAG_DEFAULT, NULL, &pszPath))
    {
        result = pszPath;
        CoTaskMemFree(pszPath);
    }
    return result;
}

void InitailiseKnownFoldersArray()
{
    for (size_t i = 0; i < NumberOfKnownFolders; i++)
    {
        KnownFolder* item = &(KnownFolders[i]);

        std::wstring path = GetFolderPath(item->m_fid);
        item->m_path = path;
        item->m_displayPath = (path.empty()) ? _T("(null)") : path;
    }
}

void ShowKnownFolders()
{
    for (size_t i = 0; i < NumberOfKnownFolders; i++)
    {
        KnownFolder const*const item = &(KnownFolders[i]);

        _tprintf(_T("%s=\"%s\"\r\n"), item->m_name, item->m_displayPath.c_str());
    }
}

KnownFolder* FindKnownFolder(PTSTR name)
{
    KnownFolder* result = NULL;

    for (size_t i = 0; i < NumberOfKnownFolders; i++)
    {
        KnownFolder* item = &(KnownFolders[i]);

        if (_tcsicmp(item->m_name, name) == 0)
        {
            if (false == item->m_path.empty())
            {
                result = item;
            }
        }
    }
    return result;
}

int wmain(int argc, PTSTR argv[])
{
    bool listKnownFolders = (1 == argc);

    InitailiseKnownFoldersArray();

    if (listKnownFolders)
    {
        ShowKnownFolders();
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            PTSTR arg = argv[i];

            KnownFolder const*const item = FindKnownFolder(arg);
            if (item != NULL && item->m_path.empty() == false)
            {
                _putts(item->m_path.c_str());
            }
        }
    }
    return 0;
}

