#include <ros_node_control.hpp>
#include <cstdlib>

ROSNodeControl ROSNodeControl::ROSmaster(ROSNodeInfo::NodeType::NODE, "roscore", "roscore");

ROSNodeControl::ROSNodeControl(ROSNodeInfo rosNodeInfoArg)
: rosNodeInfo(rosNodeInfoArg), nodeRunning(0)
{
}

ROSNodeControl::ROSNodeControl(ROSNodeInfo::NodeType nodeType, std::string packageName, std::string executingFileName)
: rosNodeInfo(nodeType, packageName, executingFileName), nodeRunning(0)
{
}

ROSNodeControl::~ROSNodeControl(){
    if(nodeRunning){
        kill();
        waitKilling();
    }
}

void ROSNodeControl::run(){
    if(nodeRunning || !(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE || rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::LAUNCH)) return;

    runCommand = "";

    if(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE && rosNodeInfo.getPackageName() == "roscore" && rosNodeInfo.getExecutingFileName() == "roscore")
        runCommand += "roscore";
    else{
        runCommand += rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE ? "rosrun" : "roslaunch";
        runCommand += " ";
        if((!rosNodeInfo.getPackageName().empty() && rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::LAUNCH) || rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE){
            runCommand += rosNodeInfo.getPackageName();
            runCommand += " ";
        }
        runCommand += rosNodeInfo.getExecutingFileName();
    }

    nodeRunning = 1;
    pthread_create(&thread, NULL, this->threadFunction, (void*)this);
}

void ROSNodeControl::kill(){
    if(!nodeRunning || !(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE || rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::LAUNCH)) return;

    std::string killCommand = "kill $(ps -eo pid,command | grep \"";
    if(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE && rosNodeInfo.getPackageName() == "roscore" && rosNodeInfo.getExecutingFileName() == "roscore")
        killCommand += "/usr/bin/python /opt/ros/$ROS_DISTRO/bin/roscore";
    else{
        if(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::LAUNCH)
            killCommand += "/usr/bin/python /opt/ros/$ROS_DISTRO/bin/roslaunch ";
        killCommand += getROSNodePathCommand(rosNodeInfo);
    }

    killCommand += "\" | grep -v \"grep\" | cut -b 1-5 | tr -d \' \')";
    system(killCommand.c_str());
}

void ROSNodeControl::waitKilling(){
    pthread_join(thread, NULL);
}

void ROSNodeControl::killChild(ROSNodeInfo rosNodeInfoArg){
    if(!nodeRunning || rosNodeInfo.getNodeType() != ROSNodeInfo::NodeType::LAUNCH) return;
    if(rosNodeInfoArg == rosNodeInfo)
        kill();
    else{
        std::string killCommand = "kill $(ps -eo pid,ppid,command | grep \"^......$(ps -eo pid,command | grep \"/usr/bin/python /opt/ros/$ROS_DISTRO/bin/roslaunch ";
        killCommand += getROSNodePathCommand(rosNodeInfo);
        killCommand += "\" | grep -v \"grep\" | cut -b 1-5)\" | grep \"";
        if(rosNodeInfoArg.getNodeType() == ROSNodeInfo::NodeType::LAUNCH)
            killCommand += "/usr/bin/python /opt/ros/$ROS_DISTRO/bin/roslaunch ";
        killCommand += getROSNodePathCommand(rosNodeInfoArg);
        killCommand += "\" | cut -b 1-5 | tr -d \' \')";
        system(killCommand.c_str());
    }
}

void* ROSNodeControl::threadFunction(void* arg){
    ROSNodeControl &rosNodeControl = *(ROSNodeControl*)arg;
    system(rosNodeControl.runCommand.c_str());
    rosNodeControl.nodeRunning = 0;
    pthread_exit(NULL);
}

std::string ROSNodeControl::getROSNodePathCommand(ROSNodeInfo rosNodeInfo){
    std::string pathCommand = "";
    if(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::NODE) {
        pathCommand += "$(catkin_find --libexec ";
        pathCommand += rosNodeInfo.getPackageName();
        pathCommand += " ";
        pathCommand += rosNodeInfo.getExecutingFileName();
        pathCommand += ")";
    }
    else if(rosNodeInfo.getNodeType() == ROSNodeInfo::NodeType::LAUNCH){
        if(rosNodeInfo.getPackageName().empty()){
            pathCommand += rosNodeInfo.getExecutingFileName();
        }
        else{
            pathCommand += "$(catkin_find --share ";
            pathCommand += rosNodeInfo.getPackageName();
            pathCommand += " ";
            pathCommand += rosNodeInfo.getExecutingFileName();
            pathCommand += ")";
        }
    }
    return pathCommand;
}
