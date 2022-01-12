# TcpServer
基于Reactor模型事件驱动C++服务器

##1.实现channel类根据事件注册回调函数，事件驱动，有响应时间发生调用回调函数；
##2.抽象连接类Connection,继承实现Httpconn类；
##3.采用主从Reactor模式，一个mainReactor，多subReactor，mainRector负责建立连接，采用Epoll水平触发防止丢失连接，将连接分配给subRector，subRector负责读写以及业务逻辑；
##4.利用RAII以及单例模式实现数据库连接池；
##5.利用单例模式以及阻塞线程安全队列实现异步日志；
##6.利用状态机以及正则表达式实现Http报文解析；
