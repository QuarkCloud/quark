
1、size_t和wchar_t在c语言下，vs2008不支持，需要独立定义

2、stdbool.h默认没有，需要加入

3、sizeof不能求匿名结构的大小

4、gcc的内建函数__builtin_xxx和__atomic_xxx类似模板，支持任意类型，在vs2008下，实现比较麻烦。

5、C的标准在vs2008下，支持不完整，如果改成cpp文件会好点。

6、gcc的内置宏 \_\_func\_\_ 对影城 \_\_FUNCTION\_\_ 

7、内置\_\_builtin\_unreachable没有对应的函数

8、\_\_attribute\_\_位置在后，而且没有完全的对应的，需要改代码。
