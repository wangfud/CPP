

# 第十六章 string类和标准模板库

本章内容包括：

- 标准C++ string类；
- 模板auto_ptr、unique_ptr 和 shared_ptr；
- 标准模板库；
- 容器类；
- 迭代器
- 函数对象；
- STL算法
- 模板initializer_list

## 16.1 string类

### 16.1.1 构造字符串

![image-20240812161730181](chapter16.assets/image-20240812161730181.png)

第6个构造函数有一个模板参数：

![image-20240812162355730](chapter16.assets/image-20240812162355730.png)

begin和end将像指针那样，指向内存中两个位置（通常，begin和end可以是迭代器——广泛用于STL中的广义化指针）。构造函数将使用begin和end指向的位置之间的值，对string对象进行初始化。[begin, end)来自数学中，意味着包括begin，但不包括end在内的区间

#### 2．C++11新增的构造函数

构造函数string（string && str）类似于复制构造函数，导致新创建的string为str的副本。但与复制构造函数不同的是，它不保证将str视为const。这种构造函数被称为移动构造函数（move constructor）。在有些情况下，编译器可使用它而不是复制构造函数，以优化性能

构造函数string（initializer_list<char> il）让您能够将列表初始化语法用于string类。也就是说，它使得下面这样的声明是合法的：

![image-20240812162741986](chapter16.assets/image-20240812162741986.png)

### 16.1.2 string类输入

**对于类，很有帮助的另一点是，知道有哪些输入方式可用。**

对于C-风格字符串，有3种方式：

![image-20240812162845268](chapter16.assets/image-20240812162845268.png)

对于string对象，有两种方式：

![image-20240812162908349](chapter16.assets/image-20240812162908349.png)

两个版本的getline( )都有一个可选参数，用于指定使用哪个字符来确定输入的边界：

![image-20240812163115550](chapter16.assets/image-20240812163115550.png)

在功能上，它们之间的主要区别在于，string版本的getline( )将自动调整目标string对象的大小，使之刚好能够存储输入的字符：

![image-20240812163145466](chapter16.assets/image-20240812163145466.png)

> 自动调整大小的功能让string版本的getline( )不需要指定读取多少个字符的数值参数。

下面更深入地探讨一下string输入函数。

上述两个函数对string自动调整string大小的限制：

- 第一个限制因素是string对象的最大允许长度，由常量string::npos指定。这通常是最大的unsigned int值。如果您试图将整个文件的内容读取到单个string对象中，这可能成为限制因素。
- 第二个限制因素是程序可以使用的内存量。

string版本的getline( )函数从输入中读取字符，并将其存储到目标string中，直到发生下列三种情况之一：

- **到达文件尾**，在这种情况下，输入流的eofbit将被设置，这意味着方法fail( )和eof( )都将返回true；
- **遇到分界字符（默认为\n）**，在这种情况下，**将把分界字符从输入流中删除，但不存储它**；
- **读取的字符数达到最大允许值（string::npos和可供分配的内存字节数中较小的一个）**，在这种情况下，将设置输入流的failbit，这意味着方法fail( )将返回true。

> 输入流对象有一个统计系统，用于跟踪流的错误状态。在这个系统中，检测到文件尾后将设置eofbit寄存器，检测到输入错误时将设置failbit寄存器，出现无法识别的故障（如硬盘故障）时将设置badbit寄存器，一切顺利时将设置goodbit寄存器

程序清单16.2是一个从文件中读取字符串的简短示例，它假设文件中包含用冒号字符分隔的字符串，并使用指定分界符的getline( )方法。然后，显示字符串并给它们编号，每个字符串占一行。

程序清单16.2 strfile.cpp

```cpp
// strfile.cpp -- read strings from a file
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
int main()
{
     using namespace std;
     ifstream fin;
     fin.open("tobuy.txt");
     if (fin.is_open() == false)
     {
        cerr << "Can't open file. Bye.\n";
        exit(EXIT_FAILURE);
     }
     string item;
     int count = 0;
     
     getline(fin, item, ':');
     while (fin)  // while input is good
     {
        ++count;
        cout << count <<": " << item << endl;
        getline(fin, item,':');     
     }
     cout << "Done\n";
     fin.close();
	 // std::cin.get();
	 // std::cin.get();
     return 0;
}

```

### 16.1.3 使用字符串

#### 1.比较字符串

String类对全部6个关系运算符都进行了重载

对于每个关系运算符，都以三种方式被重载，以便能够将string对象与另一个string对象、C-风格字符串进行比较，并能够将C-风格字符串与string对象进行比较：

![image-20240812164715639](chapter16.assets/image-20240812164715639.png)

#### 2.字符串长度

size( )和length( )成员函数都返回字符串中的字符数

#### 3.字符串查找

size_type find(const string &str, size_type pos = 0)const

从pos位置查找子串str。找到首次出现的索引，找不到返回 返回string :: npos

![image-20240812165156398](chapter16.assets/image-20240812165156398.png)

string库还提供了相关的方法：rfind( )、find_first_of( )、find_last_of( )、find_first_not_of( )和find_last_not_of( )，

### 16.1.4 string还提供了哪些功能

- 字符串删除部分或者全部
- 字符串替换
- 插入字符（子串）或删除字符（子串）

- 从字符串中提取子字符串
- 将一个字符串中的内容复制到另一个字符串中
- 交换两个字符串的内容

### 16.1.5 字符串种类

string库实际上是、基于一个模板类的：

![image-20240812165736949](chapter16.assets/image-20240812165736949.png)

模板basic_string有4个具体化，每个具体化都有一个typedef名称：

![image-20240812165817633](chapter16.assets/image-20240812165817633.png)

## 16.2 智能指针模板类

智能指针是行为类似于指针的类对象，但这种对象还有其他功能。

为什么要使用智能指针？

请看下面的函数：

![image-20240812171253044](chapter16.assets/image-20240812171253044.png)

上述代码的问题是：每次调用函数都会在堆中创建一个对象，但是该对象并没有回收。

就算手动用delete回收也会出现风险：

![image-20240812171821578](chapter16.assets/image-20240812171821578.png)

当出现异常时，delete将不被执行，因此也将导致内存泄漏。

### 16.2.1 使用智能指针

这三个智能指针模板（auto_ptr、unique_ptr和shared_ptr）都定义了类似指针的对象，可以将new获得（直接或间接）的地址赋给这种对象。当智能指针过期时，其析构函数将使用delete来释放内存。

![image-20240812173948045](chapter16.assets/image-20240812173948045.png)

要创建智能指针对象，必须包含头文件memory，该文件模板定义。

![image-20240812174313184](chapter16.assets/image-20240812174313184.png)

![image-20240812174318182](chapter16.assets/image-20240812174318182.png)

>  模板auto_ptr是C++98提供的解决方案，C++11已将其摒弃，并提供了另外两种解决方案。

下面是使用auto_ptr修改该函数的结果：

![image-20240812174357452](chapter16.assets/image-20240812174357452.png)

所有智能指针类都一个explicit构造函数，该构造函数将指针作为参数。因此不需要自动将指针转换为智能指针对象：

![image-20240814152241432](chapter16.assets/image-20240814152241432.png)

智能指针对象的很多方面都类似于常规指针:，如果ps是一个智能指针对象，则可以对它执行解除引用操作（* ps）、用它来访问结
构成员（ps->puffIndex）、将它赋给指向相同类型的常规指针。

先说说对全部三种智能指针都应避免的一点：

![image-20240814152450601](chapter16.assets/image-20240814152450601.png)

> pvac过期时，程序将把delete运算符用于非堆内存，这是错误的。

### 16.2.2 有关智能指针的注意事项

为何摒弃auto_ptr呢？

先来看下面的赋值语句：

![image-20240814155822478](chapter16.assets/image-20240814155822478.png)

上述语句使得两个指针将指向同一个string对象，这是不能接受的，因为程序将试图删除同一个对象两次。一次是ps过期时，另一次是vocation过期时。

要避免这种问题，方法有多种：

- 定义赋值运算符，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本。

- 建立所有权（ownership）概念，对于特定的对象，只能有一个智能指针可拥有它，这样只有拥有对象的智能指针的构造函数会删除该对象。然后，让赋值操作转让所有权。这就是用于**auto_ptr和unique_ptr的策略**，但unique_ptr的策略更严格。

- 创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为引用计数（referencecounting）。这是shared_ptr采用的策略。

  > 例如，赋值时，计数将加1，而指针过期时，计数将减1。仅当最后一个指针过期时，才调用delete。

当然，同样的策略也适用于复制构造函数。



