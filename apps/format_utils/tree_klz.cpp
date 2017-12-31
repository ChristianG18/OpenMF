#include <iostream>
#include <klz/parser.hpp>
#include <loggers/console.hpp>
#include <cxxopts.hpp>
#include <vector>

#define KLZ_MODULE_STR "KLZ util"

void dump(MFFormat::DataFormatTreeKLZ &klz)
{
    std::vector<MFFormat::DataFormatTreeKLZ::Link> links = klz.getLinks();

    std::cout << "LINKS (" << links.size() << "):" << std::endl;

    std::vector<std::string> linkStrings;

    for (auto i = 0; i < links.size(); ++i)
    {
        char buffer[255];
        memcpy(buffer,links[i].mName,255);
        buffer[links[i].mNameLength] = 0;

        linkStrings.push_back(buffer);

        std::cout << "  " << buffer << std::endl;
    }

    std::cout << std::endl;
    std::cout << "GRID (" << klz.getGridWidth() << " x " << klz.getGridHeight() << "):" << std::endl;

    for (auto y = 0; y < klz.getGridHeight(); ++y)
        for (auto x = 0; x < klz.getGridWidth(); ++x)
        {
            MFFormat::DataFormatTreeKLZ::Cell cell = klz.getGridCell(x,y);

            if (cell.mNumObjects > 0)
                std::cout << "  [" << x << "," << y << "]\tobjects: " << cell.mNumObjects << std::endl;
        }

    std::cout << std::endl;

    #define dumpItems(getFunc,printCmd) \
    {\
        auto items = getFunc;\
        for (auto i = 0; i < items.size(); ++i)\
        {\
            auto item = items[i];\
            printCmd;\
        }\
    }

    std::cout << "COLLISIONS:" << std::endl;
    dumpItems(klz.getFaceCols(),std::cout << "  face" << std::endl);
    dumpItems(klz.getAABBCols(),std::cout << "  AABB\t\tp1: " << item.mMin.str() << "\t\tp2: " << item.mMax.str() << "\t\tlink: " << item.mLink << " (" << linkStrings[item.mLink] << ")" << std::endl);
}

int main(int argc, char** argv)
{
    cxxopts::Options options(KLZ_MODULE_STR,"CLI utility for Mafia TREE_KLZ format.");

    options.add_options()
        ("h,help","Display help and exit.")
        ("i,input","Specify input file name.",cxxopts::value<std::string>());

    options.parse_positional({"i"});
    options.positional_help("file");
    auto arguments = options.parse(argc,argv);

    if (arguments.count("h") > 0)
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    bool verbose = arguments.count("v") > 0;

    if (arguments.count("i") < 1)
    {
        MFLogger::ConsoleLogger::fatal("Expected file.", KLZ_MODULE_STR);
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string inputFile = arguments["i"].as<std::string>();

    std::ifstream f;

    f.open(inputFile, std::ios::binary);

    if (!f.is_open())
    {
        MFLogger::ConsoleLogger::fatal("Could not open file " + inputFile + ".", KLZ_MODULE_STR);
        return 1;
    }

    MFFormat::DataFormatTreeKLZ klz;

    klz.load(f);

    f.close();

    dump(klz);

    return 0;
}
