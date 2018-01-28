#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <ros_node_control.hpp>
#include <keyboard_lib.hpp>

int main() {

    Keyboard keyboard;

    ROSNodeControl::ROSmaster.run();
    ROSNodeControl node(ROSNodeInfo::NodeType::LAUNCH, "usb_cam", "usb_cam-test.launch");

    while(!keyboard.keyPush(KEY_ESC)){

        if(keyboard.keyPush(KEY_F6)){
            std::cout << "Start thread usb_cam.launch" << std::endl;
            node.run();
            std::cout << "Thread started" << std::endl << std::endl;
        }
        else if(keyboard.keyPush(KEY_F7)){
            std::cout << "Kill process usb_cam.launch" << std::endl;
            node.kill();
            std::cout << "Process killed" << std::endl;
            node.waitKilling();
            std::cout << "Thread joined" << std::endl;
        }
        else if(keyboard.keyPush(KEY_F8)){
            std::cout << "Kill process usb_cam" << std::endl;
            node.killChild(ROSNodeInfo(ROSNodeInfo::NodeType::NODE, "usb_cam", "usb_cam_node"));
            std::cout << "Process killed" << std::endl;
        }
        else if(keyboard.keyPush(KEY_F9)){
            std::cout << "Kill process image_view" << std::endl;
            node.killChild(ROSNodeInfo(ROSNodeInfo::NodeType::NODE, "image_view", "image_view"));
            std::cout << "Process killed" << std::endl;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
    }

    ROSNodeControl::ROSmaster.kill();
    ROSNodeControl::ROSmaster.waitKilling();
    return 0;
}
