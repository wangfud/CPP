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

### 18.1.8 模板和STL方面的修改

#### 1．基于范围的for循环

范围for循环主要的应用对象为：内置数组以及包含方法begin( ) 和end( ) 的类（如std::string）和STL容器

如果要在循环中修改数组或容器的每个元素，可使用引用类型：

```cpp
std::vector<int> vi(6);
for(auto &x:vi)
    x = std::rand();
```



#### 2．新的STL容器

C++11新增了STL容器forward_list、unordered_map、unordered_multimap、unordered_set和unordered_multiset

容器forward_list是一种单向链表，只能沿一个方向遍历；与双向链接的list容器相比，它更简单，在占用存储空间方面更经济。其他四种容器都是使用哈希表实现的。

C++11还新增了模板array（这在第4和16章讨论过）。要实例化这种模板，可指定元素类型和固定的元素数：

```cpp
std::array<int,360> ar;		//array of 360 ints
```

array确实有方法begin( )和end( )，这让您能够对array对象使用众多基于范围的STL算法。

#### 3．新的STL方法

C++11新增了STL方法cbegin( )和cend( )。是begin( )和end( )的const版本。

与此类似，crbegin( )和crend( )是rbegin( )和rend( )的const版本。

#### 4．valarray升级

其最初的设计导致无法将基于范围的STL算法用于valarray对象

C++11添加了两个函数（begin( )和end( )），它们都接受valarray作为参数，并返回迭代器，从而解决了上述问题。

#### 5．摒弃export

#### 6．尖括号

为避免与运算符>>混淆，C++要求在声明嵌套模板时使用空格将尖括号分开：

```cpp

```



![image-20240905110850724](chapter18.assets/image-20240905110850724.png)

C++11不再这样要求：

```cpp

```



![image-20240905110904751](chapter18.assets/image-20240905110904751.png)

### 18.1.9 右值引用

传统的C++引用（现在称为左值引用）使得标识符关联到左值。左值是一个表示数据的表达式（如变量名或解除引用的指针），程序可获取其地址。

最初，左值可出现在赋值语句的左边，但修饰符const的出现使得可以声明这样的标识符，即不能给它赋值，但可获取其地址：

**左值引用**

*左值引用在汇编层面其实和普通的指针是一样的；*定义引用变量必须初始化，因为引用其实就是一个别名

```cpp
int a = 10;
int &b = a;  // 定义一个左值引用变量b
```

无法将字面量值（立即数）赋值给左值引用，因为立即数并没有在内存中存储，而是存储在寄存器中

```cpp
int &var = 10; //编译失败
```

解决方法：使用常引用。

```cpp
const int &var = 10;
//常引用底层实现:
const int temp = 10; 
const int &var = temp;
```

总结：

- 左值引用要求右边的值必须能够取地址，如果无法取地址，可以用常引用；

- 但使用常引用后，我们只能通过引用来读取数据，无法去修改数据，因为其被const修饰成常量引用了

**右值引用**

C++对于左值和右值没有标准定义，但是有一个被广泛认同的说法：

- 可以取地址的，有名字的，非临时的就是左值；如变量，函数返回的引用，const对象等
- 不能取地址的，没有名字的，临时的就是右值；如立即数，函数返回值