```cpp
#include <iostream>
#include <string>
#include <memory>

int main(){
    using namespace std;
    auto_ptr<string> films[5] = {
            auto_ptr<string> (new string("Fowl Balls")),
            auto_ptr<string> (new string("Duck Walks")),
            auto_ptr<string> (new string("Chicken Runs")),
            auto_ptr<string> (new string("Turkey Errors")),
            auto_ptr<string> (new string("Goose Eggs"))
    };

    auto_ptr<string> pwin;
    pwin = films[2];
    cout << "The nominees for best avian baseball film are\n";
    for (int i = 0; i < 5; ++i) {
        cout<<*films[i]<<endl;
    }
    cout << "The winner is " << *pwin << "!\n";
    return 0;
}
```

![image-20240814161015476](chapter16.assets/image-20240814161015476.png)

这里的问题在于，下面的语句将所有权从films[2]转让给pwin：

![image-20240814161033734](chapter16.assets/image-20240814161033734.png)

这导致films[2]不再引用该字符串。在auto_ptr放弃对象的所有权后，便可能使用它来访问该对象。当程序打印films[2]指向的字符串时，却发现这是一个空指针.

如果在程序清单16.6中使用shared_ptr代替auto_ptr，则程序将正常运行，其输出如下

![image-20240814161242554](chapter16.assets/image-20240814161242554.png)

这次pwin和films[2]指向同一个对象，而引用计数从1增加到2。在程序末尾，后声明的pwin首先调用其析构函数，该析构函数将引用计数降低到1。然后，shared_ptr数组的成员被释放，对filmsp[2]调用析构函数时，将引用计数降低到0，并释放以前分配的空间。

如果使用unique_ptr，结果将如何呢？与auto_ptr一样，unique_ptr也采用所有权模型。但使用unique_ptr时，程序不会等到运行阶段崩溃，而在编译器因下述代码行出现错误：

![image-20240814161351201](chapter16.assets/image-20240814161351201.png)

### 16.2.3 unique_ptr为何优于auto_ptr

请先看auto_ptr的情况：

![image-20240814162145670](chapter16.assets/image-20240814162145670.png)

在语句#3中，p2接管string对象的所有权后，p1的所有权将被剥夺。这解决了p1和p2指向同一个对象的问题，但是又会使p1变成空指针，造成隐患。

下面来看使用unique_ptr的情况：

![image-20240814162428554](chapter16.assets/image-20240814162428554.png)

编译器认为语句#6非法，避免了p3不再指向有效数据的问题。因此，unique_ptr比auto_ptr更安全（编译阶段错误比潜在的程序崩溃更安全）

**对于unique_ptr，程序试图将一个unique_ptr赋给另一个时，如果源unique_ptr是个临时右值，编译器允许这样做；如果源unique_ptr将存在一段时间，编译器将禁止这样做**。

![image-20240814162628475](chapter16.assets/image-20240814162628475.png)

![image-20240814162634114](chapter16.assets/image-20240814162634114.png)

demo( )返回一个临时unique_ptr，然后ps接管了原本归返回的unique_ptr所有的对象，而返回的unique_ptr被销毁，没有机会使用它来访问无效的数据。

![image-20240814162812980](chapter16.assets/image-20240814162812980.png)

语句#1将留下悬挂的unique_ptr（pul），这可能导致危害。语句#2不会留下悬挂的、unique_ptr，因为它调用unique_ptr的构造函数，该构造函数创建的临时对象在其所有权转让给pu后就会被销毁。**这也是禁止（只是一种建议，编译器并不禁止）在容器对象中使用auto_ptr，但**
**允许使用unique_ptr的原因**。

如果您可能确实想执行类似于语句#1的操作，C++有一个标准库函数std::move( )，让您能够将一个unique_ptr赋给另一个，它使用了
C++11新增的移动构造函数和右值引用。

![image-20240814163015342](chapter16.assets/image-20240814163015342.png)

> 警告：
>
> - 使用new分配内存时，才能使用auto_ptr和shared_ptr，使用new [ ]分配内存时，不能使用它们；
> - 不使用new分配内存时，不能使用auto_ptr或shared_ptr；
> - 不使用new或new []分配内存时，不能使用unique_ptr。

### 16.2.4 选择智能指针

如果程序要使用多个指向同一个对象的指针，应选择shared_ptr。

这样的情况包括：有一个指针数组，并使用一些辅助指针来标识特定的元素，如最大的元素和最小的元素；两个对象包含都指向第三个对象的指针；STL容器包含指针。

如果程序不需要多个指向同一个对象的指针，则可使用unique_ptr。



## 16.3 标准模板库

STL提供了一组表示容器、迭代器、函数对象和算法的模板

> - STL容器是同质的，即存储的值的类型相同；
>
> - 算法是完成特定任务（如对数组进行排序或在链表中查找特定值）的处方；
>
> - 迭代器能够用来遍历容器的对象，与能够遍历数组的指针类似，是广义指针；
>
> - 函数对象是类似于函数的对象，可以是类对象或函数指针（包括函数名，因为函数名被用作指针）。

STL使得能够构造各种容器（包括数组、队列和链表）和执行各种操作（包括搜索、排序和随机排列）。

### 16.3.1 模板类vector

```cpp
// vect1.cpp -- introducing the vector template
#include <iostream>
#include <string>
#include <vector>

const int NUM = 5;
int main()
{
    using std::vector;
    using std::string;
    using std::cin;
    using std::cout;
    using std::endl;

    vector<int> ratings(NUM);
    vector<string> titles(NUM);
    cout << "You will do exactly as told. You will enter\n"
         << NUM << " book titles and your ratings (0-10).\n";
    int i;
    for (i = 0; i < NUM; i++)
    {
        cout << "Enter title #" << i + 1 << ": ";
        getline(cin,titles[i]);
        cout << "Enter your rating (0-10): ";
        cin >> ratings[i];
        cin.get();
    }
    cout << "Thank you. You entered the following:\n"
          << "Rating\tBook\n";
    for (i = 0; i < NUM; i++)
    {
        cout << ratings[i] << "\t" << titles[i] << endl;
    }
    // cin.get();

    return 0; 
}

```



![image-20240814202031024](chapter16.assets/image-20240814202031024.png)

### 16.3.2 可对矢量执行的操作

所有的STL容器都提供了一些基本方法，其中包括size( )——返回容器中元素数目、swap( )——交换两个容器的内
容、begin( )——返回一个指向容器中第一个元素的迭代器、end( )——返回一个表示超过容器尾的迭代器。

什么是迭代器？它是一个广义指针。事实上，它可以是指针，也可以是一个可对其执行类似指针的操作——如解除引用（如operator*( )）和递增（如operator++( )）——的对象

每个容器类都定义了一个合适的迭代器，该迭代器的类型是一个名为iterator的typedef，其作用域为整个类。例如，要为vector的double类型规范声明一个迭代器，可以这样做：

![image-20240814202221292](chapter16.assets/image-20240814202221292.png)

![image-20240814202304515](chapter16.assets/image-20240814202304515.png)

还有一个C++11自动类型推断很有用的地方,上述语句可用c++自动类型推断实现：

![image-20240814202316455](chapter16.assets/image-20240814202316455.png)

可以用判断begin()和end()是否相等来判断是否迭代到末尾：

![image-20240814202435654](chapter16.assets/image-20240814202435654.png)

- push_back( ):它将元素添加到矢量末尾,会自动增加vector的长度。

- erase( )方法删除矢量中给定区间的元素。

![image-20240814202600831](chapter16.assets/image-20240814202600831.png)

> 上述删除scores [begin,begin+2)区间的元素

- insert( )方法的功能与erase( )相反：从参数1指定的位置开始插入参数2和参数3确定的区间的元素。

![image-20240814202719238](chapter16.assets/image-20240814202719238.png)

### 16.3.3 对矢量可执行的其他操作

下面来看3个具有代表性的STL函数：for_each( )、random_shuffle( )和sort( )。

- for_each( )函数将被指向的函数应用于容器区间中的各个元素。被指向的函数不能修改容器元素的值。可以用for_each( )函数来代替for循环。

![image-20240815100039471](chapter16.assets/image-20240815100039471.png)

替换为：

![image-20240815100049305](chapter16.assets/image-20240815100049305.png)

- Random_shuffle( )函数接受两个指定区间的迭代器参数，并随机排列该区间中的元素

![image-20240815100137665](chapter16.assets/image-20240815100137665.png)

该函数要求容

器类允许随机访问，vector类可以做到这一点。

- sort( )函数也要求容器支持随机访问。该函数有两个版本：

  - 版本1：默认使用存储容器中的类型元素定义的<运算符进行排序

  ![image-20240815101357796](chapter16.assets/image-20240815101357796.png)

  对于对象，可以重载<运算符从而完成排序：

  ![image-20240815101520718](chapter16.assets/image-20240815101520718.png)

  

  - 版本2：支持手动传入排序函数

  ![image-20240815101421436](chapter16.assets/image-20240815101421436.png)

  ![image-20240815101425453](chapter16.assets/image-20240815101425453.png)

### 16.3.4 基于范围的for循环（C++11）

