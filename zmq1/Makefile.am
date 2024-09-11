# 二进制程序
bin_PROGRAMS = server1 client1

ZMQ_LIBS="/usr/lib/x86_64-linux-gnu/libzmq.a"
AC_SUBST(ZMQ_LIBS)

# 源文件
server1_SOURCES = server1.cpp
client1_SOURCES = client1.cpp

# 链接ZeroMQ库
server1_LDADD = $(ZMQ_LIBS)
client1_LDADD = $(ZMQ_LIBS)

# 包含头文件
AM_CPPFLAGS = $(ZMQ_CFLAGS)
