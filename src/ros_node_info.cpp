#include <ros_node_info.hpp>

ROSNodeInfo::ROSNodeInfo(NodeType nodeTypeArg, std::string packageNameArg, std::string executingFileNameArg)
: nodeType(nodeTypeArg), packageName(packageNameArg), executingFileName(executingFileNameArg)
{
}

ROSNodeInfo::~ROSNodeInfo(){
    packageName.clear();
    executingFileName.clear();
}

ROSNodeInfo::NodeType ROSNodeInfo::getNodeType(){
    return nodeType;
}

std::string ROSNodeInfo::getPackageName(){
    return  packageName;
}

std::string ROSNodeInfo::getExecutingFileName(){
    return executingFileName;
}

bool ROSNodeInfo::operator==(ROSNodeInfo &rosNodeInfoArg){
    return rosNodeInfoArg.getNodeType() == this->getNodeType() && rosNodeInfoArg.getPackageName() == this->getPackageName() && rosNodeInfoArg.getExecutingFileName() == this->getExecutingFileName();
}