for_each可以用范围for循环来实现：

![image-20240815102004603](chapter16.assets/image-20240815102004603.png)



![image-20240815102008656](chapter16.assets/image-20240815102008656.png)

for_each不能修改容器的内容，但是范围for循环可以：通过指定一个引用参数。

![image-20240815102057467](chapter16.assets/image-20240815102057467.png)

![image-20240815102101916](chapter16.assets/image-20240815102101916.png)

## 16.4 泛型编程

STL是一种泛型编程（generic programming）

面向对象编程关注的是编程的数据方面，而泛型编程关注的是算法。它们之间的共同点是抽象和创建可重用代码，但它们的理念绝然不同。

### 16.4.1 为何使用迭代器

为了解为何需要迭代器，我们来看如何为两种不同数据表示实现find函数，然后来看如何推广这种方法。

首先看一个在double数组中搜索特定值的函数，可以这样编写该函数：

![image-20240815104646651](chapter16.assets/image-20240815104646651.png)

泛型编程旨在使用同一个find函数来处理数组、链表或任何其他容器类型。即函数不仅独立于容器中存储的数据类型，而且独立于容器本身的数据结构。

模板提供了存储在容器中的数据类型的通用表示，因此还需要遍历容器中的值的通用表示，迭代器正是这样的通用表示。

迭代器应具备哪些特征呢？

- 能够执行解引用操作：应对*p进行定义
- 能够将一个迭代器赋给另一个：应对表达式p=q进行定义。
- 能够执行比较操作，判断是否相等：应对p= =q和p!=q进行定义。
- 能通过迭代遍历容器所有元素：为迭代器p定义++p和p++来实现。

常规指针就能满足迭代器的要求，因此，可以这样重新编写find_arr( )函数：

![image-20240816112648352](chapter16.assets/image-20240816112648352.png)



可以定义一个迭代器类，其中定义了运算符*和++：

![image-20240815105545716](chapter16.assets/image-20240815105545716.png)

STL遵循上面介绍的方法。首先，每个容器类（vector、list、deque等）定义了相应的迭代器类型。对于其中的某个类，迭代器可能是指针；而对于另一个类，则可能是对象。

不管实现方式如何，迭代器都将提供所需的操作，如*和++（有些类需要的操作可能比其他类多）。其次，每个容器类都有一个超尾标记，当迭代器递增到超越容器的最后一个值后，这个值将被赋给迭代器。

每个容器类都有begin( )和end( )方法，它们分别返回一个指向容器的第一个元素和超尾位置的迭代器

每个容器类都使用++操作，让迭代器从指向第一个元素逐步指向超尾位置，从而遍历容器中的每一个元素。

来总结一下STL方法。首先是处理容器的算法，应尽可能用通用的术语来表达算法，使之独立于数据类型和容器类型。为使通用算法能够适用于具体情况，应定义能够满足算法需求的迭代器，并把要求加到容器设计上。即基于算法的要求，设计基本迭代器的特征和容器特征。

### 16.4.2 迭代器类型

STL定义了5种迭代器，并根据所需的迭代器类型对算法进行了描述。这5种迭代器分别是输入迭代器、输出迭代器、正向迭代器、双向迭代器和随机访问迭代器。

例如，find()的原型与下面类似：

![image-20240815110929380](chapter16.assets/image-20240815110929380.png)

下面的原型指出排序算法需要一个随机访问迭代器：

![image-20240815110950969](chapter16.assets/image-20240815110950969.png)

5中迭代器的通用操作：

- 解除引用
- ==
- !=

注意：判断迭代器指向元素是否相等可以直接通过比较迭代器实现。

![image-20240815111205996](chapter16.assets/image-20240815111205996.png)

和下面的是等价的

![image-20240815111222388](chapter16.assets/image-20240815111222388.png)

#### 1．输入迭代器

输入迭代器可被程序用来读取容器中的信息

对输入迭代器解除引用将使程序能够读取容器中的值，但不一定能让程序修改值。因此，需要输入迭代器的算法将不会修改容器中的值。

**注意，输入迭代器是单向迭代器，可以递增，但不能倒退。**

#### 2．输出迭代器

 输出迭代器与输入迭代器相似，只是解除引用让程序能修改容器值，而不能读取。

发送到显示器上的输出就是如此，cout可以修改发送到显示器的字符流，却不能读取屏幕上的内容。

**简而言之，对于单通行、只读算法，可以使用输入迭代器；而对于单通行、只写算法，则可以使用输出迭代器。**

#### 3．正向迭代器

与输入迭代器和输出迭代器相似，正向迭代器只使用++运算符来遍历容器，所以它每次沿容器向前移动一个元素

另外，将正向迭代器递增后，仍然可以对前面的迭代器值解除引用（如果保存了它），并可以得到相同的值。这些特征使得多次通行算法成为可能。

正向迭代器既可以使得能够读取和修改数据，也可以使得只能读取数据：

#### 4．双向迭代器

双向迭代器具有正向迭代器的所有特性，同时支持两种（前缀和后缀）递减运算符。

#### 5．随机访问迭代器

有些算法（如标准排序和二分检索）要求能够直接跳到容器中的任何一个元素，这叫做随机访问，需要随机访问迭代器。随机访问迭代器具有双向迭代器的所有特性，同时添加了支持随机访问的操作（如指针增加运算）和用于对元素进行排序的关系运算符

![image-20240815115714393](chapter16.assets/image-20240815115714393.png)

### 16.4.3 迭代器层次结构

正向迭代器具有输入迭代器和输出迭代器的全部功能，同时还有自己的功能；双向迭代器具有正向迭代器的全部功能，同时还有自己的功能；随机访问迭代器具有正向迭代器的全部功能，同时还有自己的功能

![image-20240815120012076](chapter16.assets/image-20240815120012076.png)

为何需要这么多迭代器呢？目的是为了在编写算法尽可能使用要求最低的迭代器，并让它适用于容器的最大区间

注意，各种迭代器的类型并不是确定的，而只是一种概念性描述。正如前面指出的，每个容器类都定义了一个类级typedef名称——iterator

### 16.4.4 概念、改进和模型

STL文献使用术语概念（concept）来描述一系列的要求。因此，存在输入迭代器概念、正向迭代器概念，等等。

概念可以具有类似继承的关系。例如，双向迭代器继承了正向迭代器的功能。有些STL文献使用术语改进（refinement）来表示这种概念上的继承，因此，双向迭代器是对正向迭代器概念的一种改进。

概念的具体实现被称为模型（model）。

#### 1．将指针用作迭代器

STL算法可以使用指针来对基于指针的非STL容器进行操作。例如，可将STL算法用于数组。

![image-20240815144410922](chapter16.assets/image-20240815144410922.png)

![image-20240815144414524](chapter16.assets/image-20240815144414524.png)

C++支持将超尾概念用于数组，使得可以将STL算法用于常规数组。由于指针是迭代器，而算法是基于迭代器的，这使得可将STL算法用于常规数组

STL提供了一些预定义迭代器。

为了解其中的原因，这里先介绍一些背景知识。有一种算法（名为copy( )）可以将数据从一个容器复制到另一个容器中。

例如，下面的代码将一个数组复制到一个矢量中：

![image-20240815153442058](chapter16.assets/image-20240815153442058.png)

Copy( )函数将覆盖目标容器中已有的数据，同时目标容器必须足够大，以便能够容纳被复制的元素

现在，假设要将信息复制到显示器上。如果有一个表示输出流的迭代器，则可以使用copy( )。

STL为这种迭代器提供了ostream_iterator模板。用STL的话说，**该模板是输出迭代器概念的一个模型，它也是一个适配器（adapter）——一个类或函数，可以将一些其他接口转换为STL使用的接口。**可以通过包含头文件iterator（以前为iterator.h）并作下面的声明来创建这种迭代器：

![image-20240815161550622](chapter16.assets/image-20240815161550622.png)

第一个模板参数（这里为int）指出了被发送给输出流的数据类型；第二个模板参数（这里为char）指出了输出流使用的字符类型（另一个可能的值是wchar_t）。构造函数的第一个参数（这里为cout）指出了要使用的输出流，它也可以是用于文件输出的流（参见第17章）；最后一个字符串参数是在发送给输出流的每个数据项后显示的分隔符。

![image-20240815161822004](chapter16.assets/image-20240815161822004.png)

这意味着将dice容器的整个区间复制到输出流中，即显示容器的内容。

也可以不创建命名的迭代器，而直接构建一个匿名迭代器。即可以这样使用适配器：

![image-20240815161900278](chapter16.assets/image-20240815161900278.png)

iterator头文件还定义了一个istream_iterator模板：

![image-20240815161917294](chapter16.assets/image-20240815161917294.png)

