# ejsCpp

使用c++实现[ejs模板](https://ejs.co/)的部分语法，给[rbook]()使用

## 编译

```sh
mkdir build
cd build
cmake ..
make
```


## 支持的语法

 - 注释 `// /**/`
 - 变量定义`let/var`
    - 整数
    - 字符串 `'',""`
 - for循环
 - 数组
 - 基本运算
 - 内置函数
    - include

支持JSON变量的调用,不支持定义

不支持
 - 函数定义
 - 字符串模板
