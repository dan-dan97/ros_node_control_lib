#pragma once

#include <ros_node_info.hpp>
#include <pthread.h>
#include <string.h>

class ROSNodeControl{

public:
    ROSNodeControl(ROSNodeInfo rosNodeInfoArg);
    ROSNodeControl(ROSNodeInfo::NodeType nodeType, std::string packageName, std::string executingFileName);
    ~ROSNodeControl();

    void run();
    void kill();
    void waitKilling();
    void killChild(ROSNodeInfo rosNodeInfoArg);

    static ROSNodeControl& ROSmaster();

private:
    ROSNodeInfo rosNodeInfo;
    bool nodeRunning;
    pthread_t thread;
    std::string runCommand;

    static void* threadFunction(void* arg);
    static std::string getROSNodePathCommand(ROSNodeInfo rosNodeInfo);
};