与ostream_iterator相似，istream_iterator也使用两个模板参数。第一个参数指出要读取的数据类型，第二个参数指出输入流使用的字符类型。**使用构造函数参数cin意味着使用由cin管理的输入流，省略构造函数参数表示输入失败**，因此上述代码从输入流中读取，直到文件结尾、类型不匹配或出现其他输入故障为止。

#### 2．其他有用的迭代器

除了ostream_iterator和istream_iterator之外，头文件iterator还提供了其他一些专用的预定义迭代器类型。它们是reverse_iterator、back_insert_iterator、front_insert_iterator和insert_iterator。

- reverse -iterator

对reverse_iterator执行递增操作将导致它被递减。

为什么不直接对常规迭代器进行递减呢？主要原因是为了简化对已有的函数的使用。

例如要将vector中数据反向输出到cout中。

![image-20240815162330903](chapter16.assets/image-20240815162330903.png)

vector类有一个名为rbegin( )的成员函数和一个名为rend( )的成员函数，前者返回一个指向超尾的反向迭代器，后者返回一个指向第一个元素的反向迭代器。

>  rbegin( )和end( )返回相同的值（超尾），但类型不同（reverse_iterator和iterator）。同样，rend( )和begin( )也返回相同的值（指向第一个元素的迭代器），但类型不同。

直接对反向指针解引用实际上会**先递减，再解除引用**解决了rbegin()无法直接解引用返回末尾元素的问题。

> 假设rp是一个被初始化为dice.rbegin( )的反转指针。如果rp指向位置6，则*rp将是位置5的值

另外三种迭代器（back_insert_iterator、front_insert_iterator和insert_iterator）也将提高STL算法的通用性

copy( )不能自动根据发送值调整目标容器的长度，如果预先不知道被拷入的目标容器的大小，多余的元素就会被丢弃。

三种插入迭代器通过将复制转换为插入解决了这些问题。

back_insert_iterator将元素插入到容器尾部，而front_insert_iterator将元素插入到容器的前端。最后，insert_iterator将元素插入到insert_iterator构造函数的参数指定的位置前面。

back_insert_iterator只能用于允许在尾部快速插入的容器；vector类符合这种要求

front_insert_iterator只能用于允许在起始位置做时间固定插入的容器类型，vector类不能满足这种要求，但queue满足

insert_iterator没有这些限制，因此可以用它把信息插入到矢量的前端

> 可以用insert_iterator将复制数据的算法转换为插入数据的算法。

dice的vector<int>容器创建一个back_insert_iterator，可以这样做：

![image-20240815164348499](chapter16.assets/image-20240815164348499.png)

声明front_insert_iterator的方式与此相同。对于insert_iterator声明，还需一个指示插入位置的构造函数参数：

![image-20240815164432322](chapter16.assets/image-20240815164432322.png)

```cpp
//
// Created by wWX1283744 on 2024/8/15.
//
#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>

void output(const std::string &s) { std::cout << s << " "; };
int main(){
    using namespace std;
    string s1[4] = {"fine", "fish", "fashion", "fate"};
    string s2[2] = {"busy", "bats"};
    string s3[2] = {"silly", "singers"};
    vector<string> words(4) ;

    copy(s1,s1+4,words.begin());
    for_each(words.begin(),words.end(), output);//fine fish fashion fate
    cout<<endl;

    copy(s2,s2+2,back_insert_iterator<vector<string>>(words));
    for_each(words.begin(),words.end(), output);//fine fish fashion fate busy bats
    cout<<endl;
    copy(s3,s3+2,insert_iterator<vector<string>>(words,words.begin()));

    for_each(words.begin(),words.end(), output);//silly singers fine fish fashion fate busy bats
    cout<<endl;
    return 0;
}
```

因此，copy( )不仅可以将信息从一个容器复制到另一个容器，还可以将信息从容器复制到输出流，从输入流复制到容器中。还可以使用
copy( )将信息插入到另一个容器中。之所以copy有这么多功能，都是迭代器赋予他的功能。





### 16.4.5 容器种类

以前的11个容器类型分别是deque、list、queue、priority_queue、stack、vector、map、multimap、set、multiset和
bitset（本章不讨论bitset，它是在比特级处理数据的容器）；C++11新增了forward_list、unordered_map、unordered_multimap、unordered_set和unordered_multiset，且不将bitset视为容器，而将其视为一种独立的类别

#### 1．容器概念

容器概念指定了所有STL容器类都必须满足的一系列要求

容器是存储其他对象的对象（可以是类对象，也可以是基本类型），存储在容器中的数据为容器所有，这意味着当容器过期时，存储在容器中的数据也将过期。（然而，如果数据是指针的话，则它指向的数据并不一定过期）。

支持复制构造和可赋值的类型才能存储到容器中。基本类型满足这些要求；只要类定义没有将复制构造函数和赋值运算符声明为私有或
保护的，则也满足这种要求。

>  X表示容器类型，如vector；T表示存储在容器中的对象类型；a和b表示类型为X的值；r表示类型为X&的值；u表示类型为X的标识符（即如果X表示vector<int>，则u是一个vector<int>对象）

![image-20240815172717729](chapter16.assets/image-20240815172717729.png)

#### 2．C++11新增的容器要求

在这个表中，rv表示类型为X的非常量右值，如函数的返回值

![image-20240815172656602](chapter16.assets/image-20240815172656602.png)

复制构造和复制赋值以及移动构造和移动赋值之间的差别在于，复制操作保留源对象，而移动操作可修改源对象，还可能转让所有权，而不做任何复制。如果源对象是临时的，移动操作的效率将高于常规复

#### 3．序列

可以通过添加要求来改进基本的容器概念。序列（sequence）是一种重要的改进，因为7种STL容器类型（deque、C++11新增的forward_list、list、queue、priority_queue、stack和vector）都是序列

**序列概念增加了迭代器至少是正向迭代器这样的要求，这保证了元素将按特定顺序排列，不会在两次迭代之间发生变化。**

array也被归类到序列容器，虽然它并不满足序列的所有要求。

序列还要求其元素按严格的线性顺序排列，因此可以执行诸如将值插入到特定位置、删除特定区间等操作。

序列的基本要求

![image-20240816091527914](chapter16.assets/image-20240816091527914.png)

![image-20240816091535284](chapter16.assets/image-20240816091535284.png)

因为模板类deque、list、queue、priority_queue、stack和vector都是序列概念的模型，所以它们都支持表16.7所示的运算符。除此之外，这6个模型中的一些还可使用其他操作。

在允许的情况下，它们的复杂度为固定时间。表16.8列出了其他操作。

![image-20240816091627631](chapter16.assets/image-20240816091627631.png)

首先，a[n]和a.at(n)都返回一个指向容器中第n个元素（从0开始编号）的引用。它们之间的差别在于，如果n落在容器的有效区间外，则a.at(n)
将执行边界检查，并引发out_of_range异常

下面详细介绍这7种序列容器类型。

#### （1）vector

- vector是数组的一种类表示，它提供了自动内存管理功能
- 它提供了对元素的随机访问
- 在尾部添加和删除元素的时间是固定的，但在头部或中间插入和删除元素的复杂度为线性时间

- vector还是可反转容器（reversible container）概念的模型

> 这增加了两个类方法：rbegin( )和rend( )

- 应默认使用这种类型

#### （2）deque

deque模板类（在deque头文件中声明）表示双端队列（double-ended queue），通常被简称为deque。

在STL中，其实现类似于vector容器，支持随机访问。**主要区别在于，从deque对象的开始位置插入和删除元素的时间是固定的，而不像vector中那样是线性时间的。**

**所以，如果多数操作发生在序列的起始和结尾处，则应考虑使用deque数据结构。**
为实现在deque两端执行插入和删除操作的时间为固定的这一目的，deque对象的设计比vector对象更为复杂。因此，尽管二者都提供对元素的随机访问和在序列中部执行线性时间的插入和删除操作，但vector容器执行这些操作时速度要快些。

#### （3）list

list模板类（在list头文件中声明）表示**双向链表。**

list和vector之间关键的区别在于**，list在链表中任一位置进行插入和删除的时间都是固定的**（vector模板提供了除结尾处外的线性时间的插入和删除，在结尾处，它提供了固定时间的插入和删除）

**vector强调的是通过随机访问进行快速访问，而list强调的是元素的快速插入和删除。**

list模板类还包含了链表专用的成员函数

![image-20240816092444828](chapter16.assets/image-20240816092444828.png)

