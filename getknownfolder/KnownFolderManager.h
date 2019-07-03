#pragma once

typedef std::vector<KnownFolder> KnownFolderCollection;

class KnownFolderManager
{
public:
    KnownFolderManager() {}
    ~KnownFolderManager() {}

    KnownFolderCollection GetAll()
    {
        KnownFolderCollection result;

        CComPtr<IKnownFolderManager> kfm;
        HRESULT hr = CoCreateInstance(CLSID_KnownFolderManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&kfm));
        if (SUCCEEDED(hr))
        {
            CComHeapPtr<KNOWNFOLDERID> kfids;
            UINT ckfids = 0;

            hr = kfm->GetFolderIds(&kfids, &ckfids);
            if (SUCCEEDED(hr))
            {
                for (UINT i = 0; i < ckfids; ++i)
                {
                    CComPtr<IKnownFolder> kf;

                    KNOWNFOLDERID kfid = kfids[i];
                    hr = kfm->GetFolder(kfid, &kf);
                    if (SUCCEEDED(hr))
                    {
                        KNOWNFOLDER_DEFINITION kfd;

                        hr = kf->GetFolderDefinition(&kfd);
                        if (SUCCEEDED(hr))
                        {
                            std::wstring path = GetPath(kf);
                            if (path.empty() == false)
                            {
                                KnownFolder item
                                {
                                    kfid,
                                    kfd.pszName,
                                    GetDisplayName(kf, item.canonicalName),
                                    path
                                };

                                result.push_back(item);
                            }
                            FreeKnownFolderDefinitionFields(&kfd);
                        }
                    }
                }
            }
        }
        return result;
    }

    const KnownFolder* FindByName(const KnownFolderCollection& items, PCWSTR name, SearchBy searchBy)
    {
        const KnownFolder* result = nullptr;

        switch (searchBy)
        {
        case SearchBy::Any:
            result = FindByName(items, name);
            break;
        case SearchBy::CanonicalName:
            result = FindByCanonicalName(items, name);
            break;
        case SearchBy::DisplayName:
            result = FindByDisplayName(items, name);
            break;
        default:
            break;
        }
        return result;
    }

    const KnownFolder* FindByName(const KnownFolderCollection& items, PCWSTR name)
    {
        const KnownFolder* result = FindByDisplayName(items, name);
        if (result == nullptr)
        {
            result = FindByCanonicalName(items, name);
        }
        return result;
    }

    const KnownFolder* FindByCanonicalName(const KnownFolderCollection& items, PCWSTR name)
    {
        const KnownFolder* result = nullptr;

        for (const auto& item : items)
        {
            if (_wcsicmp(name, item.canonicalName.c_str()) == 0)
            {
                if (item.path.empty()== false)
                {
                    result = &item;
                }
            }
        }
        return result;
    }

    const KnownFolder* FindByDisplayName(const KnownFolderCollection& items, PCWSTR name)
    {
        const KnownFolder* result = nullptr;

        for (const auto& item : items)
        {
            if (_wcsicmp(name, item.displayName.c_str()) == 0)
            {
                if (item.path.empty() == false)
                {
                    result = &item;
                }
            }
        }
        return result;
    }

private:
    std::wstring GetPath(IKnownFolder* kf)
    {
        std::wstring result;
        PWSTR pszPath = nullptr;

        HRESULT hr = kf->GetPath(0, &pszPath);
        if (SUCCEEDED(hr) && pszPath != nullptr)
        {
            result = pszPath;

            CoTaskMemFree(pszPath);
        }
        return result;
    }

    std::wstring GetDisplayName(IKnownFolder* kf, const std::wstring& defaultValue)
    {
        std::wstring result;
        CComPtr<IShellItem> sh;

        HRESULT hr = kf->GetShellItem(0, IID_PPV_ARGS(&sh));
        if (SUCCEEDED(hr))
        {
            PWSTR pszName = nullptr;

            hr = sh->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
            if (SUCCEEDED(hr) && pszName != nullptr)
            {
                result = pszName;

                CoTaskMemFree(pszName);
            }
        }
        if (FAILED(hr))
        {
            result = defaultValue;
        }
        return result;
    }
};
