# 第十五章 友元、异常和其他

本章内容包括：

- 友元类；
- 友元类方法；
- 嵌套类；
- 引发异常、`try`块和`catch`块；
- 异常类；
- 运行阶段类型识别（RTTI）；
- `dynamic_cast` 和 `typeid`；
- `static_cast`、`const_cast` 和 `reiterpret_cast`。


![image-20210815204948714](https://static.fungenomics.com/images/2021/08/image-20210815204948714.png)

2．bad_alloc异常和new

对于使用new导致的内存分配问题，C++的**最新处理方式是让new引发bad_alloc异常**。头文件new包含bad_alloc类的声明，它是从exception 类公有派生而来的。但**在以前，当无法分配请求的内存量时，new返回 一个空指针**，然后我们可以 `exit(EXIT_FAILURE)`。


## 15.6 总结

友元使得能够为类开发更灵活的接口。类可以将其他函数、其他类 和其他类的成员函数作为友元。在某些情况下，可能需要使用前向声 明，需要特别注意类和方法声明的顺序，以正确地组合友元。

嵌套类是在其他类中声明的类，它有助于设计这样的助手类，即实 现其他类，但不必是公有接口的组成部分。
# 第十五章 友元、异常和其他

本章内容包括：

- 友元类；
- 友元类方法；
- 嵌套类；
- 引发异常、`try`块和`catch`块；
- 异常类；
- 运行阶段类型识别（RTTI）；
- `dynamic_cast` 和 `typeid`；
- `static_cast`、`const_cast` 和 `reiterpret_cast`。


![image-20210815204948714](https://static.fungenomics.com/images/2021/08/image-20210815204948714.png)

2．bad_alloc异常和new

对于使用new导致的内存分配问题，C++的**最新处理方式是让new引发bad_alloc异常**。头文件new包含bad_alloc类的声明，它是从exception 类公有派生而来的。但**在以前，当无法分配请求的内存量时，new返回 一个空指针**，然后我们可以 `exit(EXIT_FAILURE)`。



## 15.1 友元

### 15.1.1 友元类

什么时候希望一个类成为另一个类的友元呢？

> 我们来看一个例子。假定需要编写一个模拟电视机和遥控器的简单程序。决定定义一个Tv类和一个Remote类，来分别表示电视机和遥控器。很明显，这两个类之间应当存在某种关系，但是什么样的关系呢？遥控器并非电视机，反之亦然，所以公有继承的is-a关系并不适用。遥控器也非电视机的一部分，反之亦然，因此包含或私有继承和保护继承的has-a关系也不适用。事实上，遥控器可以改变电视机的状态，这表明应将Romote类作为Tv类的一个友元。

首先定义Tv类。可以用一组状态成员（描述电视各个方面的变量）来表示电视机。下面是一些可能的状态

- 开/关；
- 频道设置；
- 音量设置；
- 有线电视或天线调节模式；
- TV调谐或A/V输入。

接下来，必须给类提供一些修改这些设置的方法。当前，很多电视机都将控件藏在面板后面，但大多数电视机还是可以在不使用遥控器的情况下进行换台等工作的，通常只能逐频道换台，而不能随意选台。同样，通常还有两个按钮，分别用来增加和降低音量。

定义中包括一些被定义为枚举的常数。下面的语句使Remote成为友元类：

![image-20240809112216450](chapter15.assets/image-20240809112216450.png)

**友元声明可以位于公有、私有或保护部分，其所在的位置无关紧要。**

由于Remote类提到了Tv类，所以编译器必须了解Tv类后，才能处理Remote类，为此，最简单的方法是首先定义Tv类。也可以使用前向声明（forward delaration），这将稍后介绍。

程序清单15.1 tv.h

```cpp
// tv.h -- Tv and Remote classes
#ifndef TV_H_
#define TV_H_

class Tv
{
public:
    friend class Remote;   // Remote can access Tv private parts
    enum {Off, On};
    enum {MinVal,MaxVal = 20};
    enum {Antenna, Cable};
    enum {TV, DVD};

    Tv(int s = Off, int mc = 125) : state(s), volume(5),
        maxchannel(mc), channel(2), mode(Cable), input(TV) {}
    void onoff() {state = (state == On)? Off : On;}
    bool ison() const {return state == On;}
    bool volup();
    bool voldown();
    void chanup();
    void chandown();
    void set_mode() {mode = (mode == Antenna)? Cable : Antenna;}
    void set_input() {input = (input == TV)? DVD : TV;}
    void settings() const; // display all settings
private:
    int state;             // on or off
    int volume;            // assumed to be digitized
    int maxchannel;        // maximum number of channels
    int channel;           // current channel setting
    int mode;              // broadcast or cable
    int input;             // TV or DVD
};

class Remote
{
private:
    int mode;              // controls TV or DVD
public:
    Remote(int m = Tv::TV) : mode(m) {}
    bool volup(Tv & t) { return t.volup();}
    bool voldown(Tv & t) { return t.voldown();}
    void onoff(Tv & t) { t.onoff(); }
    void chanup(Tv & t) {t.chanup();}
    void chandown(Tv & t) {t.chandown();}
    void set_chan(Tv & t, int c) {t.channel = c;}
    void set_mode(Tv & t) {t.set_mode();}
    void set_input(Tv & t) {t.set_input();}
};
#endif

```

除构造函数外，所有的Romote方法都将一个Tv对象引用作为参数，这表明遥控器必须针对特定的电视机。

很多方法都使用条件运算符在两种状态之间切换：

![image-20240809113012038](chapter15.assets/image-20240809113012038.png)

如果两种状态值分别为true（1）和false（0），则可以结合使用将在附录E讨论的按位异或和赋值运算符（^=）来简化上述代码：

![image-20240809113141126](chapter15.assets/image-20240809113141126.png)

> a⊕b = (¬a ∧ b) ∨ (a ∧¬b)
>
> 如果a、b两个值不相同，则异或结果为1。如果a、b两个值相同，异或结果为0。
>
> 异或也叫半加[运算](https://baike.baidu.com/item/运算/5866856?fromModule=lemma_inlink)，其运算法则相当于不带进位的二进制加法：0⊕0=0，1⊕0=1，0⊕1=1，1⊕1=0



程序清单15.2 tv.cpp

```cpp
// tv.cpp -- methods for the Tv class (Remote methods are inline)
#include <iostream>
#include "tv.h"

bool Tv::volup()
{
    if (volume < MaxVal)
    {
        volume++;
        return true;
    }
    else
        return false;
}
bool Tv::voldown()
{
    if (volume > MinVal)
    {
        volume--;
        return true;
    }
    else
        return false;
}

void Tv::chanup()
{
    if (channel < maxchannel)
        channel++;
    else
        channel = 1;
}

void Tv::chandown()
{
    if (channel > 1)
        channel--;
    else
        channel = maxchannel;
}

void Tv::settings() const
{
    using std::cout;
    using std::endl;
    cout << "TV is " << (state == Off? "Off" : "On") << endl;
    if (state == On)
    {
        cout << "Volume setting = " << volume << endl;
        cout << "Channel setting = " << channel << endl;
        cout << "Mode = "
            << (mode == Antenna? "antenna" : "cable") << endl;
        cout << "Input = "
            << (input == TV? "TV" : "DVD") << endl;
    }
}

```

程序清单15.3 use_tv.cpp

```cpp
//use_tv.cpp -- using the Tv and Remote classes
#include <iostream>
#include "tv.h"

int main()
{
    using std::cout;
    Tv s42;
    cout << "Initial settings for 42\" TV:\n";
    s42.settings();
    s42.onoff();
    s42.chanup();
    cout << "\nAdjusted settings for 42\" TV:\n";
    s42.settings();

    Remote grey;

    grey.set_chan(s42, 10);
    grey.volup(s42);
    grey.volup(s42);
    cout << "\n42\" settings after using remote:\n";
    s42.settings();

    Tv s58(Tv::On);
    s58.set_mode();
    grey.set_chan(s58,28);
    cout << "\n58\" settings:\n";
    s58.settings();
    // std::cin.get();
    return 0; 
}

```

### 15.1.2 友元成员函数

> 从上一个例子中的代码可知，大多数Remote方法都是用Tv类的公有接口实现的。这意味着这些方法不是真正需要作为友元。事实上，唯一直接访问Tv成员的Remote方法是Remote::set_chan( )，因此它是唯一需要作为友元的方法。确实可以选择仅让特定的类成员成为另一个类的友元，而不必让整个类成为友元，但这样做稍微有点麻烦，必须小心排列各种声明和定义的顺序。下面介绍其中的原因。

让Remote::set_chan( )成为Tv类的友元的方法是，在Tv类声明中将其声明为友元：

![image-20240809115354761](chapter15.assets/image-20240809115354761.png)

然而，**要使编译器能够处理这条语句，它必须知道Remote的定义**。否则，它无法知道Remote是一个类，而set_chan是这个类的方法。这意味着**应将Remote的定义放到Tv的定义前面**。**Remote的方法提到了Tv对象，而这意味着Tv定义应当位于Remote定义之前**。避开这种循环依赖的方法是，使用**前向声明（forward declaration）**

为此，需要在Remote定义的前面插入下面的语句：

![image-20240809115532483](chapter15.assets/image-20240809115532483.png)

这样，排列次序应如下：

![image-20240809115546449](chapter15.assets/image-20240809115546449.png)

> 能否像下面这样排列呢？
>
> ![image-20240809115641593](chapter15.assets/image-20240809115641593.png)
>
> 答案是不能。原因在于，在编译器在Tv类的声明中看到Remote的一个方法被声明为Tv类的友元之前，应该先看到Remote类的声明和set_chan( )方法的声明。

还有一个麻烦。程序清单15.1的Remote声明包含了内联代码，例如：

![image-20240809115720906](chapter15.assets/image-20240809115720906.png)

由于这将调用Tv的一个方法，所以编译器此时必须已经看到了Tv类的声明，这样才能知道Tv有哪些方法，但正如看到的，该声明位于Remote声明的后面。

这种问题的解决方法是，使Remote声明中只包含方法声明，并将实际的定义放在Tv类之后。这样，排列顺序将如下：

![image-20240809115855603](chapter15.assets/image-20240809115855603.png)

检查该原型时，所有的编译器都需要知道Tv是一个类，而前向声明提供了这样的信息。当编译器到达真正的方法定义时，它已经读取了Tv类的声明，并拥有了编译这些方法所需的信息。通过在方法定义中使用inline关键字，仍然可以使其成为内联方法。

程序清单15.4列出了修订后的头文件。

程序清单15.4 tvfm.h

```cpp
// tvfm.h -- Tv and Remote classes using a friend member
#ifndef TVFM_H_
#define TVFM_H_

class Tv;                       // forward declaration

class Remote
{
public:
    enum State{Off, On};
    enum {MinVal,MaxVal = 20};
    enum {Antenna, Cable};
    enum {TV, DVD};
private:
    int mode;
public:
    Remote(int m = TV) : mode(m) {}
    bool volup(Tv & t);         // prototype only
    bool voldown(Tv & t);
    void onoff(Tv & t);
    void chanup(Tv & t);
    void chandown(Tv & t);
    void set_mode(Tv & t);
    void set_input(Tv & t);
    void set_chan(Tv & t, int c);
};

class Tv
{
public:
    friend void Remote::set_chan(Tv & t, int c);
    enum State{Off, On};
    enum {MinVal,MaxVal = 20};
    enum {Antenna, Cable};
    enum {TV, DVD};

    Tv(int s = Off, int mc = 125) : state(s), volume(5),
        maxchannel(mc), channel(2), mode(Cable), input(TV) {}
    void onoff() {state = (state == On)? Off : On;}
    bool ison() const {return state == On;}
    bool volup();
    bool voldown();
    void chanup();
    void chandown();
    void set_mode() {mode = (mode == Antenna)? Cable : Antenna;}
    void set_input() {input = (input == TV)? DVD : TV;}
    void settings() const;
private:
    int state;
    int volume;
    int maxchannel;
    int channel;
    int mode;
    int input;
};

// Remote methods as inline functions
inline bool Remote::volup(Tv & t) { return t.volup();}
inline bool Remote::voldown(Tv & t) { return t.voldown();}
inline void Remote::onoff(Tv & t) { t.onoff(); }
inline void Remote::chanup(Tv & t) {t.chanup();}
inline void Remote::chandown(Tv & t) {t.chandown();}
inline void Remote::set_mode(Tv & t) {t.set_mode();}
inline void Remote::set_input(Tv & t) {t.set_input();}
inline void Remote::set_chan(Tv & t, int c) {t.channel = c;} 
#endif

```

### 15.1.3 其他友元关系

### 15.1.4 共同的友元

需要使用友元的另一种情况是，函数需要访问两个类的私有数据。

将函数作为两个类的友元更合理

![image-20240810144349709](chapter15.assets/image-20240810144349709.png)

![image-20240810144356855](chapter15.assets/image-20240810144356855.png)

## 15.2 嵌套类

在C++中，可以将类声明放在另一个类中。在另一个类中声明的类被称为嵌套类（nested class），它通过提供新的类型类作用域来避免名称混乱。

- 包含类的成员函数可以创建和使用被嵌套类的对象
- 仅当声明位于公有部分，才能在包含类的外面使用嵌套类，而且必须使用作用域解析运算符

对类进行嵌套与包含并不同。包含意味着将类对象作为另一个类的成员，而对类进行嵌套不创建类成员，而是定义了一种类型，该类型仅在包含嵌套类声明的类中有效。

对类进行嵌套通常是为了帮助实现另一个类，并避免名称冲突。Queue类示例（第12章的程序清单12.8）嵌套了结构定义，从而实现了一种变相的嵌套类：

![image-20240810144738047](chapter15.assets/image-20240810144738047.png)

由于结构是一种其成员在默认情况下为公有的类，所以Node实际上是一个嵌套类，但该定义并没有充分利用类的功能。具体地说，它没有显式构造函数，下面进行补救。

首先，找到Queue示例中创建Node对象的位置。从类声明（程序清单11.10）和方法定义（程序清单12.11）可知，唯一创建了Node对象的地方是enqueue( )方法：

![image-20240810145118897](chapter15.assets/image-20240810145118897.png)



![image-20240810144949449](chapter15.assets/image-20240810144949449.png)

该构造函数将节点的item成员初始化为i，并将next指针设置为0，这是使用C++编写空值指针的方法之一（使用NULL时，必须包含一个定义NULL的头文件；如果您使用的编译器支持C++11，可使用nullptr）。由于使用Queue类创建的所有节点的next的初始值都被设置为空指针，因此这个类只需要该构造函数。

接下来，需要使用构造函数重新编写enqueue( )：

<img src="chapter15.assets/image-20240810145213313.png" alt="image-20240810145213313" style="zoom:67%;" />



### 15.2.1 嵌套类和访问权限

有两种访问权限适合于嵌套类。首先，嵌套类的声明位置决定了嵌套类的作用域，即它决定了程序的哪些部分可以创建这种类的对象。其次，和其他类一样，嵌套类的公有部分、保护部分和私有部分控制了对类成员的访问

#### 1．作用域

如果嵌套类是在另一个类的私有部分声明的，则只有后者知道它。

如果嵌套类是在另一个类的保护部分声明的，则它对于后者来说是可见的，但是对于外部世界则是不可见的。然而，在这种情况中，派生类将知道嵌套类，并可以直接创建这种类型的对象。

如果嵌套类是在另一个类的公有部分声明的，则允许后者、后者的派生类以及外部世界使用它，因为它是公有的。然而，由于嵌套类的作用域为包含它的类，因此在外部世界使用它时，必须使用类限定符。

![image-20240810151412396](chapter15.assets/image-20240810151412396.png)

现在假定有一个失业的教练，他不属于任何球队。要在Team类的外面创建Coach对象，可以这样做：

![image-20240810151431911](chapter15.assets/image-20240810151431911.png)

#### 2．访问控制

类可见后，起决定作用的将是访问控制。对嵌套类访问权的控制规则与对常规类相同。

在Queue类声明中声明Node类并没有赋予Queue类任何对Node类的访问特权，也没有赋予Node类任何对Queue类的访问特权。因此，Queue类对象只能显示地访问Node对象的公有成员.

## 15.3 异常

程序有时会遇到运行阶段错误，导致程序无法正常地运行下去。C++异常为处理这种情况提供了一种功能强大而灵活的工具。异常是相对较新的C++功能，有些老式编译器可能没有实现。另外，有些编译器默认关闭这种特性，您可能需要使用编译器选项来启用它。

### 15.3.1 调用abort( )

Abort( )函数的原型位于头文件cstdlib（或stdlib.h）中，其典型实现是向标准错误流（即cerr使用的错误流）发送消息abnormal program termination（程序异常终止），然后终止程序.它还返回一个随实现而异的值，告诉操作系统（如果程序是由另一个程序调用的，则告诉父进程），处理失败。abort( )是否刷新文件缓冲区（用于存储读写到文件中的数据的内存区域）取决于实现。如果愿意，也可以使用exit( )，该函数刷新文件缓冲区，但不显示消息。

```cpp
//error1.cpp -- using the abort() function
#include <iostream>
#include <cstdlib>
double hmean(double a, double b);

int main()
{
    double x, y, z;

    std::cout << "Enter two numbers: ";
    while (std::cin >> x >> y)
    {
        z = hmean(x,y);
        std::cout << "Harmonic mean of " << x << " and " << y
            << " is " << z << std::endl;
        std::cout << "Enter next set of numbers <q to quit>: ";
    }
    std::cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if (a == -b)
    {
        std::cout << "untenable arguments to hmean()\n";
        std::abort();
    }
    return 2.0 * a * b / (a + b); 
}

```

### 15.3.2 返回错误码

一种比异常终止更灵活的方法是，使用函数的返回值来指出问题。

> 例如，ostream类的get（void）成员通常返回下一个输入字符的ASCII码，但到达文件尾时，将返回特殊值EOF。对hmean( )来说，这种方法不管用。任何数值都是有效的返回值，因此不存在可用于指出问题的特殊值。**在这种情况下，可使用指针参数或引用参数来将值返回给调用程序，并使用函数的返回值来指出成功还是失败。**istream族重载>>运算符使用了这种技术的变体。通过告知调用程序是成功了还是失败了，使得程序可以采取除异常终止程序之外的其他措施。
>
> 程序清单15.8是一个采用这种方式的示例，它将hmean( )的返回值重新定义为bool，让返回值指出成功了还是失败了，另外还给该函数增加了第三个参数，用于提供答案。

```cpp
//error2.cpp -- returning an error code
#include <iostream>
#include <cfloat>  // (or float.h) for DBL_MAX

bool hmean(double a, double b, double * ans);

int main()
{
    double x, y, z;

    std::cout << "Enter two numbers: ";
    while (std::cin >> x >> y)
    {
        if (hmean(x,y,&z))
            std::cout << "Harmonic mean of " << x << " and " << y
                << " is " << z << std::endl;
        else
            std::cout << "One value should not be the negative "
                << "of the other - try again.\n";
        std::cout << "Enter next set of numbers <q to quit>: ";
    }
    std::cout << "Bye!\n";
    return 0;
}

bool hmean(double a, double b, double * ans)
{
    if (a == -b)
    {
        *ans = DBL_MAX;
        return false;
    }
    else
    {
        *ans = 2.0 * a * b / (a + b);
        return true;
    }
}

```

第三参数可以是指针或引用。对内置类型的参数，很多程序员都倾向于使用指针，因为这样可以明显看出是哪个参数用于提供答案。

另一种在某个地方存储返回条件的方法是使用一个全局变量。可能问题的函数可以在出现问题时将该全局变量设置为特定的值，而调用程序可以检查该变量。传统的C语言数学库使用的就是这种方法，它使用的全局变量名为errno。当然，必须确保其他函数没有将该全局变量用于其他目的。

### 15.3.3 异常机制

C++异常是对程序运行过程中发生的异常情况（例如被0除）的一种响应。异常提供了将控制权从程序的一个部分传递到另一部分的途径。对异常的处理有3个组成部分：

- 引发异常；
- 使用处理程序捕获异常；
- 使用try块。

throw语句实际上是跳转，即命令程序跳到另一条语句。throw关键字表示引发异常，紧随其后的值（例如字符串或对象）指出了异常的特征。

catch关键字表示捕获异常。处理程序以关键字catch开头，**随后是位于括号中的类型声明，它指出了异常处理程序要响应的异常类型；**然后是一个用花括号括起的
代码块，指出要采取的措施。catch关键字和异常类型用作标签，指出当异常被引发时，程序应跳到这个位置执行。异常处理程序也被称为catch块。

try块标识其中特定的异常可能被激活的代码块，它后面跟一个或多个catch块。try块是由关键字try指示的，关键字try的后面是一个由花括号括起的代码块，表明需要注意这些代码引发的异常。

```cpp
// error3.cpp -- using an exception
#include <iostream>
double hmean(double a, double b);

int main()
{
    double x, y, z;

    std::cout << "Enter two numbers: ";
    while (std::cin >> x >> y)
    {
        try {                   // start of try block
            z = hmean(x,y);
        }                       // end of try block
        catch (const char * s)  // start of exception handler
        {
            std::cout << s << std::endl;
            std::cout << "Enter a new pair of numbers: ";
            continue;
        }                       // end of handler
        std::cout << "Harmonic mean of " << x << " and " << y
            << " is " << z << std::endl;
        std::cout << "Enter next set of numbers <q to quit>: ";
    }
    std::cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if (a == -b)
        throw "bad hmean() arguments: a = -b not allowed";
    return 2.0 * a * b / (a + b); 
}

```

引发异常的代码与下面类似：

![image-20240810161119909](chapter15.assets/image-20240810161119909.png)

其中被引发的异常是字符串“bad hmean( )arguments: a = -b not allowed”。异常类型可以是字符串（就像这个例子中那样）或其他C++类型；通常为类类型，本章后面的示例将说明这一点。

**执行throw语句类似于执行返回语句，因为它也将终止函数的执行；但throw不是将控制权返回给调用程序，而是导致程序沿函数调用序列后退，直到找到包含try块的函数**

### 15.3.4 将对象用作异常类型

通常，引发异常的函数将传递一个对象。这样做的重要优点之一是，可以使用不同的异常类型来区分不同的函数在不同情况下引发的异常。

另外，对象可以携带信息，程序员可以根据这些信息来确定引发异常的原因.。同时，catch块可以根据这些信息来决定采取什么样的措施

<img src="chapter15.assets/image-20240810161547050.png" alt="image-20240810161547050" style="zoom: 67%;" />

函数hmean( )可以使用下面这样的代码：

![image-20240810161709394](chapter15.assets/image-20240810161709394.png)

```cpp
//error4.cpp � using exception classes
#include <iostream>
#include <cmath> // or math.h, unix users may need -lm flag
#include "exc_mean.h"
// function prototypes
double hmean(double a, double b);
double gmean(double a, double b);
int main()
{
    using std::cout;
    using std::cin;
    using std::endl;
    
    double x, y, z;

    cout << "Enter two numbers: ";
    while (cin >> x >> y)
    {
        try {                  // start of try block
            z = hmean(x,y);
            cout << "Harmonic mean of " << x << " and " << y
                << " is " << z << endl;
            cout << "Geometric mean of " << x << " and " << y
                << " is " << gmean(x,y) << endl;
            cout << "Enter next set of numbers <q to quit>: ";
        }// end of try block
        catch (bad_hmean & bg)    // start of catch block
        {
            bg.mesg();
            cout << "Try again.\n";
            continue;
        }                  
        catch (bad_gmean & hg) 
        {
            cout << hg.mesg();
            cout << "Values used: " << hg.v1 << ", " 
                 << hg.v2 << endl;
            cout << "Sorry, you don't get to play any more.\n";
            break;
        } // end of catch block
    }
    cout << "Bye!\n";
    // cin.get();
    // cin.get();
    return 0;
}

double hmean(double a, double b)
{
    if (a == -b)
        throw bad_hmean(a,b);
    return 2.0 * a * b / (a + b);
}

double gmean(double a, double b)
{
    if (a < 0 || b < 0)
        throw bad_gmean(a,b);
    return std::sqrt(a * b); 
}

```



### 15.3.6 栈解退

假设try块没有直接调用引发异常的函数，而是调用了对引发异常的函数进行调用的函数，则程序流程将从引发异常的函数跳到包含try块和处理程序的函数。这涉及到栈解退（unwinding the stack）

C++通常通过将信息放在栈（参见第9章）中来处理函数调用。具体地说，程序将调用函数的指令的地址（返回地址）放到栈中。当被调用的函数执行完毕后，程序将使用该地址来确定从哪里开始继续执行。另外，函数调用将函数参数放到栈中。在栈中，这些函数参数被视为自动变量。如果被调用的函数创建了新的自动变量，则这些变量也将被添加到栈中。如果被调用的函数调用了另一个函数，则后者的信息将被添加到栈中，依此类推。当函数结束时，程序流程将跳到该函数被调用时存储的地址处，同时栈顶的元素被释放。因此，函数通常都返回到调用它的函数，依此类推，同时每个函数都在结束时释放其自动变量。如果自动变量是类对象，则类的析构函数（如果有的话）将被调用。

现在假设函数由于出现异常（而不是由于返回）而终止，则程序也将释放栈中的内存，但不会在释放栈的第一个返回地址后停止，而是继续释放栈，直到找到一个位于try块（参见图15.3）中的返回地址。随后，控制权将转到块尾的异常处理程序，而不是函数调用后面的第一条语句。这个过程被称为栈解退

引发机制的一个非常重要的特性是，和函数返回一样，对于栈中的自动类对象，类的析构函数将被调用

>  **程序进行栈解退以回到能够捕获异常的地方时，将释放栈中的自动存储型变量。如果变量是类对象，将为该对象调用析构函数**

### 15.3.8 exception类

较新的C++编译器将异常合并到语言中。例如，为支持该语言，exception头文件（以前为exception.h或except.h）定义了exception类，C++可以把它用作其他异常类的基类。代码可以引发exception异常，也可以将exception类用作基类。

有一个名为what( )的虚拟成员函数，它返回一个字符串，该字符串的特征随实现而异。然而，由于这是一个虚方法，因此可以在从exception派生而来的类中重新定义它：

<img src="chapter15.assets/image-20240810165547468.png" alt="image-20240810165547468" style="zoom:67%;" />

C++库定义了很多基于exception的异常类型。

1．stdexcept异常类

该文件定义了logic_error和runtime_error类，它们都是以公有方式从exception派生而来的：

异常类系列logic_error描述了典型的逻辑错误。

> domain_error；
> invalid_argument；
> length_error；
> out_of_bounds。

runtime_error异常系列描述了可能在运行期间发生但难以预计和防范的错误。

> range_error；
> overflow_error；
> underflow_error。

![image-20240810165942774](chapter15.assets/image-20240810165942774.png)

2．bad_alloc异常和new

对于使用new导致的内存分配问题，C++的最新处理方式是让new引发bad_alloc异常。

![image-20240810170008492](chapter15.assets/image-20240810170008492.png)

3．空指针和new

很多代码都是在new在失败时返回空指针时编写的。为处理new的变化，有些编译器提供了一个标记（开关），让用户选择所需的行为。当前，C++标准提供了一种在失败时返回空指针的new，其用法如下：

![image-20240810170144389](chapter15.assets/image-20240810170144389.png)

使用这种new，可将程序清单15.13的核心代码改为如下所示：

![image-20240810170157109](chapter15.assets/image-20240810170157109.png)

![image-20240810170321957](chapter15.assets/image-20240810170321957.png)

## 15.4 RTTI

RTTI是运行阶段类型识别（Runtime Type Identification）的简称。这是新添加到C++中的特性之一，很多老式实现不支持。

RTTI旨在为程序在运行阶段确定对象的类型提供一种标准方式

### 15.4.1 RTTI的用途

>  假设有一个类层次结构，其中的类都是从同一个基类派生而来的，则可以让基类指针指向其中任何一个类的对象。这样便可以调用这样的函数：在处理一些信息后，选择一个类，并创建这种类型的对象，然后返回它的地址，而该地址可以被赋给基类指针。

**如何知道指针指向的是哪种对象呢？**



### 15.4.2 RTTI的工作原理

C++有3个支持RTTI的元素。

- 如果可能的话，dynamic_cast运算符将使用一个指向基类的指针来生成一个指向派生类的指针；否则，该运算符返回0——空指针。
- typeid运算符返回一个指出对象的类型的值。
- type_info结构存储了有关特定类型的信息。

**只能将RTTI用于包含虚函数的类层次结构，原因在于只有对于这种类层次结构，才应该将派生对象的地址赋给基类指针。**

#### 1．dynamic_cast运算符

dynamic_cast运算符是最常用的RTTI组件，它不能回答“指针指向的是哪类对象”这样的问题，但能够回答“是否可以安全地将对象的地址赋给特定类型的指针”这样的问题

假设有下面这样的类层次结构：

![image-20240810172147203](chapter15.assets/image-20240810172147203.png)

接下来假设有下面的指针：

![image-20240810172203092](chapter15.assets/image-20240810172203092.png)

最后，对于下面的类型转换：

![image-20240810172233736](chapter15.assets/image-20240810172233736.png)

**哪些是安全的？**

根据类声明，它们可能全都是安全的，但只有那些指针类型与对象的类型（或对象的直接或间接基类的类型）相同的类型转换才一定是安全的。

>  例如，类型转换#1就是安全的，因为它将Magificent类型的指针指向类型为Magnificent的对象。
>
> 类型转换#2就是不安全的，因为它将基数对象（Grand）的地址赋给派生类（Magnificent）指针。因此，程序将期望基类对象有派生类的特征，而通常这是不可能的。例如，Magnificent对象可能包含一些Grand对象没有的数据成员。
>
> 然而，类型转换#3是安全的，因为它将派生对象的地址赋给基类指针。

即公有派生确保Magnificent对象同时也是一个Superb对象（直接基类）和一个Grand对象（间接基类）。因此，将它的地址赋给这3种类型的指针都是安全的。虚函数确保了将这3种指针中的任何一种指向Magnificent对象时，都将调用Magnificent方法。

**通常想知道类型的原因在于：知道类型后，就可以知道调用特定的方法是否安全**

先来看一下dynamic_cast的语法。该运算符的用法如下，其中pg指向一个对象：

![image-20240810172840062](chapter15.assets/image-20240810172840062.png)

这提出了这样的问题：指针pg的类型是否可被安全地转换为Superb *？如果可以，运算符将返回对象的地址，否则返回一个空指针

**dynamic_cast语法：通常，如果指向的对象（*pt）的类型为Type或者是从Type直接或间接派生而来的类型，则下面的表达式将指针pt转换为Type类型的指针：**

**![image-20240810173127763](chapter15.assets/image-20240810173127763.png)**

**否则，结果为0，即空指针**。

```cpp
// rtti1.cpp -- using the RTTI dynamic_cast operator
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;

class Grand
{
private:
    int hold;
public:
    Grand(int h = 0) : hold(h) {}
    virtual void Speak() const { cout << "I am a grand class!\n";}
    virtual int Value() const { return hold; }
};

class Superb : public Grand
{
public:
    Superb(int h = 0) : Grand(h) {}
    void Speak() const {cout << "I am a superb class!!\n"; }
    virtual void Say() const
        { cout << "I hold the superb value of " << Value() << "!\n";}
};

class Magnificent : public Superb
{
private:
    char ch;
public:
    Magnificent(int h = 0, char c = 'A') : Superb(h), ch(c) {}
    void Speak() const {cout << "I am a magnificent class!!!\n";}
    void Say() const {cout << "I hold the character " << ch <<
               " and the integer "  << Value() << "!\n"; }
};

Grand * GetOne();

int main()
{
    std::srand(std::time(0));
    Grand * pg;
    Superb * ps;
    for (int i = 0; i < 5; i++)
    {
        pg = GetOne();
        pg->Speak();
        if( ps = dynamic_cast<Superb *>(pg))
            ps->Say();
    }
    // std::cin.get();
    return 0;
}

Grand * GetOne()    // generate one of three kinds of objects randomly
{
    Grand * p;
    switch( std::rand() % 3)
    {
        case 0: p = new Grand(std::rand() % 100);
                    break;
        case 1: p = new Superb(std::rand() % 100);
                    break;
        case 2: p = new Magnificent(std::rand() % 100, 
                              'A' + std::rand() % 26);
                    break;
    }
    return p; 
}

```

#### 2．typeid运算符和type_info类

typeid运算符使得能够确定两个对象是否为同种类型。它与sizeof有些相像，可以接受两种参数：

> 类名；
> 结果为对象的表达式。

typeid运算符返回一个对type_info对象的引用，其中，type_info是在头文件typeinfo（以前为typeinfo.h）中定义的一个类。type_info类重载了= =和!=运算符，以便可以使用这些运算符来对类型进行比较。

例如，如果pg指向的是一个Magnificent对象，则下述表达式的结果为bool值true，否则为false：

![image-20240810173616875](chapter15.assets/image-20240810173616875.png)

如果pg是一个空指针，程序将引发bad_typeid异常。该异常类型是从exception类派生而来的，是在头文件typeinfo中声明的。

 通常（但并非一定）是类的名称

![image-20240810174458985](chapter15.assets/image-20240810174458985.png)

### 15.5 类型转换运算符

##### 自动类型转换

主要发生在基本内置类型：char，short ,int ,long ,float，double

- 当不同类型的变量同时运算时就会发生数据类型的自动转换。
  - char 和 int 两个类型的变量相加时，就会把 char 先转换成 int 再进行加法运算
  - 如果是 int 和 double 类型进行运算时，就会把 int 转换成 double 再进行运算。
  - 条件判断中，非布尔型自动转换为布尔类型。

- 用一个参数作为另一个不同类型参数的赋值时出现的自动转换。
  - 当定义参数是char，输入是int时，自动将int通过ASCII转换为字符
  - 当定义参数是int，输入是浮点型，自动转换为浮点型。

##### string与“万物”互转























在C++的创始人Bjarne Stroustrup看来，C语言中的类型转换运算符太过松散。例如，请看下面的代码：
<img src="chapter15.assets/image-20240812142041244.png" alt="image-20240812142041244" style="zoom:80%;" />

> 首先，上述3种类型转换中，哪一种有意义？除非不讲理，否则它们中没有一个是有意义的。其次，这3种类型转换中哪种是允许的呢？在C语言中都是允许的。

对于这种松散情况，Stroustrop采取的措施是，更严格地限制允许的类型转换，并添加4个类型转换运算符，使转换过程更规范：

> dynamic_cast；
> const_cast；
> static_cast；
> reinterpret_cast。

可以根据目的选择一个适合的运算符，而不是使用通用的类型转换。这指出了进行类型转换的原因，并让编译器能够检查程序的行为是否与设计者想法吻合。

#### 1.dynamic_cast

![image-20240812142301716](chapter15.assets/image-20240812142301716.png)

使得能够在类层次结构中进行向上转换（由于is-a关系，这样的类型转换是安全的），而不允许其他转换。

> 假设High和Low是两个类，而ph和pl的类型分别为High *和Low *，则仅当Low是High的可访问基类（直接或间接）时，下面
> 的语句才将一个Low*指针赋给pl：
>
> ![image-20240812142435070](chapter15.assets/image-20240812142435070.png)

#### 2.const_cast

> (1)const_cast只针对指针、引用，当然，this指针也是其中之一。
> (2)const_cast的大部分使用主要是将常量指针转换为常指针。常量指针指向的空间的内容不允许被修改，但是使用const_cast进行强制转换就可以修改。
> (3)const_cast只能调节类型限定符，不能修改基本类型。
> ------------------------------------------------
>
>                             版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
>
> 原文链接：https://blog.csdn.net/yi_chengyu/article/details/121921622

const_cast运算符用于执行只有一种用途的类型转换，即改变值为const或volatile，其语法与dynamic_cast运算符相同：

![image-20240812142516239](chapter15.assets/image-20240812142516239.png)

type_name和expression的类型必须相同

![image-20240812145911036](chapter15.assets/image-20240812145911036.png)

提供该运算符的原因是，有时候可能需要这样一个值，它在大多数时候是常量，而有时又是可以修改的。在这种情况下，可以将这个值声明为const，并在需要修改它的时候，使用const_cast。

这也可以通过通用类型转换来实现，但通用转换也可能同时改变类型：

![image-20240812150012422](chapter15.assets/image-20240812150012422.png)

由于编程时可能无意间同时改变类型和常量特征，因此使用const_cast运算符更安全。

const_cast不是万能的。它可以修改指向一个值的指针，但修改const值的结果是不确定的。

```cpp
int pop1 = 3226;
const int pop2 = 2563;

int *pc1 = const_cast<int*>(&pop1);
*pc1+=100;
cout<< pop1<<endl;//3326
int *pc2 = const_cast<int*>(&pop2);
*pc2+=100;
cout<< pop2<<endl;//2563
```

指针pc删除了const特征，因此可用来修改指向的值，但仅当指向的值不是const时才可行

#### 3.static_cast

语法：

![image-20240812151358561](chapter15.assets/image-20240812151358561.png)

仅当type_name可被隐式转换为expression所属的类型或expression可被隐式转换为type_name所属的类型时，上述转换才是合法的，否则将出错。

假设High是Low的基类，而Pond是一个无关的类，则从High到Low的转换、从Low到High的转换都是合法的，而从Low到Pond的转换是不允许的：

![image-20240812151508913](chapter15.assets/image-20240812151508913.png)

同理，由于无需进行类型转换，枚举值就可以被转换为整型，所以可以用static_cast将整型转换为枚举值。同样，可以使用static_cast将double转换为int、将float转换为long以及其他各种数值转换。

>  使用场景：
>
> 1.1 基础数据类型的转换
> 可以将一种基础数据类型转换为另一种基础数据类型。例如，将 double 转换为 int，或将 float 转换为 double 等。
>
> ```cpp
> double d = 5.5;
> int i = static_cast<int>(d);  // i = 5
> 
> ```
>
> 
>
> 1.2 指向派生类的指针或引用转换为指向基类的指针或引用
>
> ```cpp
> class Base {};
> class Derived : public Base {};
> Derived derivedObj;
> Base* basePtr = static_cast<Base*>(&derivedObj);
> 
> ```
>
> 
>
> 1.3 指向基类的指针或引用转换为指向派生类的指针或引用
> 但这是不安全的，因为在转换过程中没有运行时检查。如果确实需要运行时检查，应使用 dynamic_cast。
>
> ```cpp
> Base* basePtr = new Base();
> Derived* derivedPtr = static_cast<Derived*>(basePtr); // 不安全！
> ```
>
> 1.4 在有关联的类型之间进行转换
> 例如，转换枚举值为整数。
>
> ```cpp
> enum Color { RED, GREEN, BLUE };
> int value = static_cast<int>(GREEN);  // value = 1
> 
> ```
>
> 

#### 4.reinterpret_cast

- **不同基础类型指针类型之间转换**

```cpp
int *p = new int;

// 编译失败 //error: invalid static_cast from type ‘int*’ to type ‘char*’
char* p1 =  static_cast<char*>(p);

// 编译成功
char* p2 =  reinterpret_cast<char*>(p1);

```



- **将地址转换成整数**

```cpp
struct B { int val;};

B b{101};

std::cout << "&b=" << &b << std::endl;
long addr = reinterpret_cast<long>(&b);
std::cout << "addr=" << addr << std::endl;

```



该运算符的语法与另外3个相同：

![image-20240812152334567](chapter15.assets/image-20240812152334567.png)

下面是一个使用示例

```c++
struct dat{short a;short b;};
long value = 100;
dat *pd = reinterpret_cast<dat *>(&value);//将 long * --> dat *
cout<< &value<<endl;//0x61fdfc
cout<<pd<<endl;//0x61fdfc
long *pt = reinterpret_cast<long *>(pd);//转换回原来的指针类型
cout<<*pt<<endl;//100
```

其作用是可以将一个指针转化为任意其他类型的指针。



## 15.6 总结

友元使得能够为类开发更灵活的接口。类可以将其他函数、其他类 和其他类的成员函数作为友元。在某些情况下，可能需要使用前向声 明，需要特别注意类和方法声明的顺序，以正确地组合友元。

嵌套类是在其他类中声明的类，它有助于设计这样的助手类，即实 现其他类，但不必是公有接口的组成部分。
