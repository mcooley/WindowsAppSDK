#include <fstream>
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

    if (argc < 3)
    {
        std::wcout << L"Usage: MergePri.exe [ListOfInputFiles.txt] [output.pri]" << std::endl;
        return -1;
    }
    auto inputFileList = argv[1];
    auto outputPriFile = argv[2];

    wifstream inputFile;
    inputFile.open(inputFileList);

    while (!inputFile.eof())
    {
        std::wstring inputPriFile;
        std::getline(inputFile, inputPriFile);
        if (!inputPriFile.empty())
        {
            std::wcout << L"Adding file " << inputPriFile << std::endl;
            THROW_IF_FAILED(spResourcePackMerge->AddPriFile(
                inputPriFile.c_str(),
                PriFileMerger::DropDuplicateCandidates | PriFileMerger::InPlaceMerge | PriFileMerger::DefaultPriMergeFlags));
        }
    }

    std::wcout << L"Writing merged file to " << outputPriFile << std::endl;
    THROW_IF_FAILED(spResourcePackMerge->WriteToFile(outputPriFile));
}
