#include <iostream>
#include <scene2_bin/parser.hpp>
#include <utils/logger.hpp>
#include <vfs/vfs.hpp>
#include <utils/openmf.hpp>
#include <cxxopts.hpp>

using namespace MFLogger;

typedef struct
{
    uint32_t mType;
    const char *mName;
} TypeName;

TypeName const gObjectTypeNames[] = {
    {0x02, "OBJECT_TYPE_LIGHT"},
    {0x03, "OBJECT_TYPE_CAMERA"},
    {0x04, "OBJECT_TYPE_SOUND"},
    {0x09, "OBJECT_TYPE_MODEL"},
    {0x0C, "OBJECT_TYPE_OCCLUDER"},
    {0x99, "OBJECT_TYPE_SECTOR"},
    {0x9B, "OBJECT_TYPE_SCRIPT"},
};

TypeName const gLightTypeNames[] = {
    {0x01, "LIGHT_TYPE_POINT"},
    {0x03, "LIGHT_TYPE_DIRECTIONAL"},
    {0x04, "LIGHT_TYPE_AMBIENT"},
    {0x05, "LIGHT_TYPE_FOG"},
    {0x06, "LIGHT_TYPE_POINT_AMBIENT"},
    {0x08, "LIGHT_TYPE_LAYERED_FOG"},
};

char const *getTypeName(size_t count, TypeName const *typeNames, uint32_t objType, char const *defaultType = "TYPE_UNKNOWN")
{
    for (size_t i = 0; i < count; i++)
    {
        if (typeNames[i].mType == objType)
            return typeNames[i].mName;
    }

    return defaultType;
}

void dump(MFFormat::DataFormatScene2BIN scene2Bin, uint32_t objType)
{
    MFLogger::Logger::raw("view distance: " + std::to_string(scene2Bin.getViewDistance()) + ",", "dump");
    MFLogger::Logger::raw("field of view: " + std::to_string(scene2Bin.getFov()) + ",", "dump");
    MFLogger::Logger::raw("clipping planes: [" + scene2Bin.getClippingPlanes().str() + "],", "dump");
    MFLogger::Logger::raw("number of objects: " + std::to_string(scene2Bin.getNumObjects()) + ",", "dump");
    MFLogger::Logger::raw("", "dump");

    for (auto pair : scene2Bin.getObjects())
    {
        auto object = pair.second;

        if (object.mType != objType && objType != 0) continue;

        MFLogger::Logger::raw("\tobject name: " + object.mName + " {", "dump");
        MFLogger::Logger::raw("\t\ttype: " + std::string(getTypeName(7, gObjectTypeNames, object.mType)) + "(" + std::to_string(object.mType) + "),", "dump");
        MFLogger::Logger::raw("\t\tposition: [" + object.mPos.str() + "],", "dump");
        MFLogger::Logger::raw("\t\tposition2: [" + object.mPos2.str() + "],", "dump");
        MFLogger::Logger::raw("\t\trotation: [" + object.mRot.str() + "],", "dump");
        MFLogger::Logger::raw("\t\tscale: [" + object.mScale.str() + "],", "dump");
        MFLogger::Logger::raw("\t\tparent name: " + object.mParentName + ",", "dump");

        if (object.mType == MFFormat::DataFormatScene2BIN::OBJECT_TYPE_MODEL)
            MFLogger::Logger::raw("\t\tmodel name: " + object.mModelName + ",", "dump");
        
        if (object.mType == MFFormat::DataFormatScene2BIN::OBJECT_TYPE_LIGHT)
        {
            MFLogger::Logger::raw("\t\tlight properties {", "dump");
            MFLogger::Logger::raw("\t\t\tlight type: " + std::string(getTypeName(5, gLightTypeNames, object.mLightType)) + "(" + std::to_string(object.mLightType) + "),", "dump");
            MFLogger::Logger::raw("\t\t\tlight colour: [" + object.mLightColour.str() + "],", "dump");
            MFLogger::Logger::raw("\t\t\tlight power: " + std::to_string(object.mLightPower), "dump");
            MFLogger::Logger::raw("\t\t\tlight unk0: " + std::to_string(object.mLightUnk0) + ",", "dump");
            MFLogger::Logger::raw("\t\t\tlight unk1: " + std::to_string(object.mLightUnk1) + ",", "dump");
            MFLogger::Logger::raw("\t\t\tlight near: " + std::to_string(object.mLightNear) + ",", "dump");
            MFLogger::Logger::raw("\t\t\tlight far: " + std::to_string(object.mLightFar) + ",", "dump");
            MFLogger::Logger::raw("\t\t}", "dump");
        }
        MFLogger::Logger::raw("\t}", "dump");
    }
}

int main(int argc, char** argv)
{
    cxxopts::Options options("scene2_bin","CLI utility for Mafia scene2.bin format.");

    options.add_options()
        ("h,help","Display help and exit.")
        ("i,input","Specify input file name.",cxxopts::value<std::string>())
        ("t,type","Specify object type.",cxxopts::value<int>());

    options.parse_positional({"i","t"});
    options.positional_help("file [type]");
    auto arguments = options.parse(argc,argv);

    if (arguments.count("h") > 0)
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (arguments.count("i") < 1)
    {
        MFLogger::Logger::fatal("Expected file.", "dump");
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string inputFile = arguments["i"].as<std::string>();

    std::ifstream f;
    f.open(inputFile, std::ios::binary);

    if (!f.is_open())
    {
        MFLogger::Logger::fatal("Could not open file " + inputFile + ".", "dump");
        return 1;
    }

    MFFormat::DataFormatScene2BIN scene2Bin;

    bool success = scene2Bin.load(f);

    if (!success)
    {
        MFLogger::Logger::fatal("Could not parse file " + inputFile + ".", "dump");
        return 1;
    }

    uint32_t objType = 0;

    if (arguments.count("t") >= 1)
        objType = arguments["t"].as<int>();

    dump(scene2Bin, objType);

    return 0;
}