```cpp
//
// Created by wWX1283744 on 2024/8/16.
//
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
void outint(int n) {std::cout << n << " ";}

int main(){
    using namespace std;

    list<int> one(5,2);

    int stuff[5] = {1,2,3,4,5};
    list<int> two;
    two.insert(two.begin(),stuff,stuff+2);
    int more[6] = {6, 4, 2, 4, 6, 5};
    list<int> three(two);
    three.insert(three.end(),more,more+6 );

    cout << "List one: ";
    for_each(one.begin(),one.end(), outint);
    cout << endl << "List two: ";
    for_each(two.begin(), two.end(), outint);
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 1 2 6 4 2 4 6 5

    three.remove(2);
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 1 6 4 4 6 5

    three.splice(three.begin(),one);
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 2 2 2 2 2 1 6 4 4 6 5
    three.unique();
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 2 1 6 4 6 5
    three.sort();
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 1 2 4 5 6 6
    three.merge(two);
    cout << endl << "List three: ";
    for_each(three.begin(), three.end(), outint);//List three: 1 1 2 2 4 5 6 6
    cout << endl << "List two: ";
    for_each(two.begin(), two.end(), outint);//List two:
    return 0;
}
```

> insert( )和splice( )之间的主要区别在于：insert( )将原始区间的副本插入到目标地址，而splice( )则将原始区间移到目标地址
>
> splice( )方法执行后，迭代器仍有效。也就是说，如果将迭代器设置为指向one中的元素，则在splice( )将它重新定位到元素three后，该迭代器仍然指向相同的元素。
>
> unique( )只能将相邻的相同值压缩为单个值。但应用sort( )后再应用unique( )时，每个值将只占一个位置。
>
> 非成员sort( )函数（程序清单16.9），但它需要随机访问迭代器。所以不能将非成员函数sort( )用于链表。因此，这个类中包括了一个只能在类中使用的成员版本。

#### （4）list工具箱

sort( )、merge( )和unique( )方法还各自拥有接受另一个参数的版本，该参数用于指定用来比较元素的函数。同样，remove( )方法也有一个接受另一个参数的版本，该参数用于指定用来确定是否删除元素的函数。这些参数都是谓词函数

#### （5）forward_list（C++11）

C++11新增了容器类forward_list，它实现了单链表

因此forward_list只需要正向迭代器，因此，不同于vector和list，forward_list是不可反转的容器。

相比于list，forward_list更简单、更紧凑，但功能也更少。

#### （6）queue

queue模板类（在头文件queue（以前为queue.h）中声明）是一个适配器类。由前所述，ostream_iterator模板就是一个适配器，让输出流能够使用迭代器接口。同样，queue模板让底层类（默认为deque）展示典型的队列接口。

queue模板的限制比deque更多，支持持下面所示的操作：

![image-20240820172935701](chapter16.assets/image-20240820172935701.png)

#### （8）priority_queue

priority_queue模板类（在queue头文件中声明）是另一个适配器类，它支持的操作与queue相同。

两者之间的主要区别在于，在priority_queue中，最大的元素被移到队首（生活不总是公平的，队列也一样）。内部区别在于，默认的底层类是vector。

可以修改用于确定哪个元素放到队首的比较方式，方法是提供一个可选的构造函数参数：

![image-20240820173215903](chapter16.assets/image-20240820173215903.png)

greater< >( )函数是一个预定义的函数对象，本章稍后将讨论它。

#### （9）stack

与queue相似，stack（在头文件stack——以前为stack.h——中声明）也是一个适配器、类，它给底层类（默认情况下为vector）提供了典型的栈接口。

stack模板的限制比vector更多。

![image-20240820173320488](chapter16.assets/image-20240820173320488.png)

#### （10）array（C++11）



### 16.4.4 关联容器

关联容器（associative container）是对容器概念的另一个改进。关联容器将值与键关联在一起，并使用键来查找值

关联容器的优点在于，它提供了对元素的快速访问。

STL提供了4种关联容器：set、multiset、map和multimap。前两种是在头文件set（以前分别为set.h和multiset.h）中定义的，而后两种是在头文件map（以前分别为map.h和multimap.h）中定义的。

最简单的关联容器是set，其值类型与键相同，键是唯一的，这意味着集合中不会有多个相同的键。

multiset类似于set，只是可能有多个值的键相同

在map中，值与键的类型不同，键是唯一的，每个键只对应一个值。multimap与map相似，只是一个键可以与多个值相关联。

#### 1．set示例

set模拟了多个概念，它是关联集合，可反转，可排序，且键是唯一的，所以不能存储多个相同的值

set也使用模板参数来指定存储的值类型：

![image-20240820195523624](chapter16.assets/image-20240820195523624.png)

第二个模板参数是可选的，可用于指示用来对键进行排序的比较函数或对象。默认情况下，将使用模板less< >（稍后将讨论）。老式C++实现可能没有提供默认值，因此必须显式指定模板参数：

![image-20240820195604036](chapter16.assets/image-20240820195604036.png)

```cpp
const int N = 5;
string s1[N] = {"22","33","11","22","44"};
set<string> A(s1,s1+N);
copy(A.begin(),A.end(),ostream_iterator<string,char>(cout," "));//11 22 33 44
```

数学为集合定义了一些标准操作，例如，并集包含两个集合合并后的内容。交集包含两个集合都有的元素。两个集合的差是第一个集合减去两个集合都有的元素。

STL提供了支持（交集，并集和差集）这些操作的算法。它们是通用函数，因此并非只能用于set对象。然而，所有set对象都自动满足使用这些算法的先决条件，即容器是经过排序的。

set_union( )函数用于求两个集合的并集，例如，要显示集合A和B的并集，可以这样做：

> 前两个迭代器定义了第一个集合的区间，接下来的两个定义了第二个集合区间，最后一个迭代器是输出迭代器，指出将结果集合复制到
> 什么位置。

![image-20240820200428812](chapter16.assets/image-20240820200428812.png)

假设要将结果放到集合C中，而不是显示它，则最后一个参数应是一个指向C的迭代器。显而易见的选择是C.begin( )，但它不管用，原因有两个。

- 首先，关联集合将键看作常量，所以C.begin( )返回的迭代器是常量迭代器，不能用作输出迭代器。

- 不直接使用C.begin( )的第二个原因是，与copy( )相似，set_union( )将覆盖容器中已有的数据，并要求容器有足够的空间容纳新信息。C是空的，不能满足这种要求。

但前面讨论的模板insert_iterator可以解决这两个问题。前面说过，它可以将复制转换为插入。另外，它还模拟了输出迭代器概念，可以用它将信息写入容器。因此，可以创建一个匿名insert_iterator，将信息复制给C。

![image-20240820200712492](chapter16.assets/image-20240820200712492.png)

**函数set_intersection( )和set_difference( )分别查找交集和获得两个集合的差，它们的接口与set_union( )相同。**

两个有用的set方法是lower_bound( )和upper_bound( )。

- ower_bound( )将键作为参数并返回一个迭代器，该迭代器指向集合中第一个不小于键参数的成员

- upper_bound( )将键作为参数，并返回一个迭代器，该迭代器指向集合中第一个大于键参数的成员。

使用这两个函数可以确定一个set集合中某个值区间的所有元素。

对于set的插入操作，因为排序决定了插入的位置，所以这种类包含只指定要插入的信息，而不指定位置的插入方法。例如，如果A和B是字符串集合，则可以这样做：

![image-20240820201220851](chapter16.assets/image-20240820201220851.png)

#### 2．multimap示例

与set相似，multimap也是可反转的、经过排序的关联容器，但键和值的类型不同，且同一个键可能与多个值相关联。

multimap声明：

例如，下面的声明创建一个multimap对象，其中键类型为int，存储的值类型为string：

![image-20240821093955120](chapter16.assets/image-20240821093955120.png)

第3个模板参数是可选的，指出用于对键进行排序的比较函数或对象。在默认情况下，将使用模板less< >（稍后将讨论），该模板将键类型作为参数。老式C++实现可能要求显式指定该模板参数。

为将信息结合在一起，实际的值类型将键类型和数据类型结合为一对。为此，STL使用模板类pair<class T, class U>将这两种值存储到一个对象中。

例如，假设要用区号作为键来存储城市名（这恰好与codes声明一致，它将键类型声明为int，数据类型声明为string），则一种方法是创建一个pair，再将它插入：

![image-20240821094218614](chapter16.assets/image-20240821094218614.png)

也可使用一条语句创建匿名pair对象并将它插入：

![image-20240821094237832](chapter16.assets/image-20240821094237832.png)

因为数据项是按键排序的，所以不需要指出插入位置。

对于pair对象，可以使用first和second成员来访问其两个部分了：

![image-20240821094307530](chapter16.assets/image-20240821094307530.png)

遍历multiset

```cpp
typedef std::multimap<KeyType, std::string> MapCode;
MapCode::iterator it;
for (it = codes.begin(); it != codes.end(); ++it)
    cout << "    " << (*it).first << "     "
    << (*it).second    << endl;
```



如何获得有关multimap对象的信息呢？

- 成员函数count( )接受键作为参数，并返回具有该键的元素数目。
- 成员函数equal_range( )用键作为参数，且返回两个迭代器，它们表示的区间与该键匹配。为返回两个值，该方法将它们封装在一个pair对象中，这里pair的两个模板参数都是迭代器。

