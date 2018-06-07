# CloudHands
利用DPDK捕获流量，流量一般从交换机镜像一个端口并连接到设备工作网口。从捕获的流量进行重组，统计等操作，并对TCP，UDP等为基础的上层应用协议进行还原
为进一步的安全分析提供必要的数据。

#编译方法
1. cd build
2. sh make.sh
3. ./CloudHands_1.0.0.bin
等待编译和安装，完成之后，将其安装在目录 /usr/local/dpdk

#基于TCP的协议还原
1. HTTP
2. MAIL（smtp,pop3,imap）

#基于UDP的协议还原
DNS

#流量分析相关的元数据

