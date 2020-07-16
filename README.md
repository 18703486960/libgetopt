# libgetopt
c++17编写的解析命令行参数的小型工具， 简单易用



将头文件包含到项目目录下即可

使用方法如下

```c++
#include <iostream>
#include "getopt.h"
#include <cstdio>
#include <cstring>

int main() {
    bool help = libgetopt::getarg( false, "-h", "--help", "-?" );
    int version = libgetopt::getarg( 20, "-v", "--version", "--show-version" );
    int depth = libgetopt::getarg( 1, "-d", "--depth", "--max-depth");
    std::string file = libgetopt::getarg( std::string ("qwe"), "-f", "--file" );
    const char* name = libgetopt::getarg("213", "-name");

    std::cout << help << ',' << file << ',' << version << std::endl;
    return 0;
}

```



```c++
./a.out -h -f=123.txt -name=456
```

