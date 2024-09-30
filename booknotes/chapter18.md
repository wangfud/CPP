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
#### 2．std::initializer_list

C++11提供了模板类initializer_list，可将其用作构造函数的参数

### 18.1.3 声明

#### 1．auto

C++11将其用于实现自动类型推断，auto需要配合显式初始化一致使用。

auto最大的作用就是简化模板类型声明。

```cpp
for(std::initializer_list<double>::iterator p = il.begin();	p!=il.end();p++)
```

替换为如下代码：

```cpp
for(auto p = il.begin();p!=il.end();p++)
```

#### 2．decltype

关键字decltype将变量的类型声明为表达式指定的类型。

语法格式：

```cpp
decltype(x) y;	//y的类型由表达式x计算得到。
```

这在定义模板时特别有用，因为只有等到模板被实例化时才能确定类型：

```cpp
template<typename T,typename U>
void ef(T t,U u)
{
    decltype(T*U) tu;
    ...
}
```

> 例如，如果T为char，U为short，则tu将为int，这是由整型算术自动执行整型提升导致的。

decltype中表达式可以为引用和const

```cpp
int j = 3;
int &k = j;
const int &n = l;
decltype(n) i1;				//i1 : const int &
decltype(j) i2;				//i2 : int
decltype((j)) i3;			//i3 : int &
decltype(k+1) i4;			//i4 : int
```

#### 3．返回类型后置

C++11新增了一种函数声明语法：在函数名和参数列表后面（而不是前面）指定返回类型：

```cpp
double f1(double,int);		//传统语法
auto f2(double,int)->double;//新语法：return type is double
```

这种语法可以使用decltype来指定模板函数的返回类型

```cpp
template<typename T,typename U>
auto eff(T t,U u)->decltype(T*U)
{
    ...
}
```

这里解决的问题是，在编译器遇到eff的参数列表前，T和U还不在作用域内，因此必须在参数列表后使用decltype

#### 4．模板别名：using =

对于冗长或复杂的标识符，如果能够创建其别名将很方便。以前，C++为此提供了typedef：

```cpp
typedef std::vector<std::string>::iterator itType;
```

C++11提供了另一种创建别名的语法

```cpp
using itType = std::vector<std::string>::iterator;
```

差别在于，**新语法也可用于模板部分具体化，但typedef不能**：

```cpp
template<typename T>
using arr12 = std::array<T,12>;	
```

例如，对于下述声明：

```cpp
std::array<double,12> a1;
std::array<std::string,12> a2;
```

可将它们替换为如下声明：

```cpp
arr12<double> a1;
arr12<std:string> a2;
```



#### 5．nullptr

**空指针是不会指向有效数据的指针。**

以前，C++在源代码中使用0表示这种指针，但内部表示可能不同。这带来了一些问题，因为这使得0即可表示指针常量，又可表示整型常量。

C++11新增了关键字nullptr，用于表示空指针；它是指针类型，不能转换为整型类型

为向后兼容，C++11仍允许使用0来表示空指针，因此表达式nullptr == 0为true，但使用nullptr而不是0提供了更高的类型安全。

> 例如，可将0传递给接受int参数的函数，但如果您试图将nullptr传递给这样的函数，编译器将此视为错误

### 18.1.4 智能指针

如果在程序中使用new从堆（自由存储区）分配内存，等到不再需要时，应使用delete将其释放。智能指针可以帮助我们自动完成这个过程。

C++11摒弃了auto_ptr，并新增了三种智能指针：unique_ptr、shared_ptr和weak_ptr

### 18.1.5 异常规范方面的修改

添加了关键字noexcept：表示函数不会引发异常

```cpp
void f989(short,short) noexcept;//don't throw an exception
```

### 18.1.6 作用域内枚举

传统枚举的缺点：如果在同一个作用域内定义两个枚举，它们的枚举成员不能同名

为解决这个问题，c++11引入了使用class或struct定义的枚举

```cpp
enum Old1{yes,no,maybe};					//traditional form
enum class New1{never,sometimes,often,always};//new form
enum class New2{never,lever,server};		//new form
```

引用特定枚举时，需要使用New1::never和New2::never

### 18.1.7 对类的修改

#### 1．显式转换运算符

C++引入了关键字explicit，以禁止单参数构造函数导致的自动转换：

```cpp
class Plebe
{
    Plebe(int);					//automatic int-to-plebe conversion
    explicit Plebe(double);		//requires explicit use
    ...
}
...
Plebe a,b;
a = 5;							//implicit conversion call Plebe(5);
b = 0.5;						//not allowed
b = Plebe(0.5);					//explicit conversion
    
```



C++11拓展了explicit的这种用法，使得可对转换函数做类似的处理

```cpp
class Plebe
{
    ...
    //转换函数
    operator int() const;
    explicit operator double const;
    ...
};
...
Plebe a,b;
int n = a;						//int-to-Plebe automatic conversion
double x = b;					//not allowed
x = double(b);					//explicit conversion ,allowed
```

#### 2．类内成员初始化

以前不可以，现在c++11可以。

通过使用类内初始化，可避免在构造函数中编写重复的代码，从而降低了程序员的工作量、厌倦情绪和出错的机会。

类内成员初始化可使用等号或大括号版本的初始化，但不能使用圆括号版本的初始化

```cpp
class Session
{
    int mem1 = 10;			//类内成员初始化
    double mem2{199.23};	//类内成员初始化
    short mem3;
    
 public:
 	Session(){}
    Session(short s):mem3(s){}
    Session(int n,double d,short s):mem1(n),mem2(d),mem3(s){}
}
```

如果构造函数在成员初始化列表中提供了相应的值，这些类内初始化的默认值将被覆盖
