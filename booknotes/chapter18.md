# 第十八章 探讨C++新标准
## 18.1 复习前面介绍过的C++11功能

### 18.1.1 新类型

C++11新增了类型long long和unsigned long long，以支持64位（或更宽）的整型；新
增了类型char16_t和char32_t，以支持16位和32位的字符表示；还新增了“原始”字符串。

### 18.1.2 统一的初始化

C++11扩大了用大括号括起的列表（初始化列表）的适用范围，使其可用于所有内置类型和用户定义的类型（即类对象）

使用初始化列表时，可添加等号（=），也可不添加：

```cpp
int x = {5};
double y{2.75};
short quar[5] {4,5,2,43,1};
```

另外，列表初始化语法也可用于new表达式中：

```cpp
int *ar = new int[4] {2,3,41,1};
```

#### 1．缩窄

初始化列表语法可防止缩窄，即禁止将数值赋给无法存储它的数值变量。但允许转换为更宽的类型。

常规赋值：

```cpp
char c1 = 1.33e21;  	//double-to-char ,undefined behavior;
char c2 = 3232323;  	//int-to-char,undefined behavior;
```

初始化表赋值：

```cpp
char c1{1.33e21};		//编译错误
char c2 =  {3232323};	//编译错误
```

只要值在较窄类型的取值范围内，将其转换为较窄的类型也是允许的：

```cpp
char c1{66};  			// int-to-char allowed;
double c2 = {66};		//int-to-double .allowed
```