![image-20240821095402420](chapter16.assets/image-20240821095402420.png)

可以使用c++11自动类型推断进行简化

![image-20240821095423607](chapter16.assets/image-20240821095423607.png)



### 16.4.5 无序关联容器（C++11）

无序关联容器是对容器概念的另一种改进。旨在提高添加和删除元素的速度以及提高查找算法的效率

底层的差别在于，关联容器是基于树结构的，而无序关联容器是基于数据结构哈希表的

有4种无序关联容器，它们是unordered_set、unordered_multiset、unordered_map和unordered_multimap



## 16.5 函数对象

很多STL算法都使用函数对象——也叫函数符（functor）

函数符是可以以函数方式与( )结合使用的任意对象。这包括**函数名、指向函数的指针和重载了( )运算符的类对象**（即定义了函数operator( )( )的类）。

例如，可以像这样定义一个类：

![image-20240821101933803](chapter16.assets/image-20240821101933803.png)

这样，重载的( )运算符将使得能够像函数那样使用Linear对象：

![image-20240821101958271](chapter16.assets/image-20240821101958271.png)

还记得函数for_each吗？它将指定的函数用于区间中的每个成员：

![image-20240821102413693](chapter16.assets/image-20240821102413693.png)

如何声明第3个参数呢？STL通过使用模板解决了这个问题。for_each的原型看上去就像这样：

![image-20240821102456817](chapter16.assets/image-20240821102456817.png)

### 16.5.1 函数符概念

正如STL定义了容器和迭代器的概念一样，它也定义了函数符概念。

- 生成器（generator）是不用参数就可以调用的函数符。
- 一元函数（unary function）是用一个参数可以调用的函数符。
- 二元函数（binary function）是用两个参数可以调用的函数符。

当然，这些概念都有相应的改进版：

- 返回bool值的一元函数是谓词（predicate）；
- 返回bool值的二元函数是二元谓词（binary predicate）。

一些STL函数需要谓词参数或二元谓词参数。

例如，sort( )的这样一个版本，即将二元谓词作为其第3个参数：

![image-20240821105720077](chapter16.assets/image-20240821105720077.png)

list模板有一个将谓词作为参数的remove_if( )成员，该函数将谓词应用于区间中的每个元素，如果谓词返回true，则删除这些元素。

例如，下面的代码删除链表three中所有大于100的元素：

![image-20240821105751094](chapter16.assets/image-20240821105751094.png)

类函数的应用场景：假设要删除另一个链表中所有大于200的值。如果需要将200也作为一个可以配置的值，由于remove_if只接受谓词函数（一个参数），故可以使用类函数，用构造函数来确定要删除的上限值。

![image-20240821110204617](chapter16.assets/image-20240821110204617.png)

使用格式如下：

![image-20240821110234841](chapter16.assets/image-20240821110234841.png)

假设已经有了一个接受两个参数的模板函数：

![image-20240821110419807](chapter16.assets/image-20240821110419807.png)

则可以使用类将它转换为单个参数的函数对象：

![image-20240821110431618](chapter16.assets/image-20240821110431618.png)

即可以这样做：

![image-20240821110458761](chapter16.assets/image-20240821110458761.png)

简而言之，类函数符TooBig2是一个函数适配器，使函数能够满足不同的接口。

使用C++11的初始化列表功能来简化初始化：

![image-20240821110607140](chapter16.assets/image-20240821110607140.png)

可简化为：

![image-20240821110616085](chapter16.assets/image-20240821110616085.png)

### 16.5.2 预定义的函数符

STL定义了多个基本函数符，它们执行诸如将两个值相加、比较两个值是否相等操作。提供这些函数对象是为了支持将函数作为参数的STL函数。

为了支持将函数作为参数的STL函数，STL定义了多个基本函数符。

例如函数transform()，它有两个版本。第一个版本接受4个参数，前两个参数是指定容器区间的迭代器（现在您应该已熟悉了这种方法），第3个参数是指定将结果复制到哪里的迭代器，最后一个参数是一个函数符，它被应用于区间中的每个元素，生成结果中的新元素。例如，请看下面
的代码：

![image-20240821110943170](chapter16.assets/image-20240821110943170.png)

上述代码计算每个元素的平方根，并将结果发送到输出流。

目标迭代器可以位于原始区间中。例如，将上述示例中的out替换为gr8.begin( )后，新值将覆盖原来的值。

第2种版本使用一个接受两个参数的函数：

```cpp
vector<int> gr8 = {1, 2, 3, 4, 5};
vector<double>m8 = {1,2,3};
transform(gr8.begin(),gr8.end(),m8.begin(),out, mean);//1 2 3 2 2.5
cout<<endl;
vector<double>m9{1,2,3,4,5};
transform(gr8.begin(),gr8.end(),m9.begin(),out, mean);//1 2 3 4 5
cout<<endl;
vector<double>m10{1,2,3,4,5,6};
transform(gr8.begin(),gr8.end(),m10.begin(),out, mean);//1 2 3 4 5
```



如果要实现两个容器元素相加，可以定义一个add函数。

![image-20240821113342755](chapter16.assets/image-20240821113342755.png)

然而，这样必须为每种类型单独定义一个函数。更好的办法是定义一个模板。头文件functional（以前为function.h）定义了
多个模板类函数对象，其中包括plus< >( )。

可以用plus< >类来完成常规的相加运算：
![image-20240821113436587](chapter16.assets/image-20240821113436587.png)

它使得将函数对象作为参数很方便：

![image-20240821113528459](chapter16.assets/image-20240821113528459.png)



对于所有内置的算术运算符、关系运算符和逻辑运算符，STL都提供了等价的函数符。表16.12列出了这些函数符的名称。

![image-20240821113558165](chapter16.assets/image-20240821113558165.png)

![image-20240821113607341](chapter16.assets/image-20240821113607341.png)

### 16.5.3 自适应函数符和函数适配器

表16.12列出的预定义函数符都是自适应的。

实际上STL有5个相关的概念：自适应生成器（adaptable generator）、自适应一元函数（adaptable unary function）、自适应二元函
数（adaptable binary function）、自适应谓词（adaptable predicate）和自适应二元谓词（adaptable binary predicate）。

使函数符成为自适应的原因是，它携带了标识参数类型和返回类型的typedef成员。这些成员分别是result_type、first_argument_type和second_argument_type。

例如，plus<int>对象的返回类型被标识为plus<int>::result_type，这是int的typedef。

函数符自适应性的意义在于：函数适配器对象可以使用函数对象，并认为存在这些typedef成员。例如，接受一个自适应函数符参数的函数可以使用result_type成员来声明一个与函数的返回类型匹配的变量。

STL提供了使用这些工具的函数适配器类。例如，假设要将矢量gr8的每个元素都增加2.5倍，则需要使用接受一个一元函数参数的transform( )版本，就像前面的例子那样：

![image-20240821114439004](chapter16.assets/image-20240821114439004.png)

multiplies( )函数符可以执行乘法运行，但它是二元函数。因此需要一个函数适配器，将接受两个参数的函数符转换为接受1个参数的函数符。

前面的TooBig2示例提供了一种方法，但STL使用binder1st和binder2nd类自动完成这一过程，它们将自适应二元函数转换为自适应一元函数。

来看binder1st。假设有一个自适应二元函数对象f2( )，则可以创建一个binder1st对象，该对象与一个将被用作f2( )的第一个参数的特定值（val）相关联：

![image-20240821114551417](chapter16.assets/image-20240821114551417.png)

这样，使用单个参数调用f1(x)时，返回的值与将val作为第一参数、将f1( )的参数作为第二参数调用f2( )返回的值相同。

即：f1(x) = f2(val,x),只是前者是一元函数，而不是二元函数。f2( )函数被适配。同样，仅当f2( )是一个自适应函数时，这才能实现。

例如，要将二元函数multiplies( )转换为将参数乘以2.5的一元函数，则可以这样做：

![image-20240821115037944](chapter16.assets/image-20240821115037944.png)

因此，将gr8中的每个元素与2.5相乘，并显示结果的代码如下：

![image-20240821115110008](chapter16.assets/image-20240821115110008.png)

binder2nd类与此类似，只是将常数赋给第二个参数，而不是第一个参数。它有一个名为bind2nd的助手函数，该函数的工作方式类似于bind1st。

程序清单16.16 funadap.cpp

