#include <iostream>

#include "mrm/BaseInternal.h"
#include "mrm/build/Base.h"
#include "mrm/readers/MrmReaders.h"
#include "mrm/readers/MrmManagers.h"
#include "mrm/build/MrmBuilders.h"

using namespace Microsoft::Resources;
using namespace Microsoft::Resources::Build;

#include "mrm/build/ResourcePackMerge.h"

int wmain(int argc, wchar_t* argv[])
{
    AutoDeletePtr<CoreProfile> pProfile;
    THROW_IF_FAILED(CoreProfile::ChooseDefaultProfile(&pProfile));
    AutoDeletePtr<UnifiedResourceView> spUnifiedResourceView;
    THROW_IF_FAILED(UnifiedResourceView::CreateInstance(pProfile, &spUnifiedResourceView));
    AutoDeletePtr<ResourcePackMerge> spResourcePackMerge;
    THROW_IF_FAILED(ResourcePackMerge::CreateInstance(pProfile, &spResourcePackMerge));

    if (argc < 2)
    {
        std::wcout << L"Usage: MergePri.exe [input1.pri input2.pri ...] [output.pri]" << std::endl;
        return -1;
    }

    for (int i = 1; i < argc - 1; i++)
    {
        std::wcout << L"Adding file " << argv[i] << std::endl;
        THROW_IF_FAILED(spResourcePackMerge->AddPriFile(
            argv[i], PriFileMerger::InPlaceMerge | PriFileMerger::DefaultPriMergeFlags));
    }

    std::wcout << L"Writing merged file to " << argv[argc - 1] << std::endl;
    THROW_IF_FAILED(spResourcePackMerge->WriteToFile(argv[argc - 1]));
}
