LIB_SRC = EventLoop.cc # 定义了一个变量LIB_SRC，它包含一个源文件EventLoop.cc。通常，这样的变量用于存储库文件的源代码文件列表
BINARIES = test1 test2 # 定义了一个变量BINARIES，它包含两个可执行文件的名称，即test1和test2。

all: $(BINARIES) # 定义了一个目标all，它依赖于BINARIES中列出的所有可执行文件。这意味着运行make all时，会构建并生成test1和test2这两个可执行文件。

include ../reactor.mk # 包含了外部文件reactor.mk。这个外部文件可能包含一些通用的构建规则或变量，以便在多个Makefile中共享。通过这种方式，可以保持Makefile的简洁性并避免重复代码。

test1: test1.cc # 分别定义了两个目标，test1和test2。它们分别依赖于test1.cc和test2.cc这两个源文件。这表示，如果这两个源文件发生了变化，那么对应的目标就需要重新构建。
test2: test2.cc
