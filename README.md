# HelloCV
计算机视觉学习项目 - ROS2 &amp; OpenCV开发环境
一.项目概述：
本项目为计算机视觉学习环境，基于Ubuntu 22.04 LTS搭建完整的ROS2和OpenCV开发环境。项目记录了从零开始的环境搭建过程、技术学习笔记和实践经验总结。
二.环境要求
操作系统: Ubuntu 22.04 LTS
ROS2版本: Humble Hawksbill
OpenCV版本: 4.5.x
开发工具: VS Code, Vim, Git
三.项目结构
1.VMware虚拟机创建
2.Ubuntu22.04安装
3.安装ROS2
4.安装OpenCV4.5.x
5.配置VS Code开发环境
6.配置Git
四.环境配置步骤
1运行VMWARE,然后创建新的虚拟机，并且自定义（高级），在选择Linux后，根据所下镜像选择Ubuntu64位，然后选择虚拟机配置
2创建好虚拟机后通过编辑虚拟机设置下载Ubuntu22.04镜像，开启虚拟机后先选择语言为中文（简体），然后安装Ubuntu,设置好用户后重启即可，此时进行登录后完成一些简单的安装也就完成了。
3在确保系统语言环境正确并启用正确的软件仓库的前提下，安装软件包管理工具，并添加了universe仓库，再添加ROS2的GPG密钥（国内清华镜像），然后添加了ROS2软件源，将ROS2环境变量添加到bashrc并立即生效。
4通过apt包管理器直接安装OpenCV
5安装Vscode主程序,并ROS2开发扩展配置
6本地Git环境初始化配置，SSH密钥对生成与GitHub绑定，远程仓库与本地关联。
五.使用方法：
1编写代码：
C++开发：进入 C++ 源文件目录cd HelloCV/src/cpp
创建新的 C++ 文件touch my_opencv_node.cpp
2编译代码
编译单个 C++ 文件
g++ -std=c++17 src/cpp/my_opencv_node.cpp -o build/my_node \
    `pkg-config --cflags --libs opencv4`
3运行
运行编译后的可执行文件./build/my_node
或者作为 ROS2 节点运行ros2 run hello_cv my_cpp_node
