CXXFLAGS = -O0 -g  -Wall -I ../.. -pthread # CXXFLAGS 定义了编译器选项，包括优化级别（-O0）、调试信息（-g）、警告开启（-Wall）、头文件搜索路径（-I ../..）和使用线程库（-pthread）。
LDFLAGS = -lpthread # LDFLAGS 定义了链接器选项，包括链接到线程库（-lpthread）。
BASE_SRC = ../../logging/Logging.cc ../../logging/LogStream.cc ../../thread/Thread.cc ../../datetime/Timestamp.cc

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