从本质上理解，创建和销毁由编译器幕后控制，程序员只能确保在本行代码有效的，就是右值(包括立即数)；而用户创建的，通过作用域规则可知其生存期的，就是左值(包括函数返回的[局部变量](https://zhida.zhihu.com/search?q=局部变量&zhida_source=entity&is_preview=1)的引用以及const对象)。

定义右值引用的格式如下：

```text
类型 && 引用名 = 右值表达式;
```

右值引用用来绑定到右值，绑定到右值以后本来会被销毁的右值的生存期会延长至与绑定到它的右值引用的生存期。

```text
int &&var = 10;
```

在汇编层面右值引用做的事情和常引用是相同的，即产生临时量来存储常量。但是，唯一 一点的区别是，右值引用可以进行读写操作，而常引用只能进行读操作。

右值引用的存在并不是为了取代左值引用，而是充分利用右值(特别是临时对象)的构造来减少对象构造和析构操作以达到提高效率的目的。

引入右值引用的主要目的之一是实现移动语义

右值引用的例子：

```cpp
int x  = 10;
int y = 23;
int && r1 = 13; //右值为立即数
int && r2 = x + y;//右值为函数int operator+(int& ,int&)的返回值
double && r3 = std::sqrt(2.0);//右值为函数返回值。
```



## 18.2 移动语义和右值引用

### 18.2.1 为何需要移动语义

先来看C++11之前的复制过程。假设有如下代码：

![image-20240905154457638](chapter18.assets/image-20240905154457638.png)

![image-20240905154507756](chapter18.assets/image-20240905154507756.png)

从表面上看，语句#1和#2类似，它们都使用一个现有的对象初始化一个vector<string>对象。

如果深入探索这些代码，将发现语句#2中allcaps( )创建了对象temp，然后程序删除allcaps( )返回的临时对象。假设vsstr这个对象管理的string非常多。这个临时对象的创建之后 利用复制构造创建了vsstr_copy,最后零食对象temp有删除了。若将临时对象的所有权直接转让给vstr_cppy2，可以让这个过程更加高效。这种方法被称为移动语义。

要实现移动语义，需要采取某种方式，让编译器知道什么时候需要复制，什么时候不需要。此时就需要使用右值引用。

可定义两个构造函数。其中一个是常规复制构造函数，它使用const左值引用作为参数；另一个是移动构造函数，它使用右值引用作为参数，该引用关联到右值实参。

### 18.2.2 一个移动示例

```cpp
// useless.cpp -- an otherwise useless class with move semantics
#include <iostream>
using namespace std;

// interface
class Useless
{
private:
    int n;          // number of elements
    char * pc;      // pointer to data
    static int ct;  // number of objects
    void ShowObject() const;
public:
    Useless();
    explicit Useless(int k);
    Useless(int k, char ch);
    Useless(const Useless & f); // regular copy constructor
    Useless(Useless && f);      // move constructor
    ~Useless();
    Useless operator+(const Useless & f)const;
// need operator=() in copy and move versions
    void ShowData() const;
};

// implementation
int Useless::ct = 0;

Useless::Useless()
{
    ++ct;
    n = 0;
    pc = nullptr;
    cout << "default constructor called; number of objects: " << ct << endl;
    ShowObject();
}

Useless::Useless(int k) : n(k)
{
    ++ct; 
    cout << "int constructor called; number of objects: " << ct << endl;
    pc = new char[n];
    ShowObject();
}

Useless::Useless(int k, char ch) : n(k)
{
    ++ct;
    cout << "int, char constructor called; number of objects: " << ct << endl;
    pc = new char[n];
    for (int i = 0; i < n; i++)
        pc[i] = ch;
    ShowObject();
}

Useless::Useless(const Useless & f): n(f.n) 
{
    ++ct;
    cout << "copy const called; number of objects: " << ct << endl;
    pc = new char[n];
    for (int i = 0; i < n; i++)
        pc[i] = f.pc[i];
    ShowObject();
}

Useless::Useless(Useless && f): n(f.n) 
{
    ++ct;
    cout << "move constructor called; number of objects: " << ct << endl;
    pc = f.pc;       // steal address
    f.pc = nullptr;  // give old object nothing in return
    f.n = 0;
    ShowObject();
}

Useless::~Useless()
{
    cout << "destructor called; objects left: " << --ct << endl;
    cout << "deleted object:\n";
    ShowObject();
    delete [] pc;
}

Useless Useless::operator+(const Useless & f)const
{
    cout << "Entering operator+()\n";
    Useless temp = Useless(n + f.n);
    for (int i = 0; i < n; i++)
        temp.pc[i] = pc[i];
    for (int i = n; i < temp.n; i++)
        temp.pc[i] = f.pc[i - n];
    cout << "temp object:\n";
    cout << "Leaving operator+()\n";
    return temp;
}

void Useless::ShowObject() const
{ 
    cout << "Number of elements: " << n;
    cout << " Data address: " << (void *) pc << endl;
}

void Useless::ShowData() const
{
    if (n == 0)
        cout << "(object empty)";
    else
        for (int i = 0; i < n; i++)
            cout << pc[i];
    cout << endl;
}

// application
int main()
{
    {
        Useless one(10, 'x');
        Useless two = one;          // calls copy constructor
        Useless three(20, 'o');
        Useless four(one + three);  // calls operator+(), move constructor
        cout << "object one: ";
        one.ShowData();
        cout << "object two: ";
        two.ShowData();
        cout << "object three: ";
        three.ShowData();
        cout << "object four: ";
        four.ShowData();
    }
    // cin.get();
}
```

### 18.2.3 移动构造函数解析

![image-20240905162301216](chapter18.assets/image-20240905162301216.png)

对象one是左值，与左值引用匹配，而表达式one + three是右值，与右值引用匹配。

### 18.2.4 赋值

适用于构造函数的移动语义考虑也适用于赋值运算符。

例如，下面演示了如何给Useless类编写复制赋值运算符和移动赋值运算符：

![image-20240905162552600](chapter18.assets/image-20240905162552600.png)

与移动构造函数一样，移动赋值运算符的参数也不能是const引用，因为这个方法修改了源对象。

### 18.2.5 强制移动

移动构造函数和移动赋值运算符使用右值。如果使用左值，如何强制调用移动构造函数？

有两种方法：

- 使用运算符static_cast<>将对象的类型强制转换为Useless &&
- 使用头文件utility中声明的函数std::move( )

![image-20240905163452956](chapter18.assets/image-20240905163452956.png)

右值引用带来的主要好处并非是让他们能够编写使用右值引用的代码，而是能够使用利用右值引用实现移动语义的库代码

例如，STL类现在都有复制构造函数、移动构造函数、复制赋值运算符和移动赋值运算符





## 18.3 新的类功能

### 18.3.1 特殊的成员函数

在原有4个特殊成员函数（默认构造函数、复制构造函数、复制赋值运算符和析构函数）的基础上，C++11新增了两个：移动构造函数和移动赋值运算符。这些成员函数是编译器在各种情况下自动提供的。

假定类名为Someclass，这两个默认的构造函数的原型如下：

![image-20240909104241403](chapter18.assets/image-20240909104241403.png)

![image-20240909104353905](chapter18.assets/image-20240909104353905.png)

如果您提供了析构函数、复制构造函数或复制赋值运算符，编译器将不会自动提供移动构造函数和移动赋值运算符；如果您提供了移动构造函数或移动赋值运算符，编译器将不会自动提供复制构造函数和复制赋值运算符。

另外，默认的移动构造函数和移动赋值运算符的工作方式与复制版本类似：执行逐成员初始化并复制内置类型。如果成员是类对象，将使用相应类的构造函数和赋值运算符，就像参数为右值一样。如果定义了移动构造函数和移动赋值运算符，这将调用它们；否则将调用复制构造函数和复制赋值运算符。

### 18.3.2 默认的方法和禁用的方法

您提供了移动构造函数，因此编译器不会自动创建默认的构造函数、复制构造函数和复制赋值构造函数。在这些情况下，您可使用关键字default显式地声明这些方法的默认版本：

![image-20240909104855998](chapter18.assets/image-20240909104855998.png)

关键字delete可用于禁止编译器使用特定方法

例如，要禁止复制对象，可禁用复制构造函数和复制赋值运算符：

![image-20240909104954512](chapter18.assets/image-20240909104954512.png)

关键字default只能用于6个特殊成员函数，但delete可用于任何成员函数。delete的一种可能用法是禁止特定的转换。

例如，假设Someclass类有一个接受double参数的方法：

![image-20240909105151526](chapter18.assets/image-20240909105151526.png)

![image-20240909105155505](chapter18.assets/image-20240909105155505.png)

int值5将被提升为5.0，进而执行方法redo( )。

![image-20240909105221952](chapter18.assets/image-20240909105221952.png)

在这种情况下，方法调用sc.redo(5)与原型redo(int) 匹配。编译器检测到这一点以及redo(int) 被禁用后，将这种调用视为编译错误

### 18.3.3 委托构造函数

为了避免在给一个类提供多个构造函数时编写重复的代码，c++11允许在一个构造函数中使用里一个构造函数，这被称为委托。

委托使用成员初始化列表语法的变种：

![image-20240909112006580](chapter18.assets/image-20240909112006580.png)

### 18.3.4 继承构造函数

C++11提供了一种让派生类能够继承基类构造函数的机制

C++98提供了一种让名称空间中函数可用的语法：

![image-20240909112401720](chapter18.assets/image-20240909112401720.png)

这让函数fn的所有重载版本都可用。也可使用这种方法让基类的所有非特殊成员函数对派生类可用

该种语法也可以应用到类集成中。让派生类使用从父类中引入过来的方法。



![image-20240909112534547](chapter18.assets/image-20240909112534547.png)

![image-20240909112539829](chapter18.assets/image-20240909112539829.png)

C++11将这种方法用于构造函数。这让派生类继承基类的所有构造函数（默认构造函数、复制构造函数和移动构造函数除外），但不会使用与派生类构造函数的特征标匹配的构造函数：

![image-20240909112800197](chapter18.assets/image-20240909112800197.png)

![image-20240909112803730](chapter18.assets/image-20240909112803730.png)

由于没有构造函数DR(int, double)，因此创建DR对象o3时，将使用继承而来的BS(int,double)。

请注意，继承的基类构造函数只初始化基类成员；如果还要初始化派生类成员，则应使用成员列表初始化语法：

![image-20240909113030750](chapter18.assets/image-20240909113030750.png)

.

### 18.3.5 管理虚方法：override和final

虚方法对实现多态类层次结构很重要，让基类引用或指针能够根据指向的对象类型调用相应的方法。

当基类和派生类虚方法相同，但是函数特征标不同时，基类的同名虚方法会被隐藏，而不是被覆盖。

![image-20240909113802698](chapter18.assets/image-20240909113802698.png)

![image-20240909113816547](chapter18.assets/image-20240909113816547.png)

在C++11中，可使用虚说明符override指出您要覆盖一个虚函数：将其放在参数列表后面。此时如果与基类方法不匹配，会出现编译错误。

![image-20240909113908902](chapter18.assets/image-20240909113908902.png)

上述方法会出现编译错误。



说明符final解决了另一个问题。您可能想禁止派生类覆盖特定的虚方法，为此可在参数列表后面加上final。例如，下面的代码禁止Action的派生类重新定义函数f( )：

![image-20240909113427457](chapter18.assets/image-20240909113427457.png)

说明符override和final并非关键字，而是具有特殊含义的标识符。这意味着编译器根据上下文确定它们是否有特殊含义





## 18.4 Lambda函数

lambda函数对使用函数谓词的STL算法来说非常有用。

### 18.4.1 比较 函数指针、函数符和Lambda函数

假设您要生成一个随机整数列表，并判断其中多少个整数可被3整除，多个少整数可被13整除。

> 使用这个例子需要使用两个stl函数：
>
> std::generate(start , end , function)
>
> 用来通过函数生成[start,end)区间的元素。
>
> std:count_if(start ,end ,function)
>
> 统计[start，end)区间的元素满足function函数的个数，function函数返回true表示满足

#### 函数指针实现

![image-20240909114529528](chapter18.assets/image-20240909114529528.png)

通过使用算法count_if( )，很容易计算出有多少个元素可被3整除。首先定义两个函数，用来判断是否能被3或13整除

![image-20240909114742487](chapter18.assets/image-20240909114742487.png)

![image-20240909114744829](chapter18.assets/image-20240909114744829.png)

![image-20240909114752096](chapter18.assets/image-20240909114752096.png)

#### 函数符实现

下面使用函数符实现删除函数定义。

![image-20240909114905967](chapter18.assets/image-20240909114905967.png)

![image-20240909114959261](chapter18.assets/image-20240909114959261.png)

#### lambda实现

名称lambda来自lambda calculus（λ演算）—一种定义和应用函数的数学系统。这个系统让您能够使用匿名函数—即无需给函数命名

与前述函数f3( )对应的lambda如下

![image-20240909115142608](chapter18.assets/image-20240909115142608.png)

这与f3( )的函数定义很像：

![image-20240909115153440](chapter18.assets/image-20240909115153440.png)

差别有两个：

- 使用[]替代了函数名（这就是匿名的由来）
- 没有声明返回类型。返回类型相当于使用decltyp根据返回值推断得到的，这里为bool。如果lambda不包含返回语句，推断出的返回类型将为void。

就这个示例而言，您将以如下方式使用该lambda：

![image-20240909115731262](chapter18.assets/image-20240909115731262.png)

仅当lambad表达式完全由一条返回语句组成时，自动类型推断才管用；否则，需要使用新增的返回类型后置语法：

![image-20240909115809094](chapter18.assets/image-20240909115809094.png)

当需要使用一个lambda函数两次时，可以给lambda定义一个名称：

![image-20240909120051065](chapter18.assets/image-20240909120051065.png)

您甚至可以像使用常规函数那样使用有名称的lambda：

![image-20240909120109362](chapter18.assets/image-20240909120109362.png)

关于lambda，需要注意：

1. mod3的实际类型随实现而异，它取决于编译器使用什么类型来跟踪lambda。

2. lambad可访问作用域内的任何动态变量；要捕获要使用的变量，可将其名称放在中括号内。[&]让您能够按引用访问所有动态变量，而[=]让您能够按值访问所有动态变量。

>  [ted, &ed]让您能够按值访问ted以及按引用访问ed
>
> [&, ted]让您能够按值访问ted以及按引用访问其他所有动态变量
>
> [=, &ed]让您能够按引用访问ed以及按值访问其他所有动态变量。

基于上面的特性，我们可以使用for_each来实现统计，代码如下：

![image-20240909144658498](chapter18.assets/image-20240909144658498.png)

通过利用这种技术，可使用一个lambda表达式计算可被3整除的元素数和可被13整除的元素数：

![image-20240909144807769](chapter18.assets/image-20240909144807769.png)

> [&]让您能够在lambad表达式中使用所有的自动变量，包括count3和count13

### 18.4.2 为何使用lambda



## 18.5 包装器

C++提供了多个包装器（wrapper，也叫适配器[adapter]）。这些对象用于给其他编程接口提供更一致或更合适的接口

C++11提供了其他的包装器：

- 模板bind可替代bind1st和bind2nd，但更灵活
- 模板mem_fn让您能够将成员函数作为常规函数进行传递
- 模板reference_wrapper让您能够创建**行为像引用但可被复制的对象**
- 包装器function让您能够以统一的方式处理多种类似于函数的形式。

### 18.5.1 包装器function及模板的低效性

请看下面的代码行：

![image-20240909153859249](chapter18.assets/image-20240909153859249.png)

ef是什么呢？

ef是可调用类型，C++中可调用类型的种类较多：函数名、函数指针、函数对象或有名称的lambda表达式。

由于可调用类型的种类比较丰富，当使用函数使用模板来表示可调用类型时，其效率非常低。

例如，有如下函数模板：

![image-20240909154757050](chapter18.assets/image-20240909154757050.png)

有如下函数名，函数对象和lambda匿名函数

![image-20240909155039863](chapter18.assets/image-20240909155039863.png)

![image-20240909155308664](chapter18.assets/image-20240909155308664.png)

现在使用模板函数use_f来调用上述各个函数：

![image-20240909155513173](chapter18.assets/image-20240909155513173.png)

从测试结果可以发现：当分别使用函数名，函数对象和lambda表达式作为上述函数的参数f调用use_f时，函数会被各实例化一次。

- 对于函数名，F被实例化函数指针类型 ，如下面的函数模板参数F被实例化为类型：double (*)(double) 

- 对于可调用对象，模板参数被实例化为Fp对象的类型。

- 对于lambda函数，F类型设置为编译器为lambda表达式使用的类型。

上述三种可调用对象应用于use_f时都有一个共同点：即他们的调用特征标相同，而函数特征标不同。

他们的调用特征标为double(double).

模板function的作用就是可以将调用特征标相同的可调用对象封装成一个统一的对象，从而让函数模板中涉及的可调用对象只需要进行一次实例化。

### 18.5.2 使用模板function修复问题

模板function是在头文件functional中声明的，它从调用特征标的角度定义了一个对象，可用于包装调用特征标相同的函数指针、函数对象或lambda表达式。

例如，下面的声明创建一个名为fdci的function对象，它接受一个char参数和一个int参数，并返回一个double值：

![image-20240909160127671](chapter18.assets/image-20240909160127671.png)

然后，可以将接受一个char参数和一个int参数，并返回一个double值的任何函数指针、函数对象或lambda表达式赋给它。

![image-20240909160204603](chapter18.assets/image-20240909160204603.png)

### 18.5.3 其他方式

对于模板function，还可以通过下面两种方式使用：

- 方式1：使用类型别用定义function

![image-20240909160334885](chapter18.assets/image-20240909160334885.png)

- 方法2：可以直接在函数模板中，将可调用对象的类型使用function模板进行包装。

![image-20240909160541462](chapter18.assets/image-20240909160541462.png)

这样函数调用将如下：

![image-20240909160610316](chapter18.assets/image-20240909160610316.png)

## 18.6 可变参数模板

可变参数模板（variadic template）让您能够创建这样的模板函数和模板类，即可接受可变数量的参数。

例如，假设要编写一个函数，它可接受任意数量的参数，参数的类型只需是cout能够显示的即可，并将参数显示为用逗号分隔的列表。请看下面的代码：

![image-20240910155350766](chapter18.assets/image-20240910155350766.png)

要创建可变参数模板，需要理解几个要点：

- 模板参数包（parameter pack）；
- 函数参数包；
- 展开（unpack）参数包；
- 递归

### 18.6.1 模板和函数参数包

C++11提供了一个用省略号(…)表示的元运算符（meta-operator），可以让声明：

- **模板参数包的标识符**：是一个类型列表。
- **函数参数包**的标识符：是一个值列表

![image-20240910162001928](chapter18.assets/image-20240910162001928.png)

其中，Args是一个模板参数包，而args是一个函数参数包。

请看下面的函数调用：

![image-20240910162034011](chapter18.assets/image-20240910162034011.png)

在这种情况下：

- Args包含与函数调用中的参数匹配的类型：char、int、const char *和double。
- args包含值‘S’、80、“sweet”和4.5

这样，可变参数模板show_list1( )与下面的函数调用都匹配：

![image-20240910162143743](chapter18.assets/image-20240910162143743.png)

### 18.6.2 展开参数包

但函数如何访问这些包的内容呢？

可将省略号放在函数参数包名的右边，将参数包展开，如：args...展开为一个函数参数列表。

![image-20240910162358549](chapter18.assets/image-20240910162358549.png)

上述调用存在缺陷，将存在一个无限递归

### 18.6.3 在可变参数模板函数中使用递归

这里的核心理念是，将函数参数包展开，对列表中的第一项进行处理，再将余下的内容传递给递归调用，以此类推，直到列表为空。

与常规递归一样，确保递归将终止很重要。这里的技巧是将模板头改为如下所示：

![image-20240910162735894](chapter18.assets/image-20240910162735894.png)

对于上述定义，show_list3( )的第一个实参决定了T和value的值，而其他实参决定了Args和args的值。

每次递归调用都将处理一个参数，并传递缩短了的列表，直到列表为空为止。

![image-20240910162934717](chapter18.assets/image-20240910162934717.png)

### 18.6.4 改进

可对show_list3( )做两方面的改进。

- 当前，该函数在列表的每项后面显示一个逗号，但如果能省去最后一项后面的逗号就好了。为此，可添加一个处理一项的模板，并让其行
  为与通用模板稍有不同：

![image-20240910163151205](chapter18.assets/image-20240910163151205.png)

- 另一个可改进的地方是，当前的版本按值传递一切，可以修改为按引用传递。

为此，可将下述代码：

![image-20240910163320769](chapter18.assets/image-20240910163320769.png)

替换为如下代码：


![image-20240910163329739](chapter18.assets/image-20240910163329739.png)

