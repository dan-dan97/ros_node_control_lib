#pragma once

#include <string>

class ROSNodeInfo{

public:
    enum class NodeType { NODE, LAUNCH };

    ROSNodeInfo(NodeType nodeTypeArg, std::string packageNameArg, std::string executingFileNameArg);
    ~ROSNodeInfo();

    NodeType getNodeType();
    std::string getPackageName();
    std::string getExecutingFileName();

    bool operator==(ROSNodeInfo &rosNodeInfoArg);

private:
    NodeType nodeType;
    std::string packageName, executingFileName;
};