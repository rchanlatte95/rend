#include "rac-io.h"

namespace rac::io
{
    std::filesystem::path GetDesktopPath()
    {
        mut_wstr p;
        if (S_OK != SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &p)) return "";
        std::filesystem::path result = p;
        CoTaskMemFree(p);
        return result;
    }

    std::string GetDesktopPathStr() { return GetDesktopPath().string(); }
}