```cpp
//
// Created by wWX1283744 on 2024/8/21.
//

#include <iostream>
#include <vector>
#include <algorithm>

const int LIM = 6;

void Show(double);

int main(){
    using namespace std;

    vector<double> gr8 = {28, 29, 30, 35, 38, 59};
    vector<double> m8 = {63, 65, 69, 75, 80, 99};

    cout.setf(ios_base::fixed);
    cout.precision(1);

    cout << "gr8:\t";
    for_each(gr8.begin(),gr8.end(), Show);
    cout<<endl;
    cout << "m8: \t";
    for_each(m8.begin(),m8.end(), Show);
    cout<<endl;

    vector<double> sum(LIM);
    transform(gr8.begin(),gr8.end(),m8.begin(),sum.begin(),plus<>());

    cout << "sum:\t";
    for_each(sum.begin(), sum.end(), Show);
    cout << endl;

    vector<double> prod(LIM);
//    transform(gr8.begin(),gr8.end(),prod.begin(), bind1st(multiplies<double>(),2.5));
    transform(gr8.begin(),gr8.end(),prod.begin(), binder1st(multiplies<double>(),2.5));
    cout << "prod:\t";
    for_each(prod.begin(), prod.end(), Show);
    cout << endl;



    return 0;
}

void Show(double v)
{
    std::cout.width(6);
    std::cout << v << ' ';
}

```

运行结果：

```
gr8:      28.0   29.0   30.0   35.0   38.0   59.0
m8:       63.0   65.0   69.0   75.0   80.0   99.0
sum:      91.0   94.0   99.0  110.0  118.0  158.0
prod:     70.0   72.5   75.0   87.5   95.0  147.5

Process finished with exit code 0
```

C++11提供了函数指针和函数符的替代品——lambda表达式,这将在第18章讨论



## 16.6 算法

对于算法函数设计，有两个主要的通用部分。

- 首先，它们都使用模板来提供泛型；
- 其次，它们都使用迭代器来提供访问容器中数据的通用表示

因为指针是一种特殊的迭代器，因此诸如copy( )等STL函数可用于常规数组。

统一的容器设计使得不同类型的容器之间具有明显关系。

例如，可以使用copy( )将常规数组中的值复制到vector对象中，将vector对象中的值复制到list对象中，将list对象中的值复制到set对象中。可以用= =来比较不同类型的容器，如deque和vector。**之所以能够这样做，是因为容器重载的= =运算符使用迭代器来比较内容**，因此如果deque对象和vector对象的内容相同，并且排列顺序也相同，则它们是相等的。

### 16.6.1 算法组

STL将算法库分成4组：

- 非修改式序列操作：操作区间每个元素，不修改元素。如find，for_each
- 修改式序列操作：操作区间每个元素，可修改值，也可修改排序。如transform，random_shuffle，copy。
- 排序和相关操作：包括多个排序函数（包括sort( )）和其他各种函数，包括集合操作
- 通用数字运算：vector最有可能使用这些操作。

前3组在头文件algorithm（以前为algo.h）中描述，第4组是专用于数值数据的，有自己的头文件，称为numeric（以前它们也位于algol.h中

### 16.6.2 算法的通用特征

STL函数使用迭代器和迭代器区间。从函数原型可知有关迭代器的假设。例如，copy( )函数的原型如下：

![image-20240823113932037](chapter16.assets/image-20240823113932037.png)

上述声明告诉我们，区间参数必须是输入迭代器或更高级别的迭代器，而指示结果存储位置的迭代器必须是输出迭代器或更高级别的迭代器。

对算法进行分类的方式之一是按结果放置的位置进行分类。有些算法就地完成工作，有些则创建拷贝。

> 例如，在sort( )函数完成时，结果被存放在原始数据的位置上，因此，sort( )是就地算法（in-place algorithm）；而copy( )函数将结果发送到另一个位置，所以它是复制算法（copying algorithm）。transform( )函数可以以这两种方式完成工作。与copy()相似，它使用输出迭代器指示结果的存储位置；与copy( )不同的是，transform( )允许输出迭代器指向输入区间，因此它可以用计算结果覆盖原来的值。

有些算法有两个版本：就地版本和复制版本。STL的约定是，复制版本的名称将以_copy结尾。复制版本将接受一个额外的输出迭代器参数，该参数指定结果的放置位置。

> 例如，函数replace( )的原型如下：
>
> ![image-20240823114146605](chapter16.assets/image-20240823114146605.png)
>
> 它将所有的old_value替换为new_value，这是就地发生的。由于这种算法同时读写容器元素，因此迭代器类型必须是ForwardIterator或更高级别的。
>
> 复制版本的原型如下：
>
> ![image-20240823114307690](chapter16.assets/image-20240823114307690.png)
>
> 在这里，结果被复制到result指定的新位置，因此对于指定区间而言，只读输入迭代器足够了。

**注意，replace_copy( )的返回类型为OutputIterator。对于复制算法，统一的约定是：返回一个迭代器，该迭代器指向复制的最后一个值后面的一个位置。**

另一个常见的变体是：有些函数有这样的版本，即根据将函数应用于容器元素得到的结果来执行操作。这些版本的名称通常以_if结尾。

> 例如，如果将函数用于旧值时，返回的值为true，则replace_if( )将把旧值替换为新的值。下面是该函数的原型：
>
> ![image-20240823114558164](chapter16.assets/image-20240823114558164.png)
>
> 如前所述，谓词是返回bool值的一元函数。还有一个replace_copy_if( )版本，您不难知道其作用和原型。
>
> STL使用诸如Predicate,Generator和BinaryPredicate等术语来指示必须模拟其他函数对象概念的参数
>
> 注意：虽然文档可指出迭代器或函数符需求，但编译器不会对此进行检查。如果您使用了错误的迭代器，则编译器试图实例化模板时，将显示大量的错误消息。

### 16.6.3 STL和string类

string类虽然不是STL的组成部分，但设计它时考虑到了STL。

例如，它包含begin( )、end( )、rbegin( )和rend( )等成员，因此可以使用STL接口

程序清单16.17用STL显示了使用一个词的字母可以得到的所有排列组合。排列组合就是重新安排容器中元素的顺序。

> next_permutation( )算法将区间内容转换为下一种排列方式。对于字符串，排列按照字母递增的顺序进行。如果成功，该算法返回true；如果区间已经处于最后的序列中，则该算法返回false。要得到区间内容的所有排列组合，应从最初的顺序开始，为此程序使用了STL算法sort( )。

```cpp
//
// Created by wWX1283744 on 2024/8/23.
//

#include <iostream>
#include <string>
#include <algorithm>

int main(){
    using namespace std;

    string letters;
    cout << "Enter the letter grouping (quit to quit): ";
    while(cin>>letters&&letters!="quit"){
        cout << "Permutations of " << letters << endl;
        sort(letters.begin(),letters.end());
        cout<<letters<<endl;
        while(next_permutation(letters.begin(),letters.end()))
            cout<<letters<<endl;
        cout << "Enter next sequence (quit to quit): ";
    }
    cout << "Done.\n";
    return 0;
}
//输出
Enter next sequence (quit to quit):all
 Permutations of all
all
lal
lla
Enter next sequence (quit to quit):
```

### 16.6.4 函数和容器方法

有时可以选择使用STL方法或STL函数。通常方法是更好的选择。首先，它更适合于特定的容器；其次，作为成员函数，它可以使用模板类的内存管理工具，从而在需要时调整容器的长度。

尽管方法通常更适合，但非方法函数更通用。正如您看到的，可以将它们用于数组、string对象、STL容器，还可以用它们来处理混合的容器类型

> 例如，假设有一个由数字组成的链表，并要删除链表中某个特定值（例如4）的所有实例。如果la是一个list<int>对象，则可以使用链表的remove( )方法，调用该方法后，链表中所有值为4的元素都将被删除，同时链表的长度将被自动调
> 整。
>
> ![image-20240823120012242](chapter16.assets/image-20240823120012242.png)
>
> 还有一个名为remove( )的STL算法（见附录G），它不是由对象调用，而是接受区间参数。因此，如果lb是一个list<int>对象，则调用该函数的代码如下：
>
> ![image-20240823120039898](chapter16.assets/image-20240823120039898.png)
>
> 然而，由于该remove( )函数不是成员，因此不能调整链表的长度。它将没被删除的元素放在链表的开始位置，并返回一个指向新的超尾值的迭代器。这样，便可以用该迭代器来修改容器的长度。例如，可以使用链表的erase( )方法来删除一个区间，该区间描述了链
> 表中不再需要的部分。
>
> ![image-20240823120140563](chapter16.assets/image-20240823120140563.png)

### 16.6.5 使用STL

STL是一个库，其组成部分被设计成协同工作。STL组件是工具，但也是创建其他工具的基本部件。

使用STL时应尽可能减少要编写的代码。STL通用、灵活的设计将节省大量工作。另外，STL设计者就是非常关心效率的算法人员，算法是经过仔细选择的，并且是内联的。

