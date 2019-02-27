**2018-01-10**

qkc项目是quark项目是一个子项目，它实现windows下的posix接口，使得linux下的c源码，可以直接在visual studio中编译，并生成本地库。该项目只在vs下有效，在linux下只有简单的重定向。
1. qkc和mingw、cygwin的目的是一致的，都是在windows下编译linux的c源码，生成本地可执行文件，实现linux项目到windows的迁移。c语言有悠久的历史，在linux下存在大量的优秀作品，能够将他们移植到windows下，是个不错的主意。
2. mingw是在windows下模拟linux，实现的是os层级的模拟器，在mingw中和linux下相似的用户体验，但它没有充分发挥windows的方便性。而qkc只是一个库，类似于libc，它实现的目标，是在vs中能够编译linux的代码。并没有任何linux的用户体验。
3. mingw使用的是gcc编译和gdb调试，而qkc只是vs的一个库，等价于windows下的libc库，可以充分体验vs在编译调试方便的优势。而且编写出来的源码，同样可以无缝的在linux编译。


vs在本地实现了部分c标准接口，但是每个版本都不一样，因此需要根据每个不同的vs版本号，做不同的对应。见下面vs的版本号对应表。


VS产品名称 | 对应版本号 | 发布年份
---|---|---
MS VC++ 14.0 | _MSC_VER = 1900 | (Visual Studio 2015)
MS VC++ 12.0 | _MSC_VER = 1800 |  (VisualStudio 2013)
MS VC++ 11.0 | _MSC_VER = 1700 | (VisualStudio 2012)
MS VC++ 10.0 | _MSC_VER = 1600 | (VisualStudio 2010)
MS VC++ 9.0  | _MSC_VER = 1500 | (VisualStudio 2008)
MS VC++ 8.0  | _MSC_VER = 1400 | (VisualStudio 2005)
MS VC++ 7.1  | _MSC_VER = 1310 | (VisualStudio 2003)
MS VC++ 7.0  | _MSC_VER = 1300 | (VisualStudio .NET)
MS VC++ 6.0  | _MSC_VER = 1200 | (VisualStudio 98)
MS VC++ 5.0  | _MSC_VER = 1100 | (VisualStudio 97)



**2018-02-28**

经过多方尝试，在visual studio下直接实现posix c接口存在这么几个难点

1.visual studio实现的msvcr 、msvcp 、windows sdk几个项目之间的头文件，存在严重的依赖关系，分离需要很大的工作量

2.在默认情况下，visual studio会自动包含这些头文件目录，同时按照编译和链接选项，关联到不同的库文件。

3.根据生成exe和dll的选项，vs会自动加入初始化代码，并选择对应的库，而这些库就包含在默认库路径中。如果要撇开默认库，就要自己实现这些初始化代码，难度极高。

4.单独实现posix c兼容性，在现实中的使用会受到限制。毕竟在linux下，也多数是c/c++混合使用，纯粹的C代码，还是历史比较久远的代码。

经过综合考虑，还是一种折衷的方案来解决这个难题。依然使用原生的头文件和库，同时单独建立一个目录，该目录下的子目录结构和linux下一致。如果posix c的头文件在vs下没有存在，那么好办，重新实现。如果已经存在，就直接引用。最麻烦的是，该头文件已经存在，但是部分函数没有实现。则根据宏，在windows实现posix 函数。

这种方式，在编译linux源码时，会比较丑陋一点，需要根据函数是否存在，引入新的头文件。

还有一种解决方案，可以确保头文件和posix一致，但是实现要麻烦。
1.创建一个新的动态库，在工程属性中忽略默认标准包含路径设置为是，但默认库还是引入。
2.按照posix的目录结构，创建头文件，引入msvcr库中符合posix规范的函数，如果没有，那么在该库中实现。
3.c++的问题比较简单，将相关头文件拷贝过来，稍微调试一下就可以了。

任何linux的源码在vs下编译，只要保持忽略默认标准包含路径，而强制使用该动态库的头文件，就能够生成windows原生的程序。


这两个方案，都没有对vs做大规模的变动，尽最大可能的使用原生的库，所以对vs的版本依赖低，兼容性更好。
