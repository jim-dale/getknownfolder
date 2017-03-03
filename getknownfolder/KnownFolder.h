#pragma once

struct KnownFolder
{
    const PTSTR     m_name;
    GUID            m_fid;
    std::wstring    m_path;
    std::wstring    m_displayPath;
};
