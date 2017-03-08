#pragma once

struct KnownFolder
{
    KNOWNFOLDERID   m_id;
    std::wstring    m_canonicalName;
    std::wstring    m_displayName;
    std::wstring    m_path;
};
