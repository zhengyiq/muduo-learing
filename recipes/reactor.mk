CXXFLAGS = -O0 -g  -Wall -I ../.. -pthread # CXXFLAGS 定义了编译器选项，包括优化级别（-O0）、调试信息（-g）、警告开启（-Wall）、头文件搜索路径（-I ../..）和使用线程库（-pthread）。
LDFLAGS = -lpthread # LDFLAGS 定义了链接器选项，包括链接到线程库（-lpthread）。
BASE_SRC = ../../logging/Logging.cc ../../logging/LogStream.cc ../../thread/Thread.cc ../../datetime/Timestamp.cc # 包含了一组基本的源代码文件。

$(BINARIES): # 这是一个通用的构建规则，用于构建所有在BINARIES变量中列出的可执行文件。
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

# $@ 是一个自动变量，代表目标文件（可执行文件的名称）。
# $(LIB_SRC) 是Makefile中定义的另一个变量，代表项目特定的源文件。
# $(BASE_SRC) 是定义的基本源文件。
# $(filter %.cc,$^) 选择所有以.cc结尾的依赖项文件。

clean:
	rm -f $(BINARIES) core  
# rm -f $(BINARIES) core 删除所有可执行文件和核心文件。