> 假设要编写一个程序，让用户输入单词。希望最后得到一个按输入顺序排列的单词列表、一个按字母顺序排列的单词列表（忽略大小写），并记录每个单词被输入的次数。
>
> 可按下列步骤来完成该需求：
>
> - 输入和保存单词列表很简单。
>
> ![image-20240823150517691](chapter16.assets/image-20240823150517691.png)
>
> - 得到按字母顺序排列的单词列表
>
> ![image-20240823150653740](chapter16.assets/image-20240823150653740.png)
>
> ToLower( )函数很容易编写，只需使用transform( )将tolower( )函数应用于字符串中的各个元素，并将字符串用作源和目标。
>
> ![image-20240823150746237](chapter16.assets/image-20240823150746237.png)
>
> - 获得每个单词在输入中出现的次数
>
> 使用count( )函数。它将一个区间和一个值作为参数，并返回这个值在区间中出现的次数，并将单词和计数作为pair<const
> string, int>对象存储在map对象中
>
> ![image-20240823150853629](chapter16.assets/image-20240823150853629.png)
>
> map类可以用数组表示法（将键用作索引）来访问存储的值。如wordmap[“the”]表示与键“the”相关联的值，故上面的代码可以简化
>
> ![image-20240823150937401](chapter16.assets/image-20240823150937401.png)
>
> 也可以使用数组表示法来报告结果：
>
> ![image-20240823150951477](chapter16.assets/image-20240823150951477.png)



## 16.7 其他库

头文件complex为复数提供了类模板complex，包含用于float、long和long double的具体化

C++11新增的头文件random提供了更多的随机数功能。

### 16.7.1 vector、valarray和array

C++提供三个数组模板：vector、valarray和array。这些类是由不同的小组开发的，用于不同的目的

- vector模板类是一个容器类和算法系统的一部分，它支持面向容器的操作，如排序、插入、重新排列、搜索、将数据转移到其他容器中等

- valarray类模板是面向数值计算的，不是STL的一部分。为很多数学运算提供了一个简单、直观的接口
  - 没有支持插入、排序、搜索等操作的方法。
  - valarray类确实有一个resize( )方法，但不能像使用vector的push_back时那样自动调整大小
  - 与vector类相比，valarray类关注的东西更少，但这使得它的接口更简单。

- array是为替代内置数组而设计的，它通过提供更好、更安全的接口，让数组更紧凑，效率更高。Array表示长度固定的数组，因此不支持push_back( )和insert( )，但提供了多个STL方法，包括begin( )、end( )、rbegin( )和rend( )，这使得很容易将STL算法用于array对象。

例如，假设有如下声明：

![image-20240824110748771](chapter16.assets/image-20240824110748771.png)

同时，假设ved1、ved2、vod1、vod2、vad1和vad2都有合适的值。**要将两个数组中第一个元素的和赋给第三个数组的第一个元素**

- 使用vector类时，可以这样做：

![image-20240824110813115](chapter16.assets/image-20240824110813115.png)

- 对于array类，也可以这样做：

![image-20240824110852507](chapter16.assets/image-20240824110852507.png)

- 然而，valarray类重载了所有算术运算符，使其能够用于valarray对象，因此您可以这样做：

![image-20240824110908620](chapter16.assets/image-20240824110908620.png)

同样，下面的语句将使vad3中每个元素都是vad1和vad2中相应元素的乘积：

![image-20240824110953989](chapter16.assets/image-20240824110953989.png)

要将数组中每个元素的值扩大2.5倍，STL方法如下：

![image-20240824111005518](chapter16.assets/image-20240824111005518.png)

valarray类重载了将valarray对象乘以一个值的运算符，还重载了各种组合赋值运算符，因此可以采取下列两种方法之一：

![image-20240824111029306](chapter16.assets/image-20240824111029306.png)

假设您要计算数组中每个元素的自然对数，并将计算结果存储到另一个数组的相应元、素中，STL方法如下：

![image-20240824111106419](chapter16.assets/image-20240824111106419.png)

valarray类重载了这种数学函数，使之接受一个valarray参数，并返回一个valarray对象，因此您可以这样做：

![image-20240824111127245](chapter16.assets/image-20240824111127245.png)

也可以使用apply( )方法，该方法也适用于非重载函数：

![image-20240824111144212](chapter16.assets/image-20240824111144212.png)

> 方法apply( )不修改调用对象，而是返回一个包含结果的新对象。

执行多步计算时，valarray接口的简单性将更为明显：

![image-20240824111217980](chapter16.assets/image-20240824111217980.png)

可以将STL功能用于valarray对象吗？通过回答这个问题，可以快速地复习一些STL原理。

> 假设有一个包含10个元素的valarray<double>对象：
>
> ![image-20240824111511767](chapter16.assets/image-20240824111511767.png)
>
> 使用数字填充该数组后，能够将STL sort( )函数用于该数组吗？
>
> valarray类没有begin( )和end( )方法，因此不能将它们用作指定区间的参数：
>
> ![image-20240824111543243](chapter16.assets/image-20240824111543243.png)
>
> 另外，vad是一个对象，而不是指针，因此不能像处理常规数组那样，使用vad和vad+10作为区间参数，即下面的代码不可行：
>
> ![image-20240824111619757](chapter16.assets/image-20240824111619757.png)
>
> 可以使用地址运算符：
>
> ![image-20240824111747228](chapter16.assets/image-20240824111747228.png)
>
> 使用6种编译器测试上述代码时，都是可行的，但是由于valarray没有定义下标超过尾部一个元素的行为，故如果让数组与预留给堆的内存块相邻（不太可能出现的条件），则上述代码会出现错误。
>
> 

为解决这种问题，C++11提供了接受valarray对象作为参数的模板函数begin( )和end()。

![image-20240824111949005](chapter16.assets/image-20240824111949005.png)

valarray数组支持和常数进行比较，得到的结果是数组每个元素和常数比较后的bool值组成的valarray数组：

![image-20240824112302620](chapter16.assets/image-20240824112302620.png)

slice类对象可用作数组索引，在这种情况下，它表的不是一个值而是一组值。slice对象被初始化为三个整数值：起始索引、索引数和跨距。

例如，slice(1, 4, 3)创建的对象表示选择4个元素，它们的索引分别是1、4、7和10

如果varint是一个valarray<int>对象，则下面的语句将把第1、4、7、10个元素都设置为10：

![image-20240824112504465](chapter16.assets/image-20240824112504465.png)

这种特殊的下标指定功能让您能够使用一个一维valarray对象来表示二维数据。

例如，假设要表示一个4行3列的数组，可以将信息存储在一个包含12个元素的valarray对象中，然后使用一个slice(0, 3, 1)对象作为下标，来表示元素0、1和2，即第1行。同样，下标slice(0, 4, 3)表示元素0、3、6和9，即第一列。





### 16.7.2 模板initializer_list（C++11）

模板initializer_list是C++11新增的。您可使用初始化列表语法将STL容器初始化为一系列值：

![image-20240824112803283](chapter16.assets/image-20240824112803283.png)

这之所以可行，是因为容器类现在包含将initializer_list<T>作为参数的构造函数。

例如，vector<double>包含一个将initializer_list<double>作为参数的构造函数，因此上述声明与下面的代码等价：

![image-20240824112843929](chapter16.assets/image-20240824112843929.png)

通常，考虑到C++11新增的通用初始化语法，可使用表示法{}而不是()来调用类构造函数：

![image-20240824113105548](chapter16.assets/image-20240824113105548.png)

但如果类也有接受initializer_list作为参数的构造函数，这将带来问题：

![image-20240824113118346](chapter16.assets/image-20240824113118346.png)

这将调用哪个构造函数呢？

![image-20240824113124878](chapter16.assets/image-20240824113124878.png)

答案是，如果类有接受initializer_list作为参数的构造函数，则使用语法{}将调用该构造函数。因此在这个示例中，对应的是情形B。

所有initializer_list元素的类型都必须相同，但编译器将进行必要的转换：

![image-20240824113153257](chapter16.assets/image-20240824113153257.png)

但不能进行隐式的窄化转换：

![image-20240824113211071](chapter16.assets/image-20240824113211071.png)

在这里，元素类型为int，不能隐式地将5.5转换为int。

除非类要用于处理长度不同的列表，否则让它提供接受initializer_list作为参数的构造函数没有意义

![image-20240824113421691](chapter16.assets/image-20240824113421691.png)

这样，使用语法{}时将调用构造函数Position(int, int, int)：

![image-20240824113449437](chapter16.assets/image-20240824113449437.png)

### 16.7.3 使用initializer_list

要在代码中使用initializer_list对象，必须包含头文件initializer_list。

 这个模板类包含成员函数begin( )和end( )，您可使用这些函数来访问列表元素。它还包含成员函数size( )，该函数返回元素数

![image-20240824155838553](chapter16.assets/image-20240824155838553.png)

![image-20240824155849939](chapter16.assets/image-20240824155849939.png)

initializer_list的迭代器类型为const，因此您不能修改initializer_list中的值：

![image-20240824155915999](chapter16.assets/image-20240824155915999.png)
