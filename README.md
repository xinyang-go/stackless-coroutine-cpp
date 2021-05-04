# 基于switch-case语句，20行代码实现无栈协程

无栈协程原理十分简单，可以看做是状态机的一种语法糖。之所以选择协程而非状态机，是由于协程的程序写法对程序员更加友好。我们可以利用switch-case语句，实现一个本质上是状态机，但在程序写法上却和通常意义上的协程相似的程序。再结合继承和宏，进行代码生成，消除大部分的重复代码就实现了一个最简单的协程。

**本程序基于C++17实现。**

## 实现方法

以python的生成器语法为例，一个常见的写法如下：

```python
def generator(x):
    for i in range(x):
        yield i
```

我们可以将这个代码写成等价的C++语言基于goto的实现方式：

```c++
class generator{
public:
    explicit generator(int x) : x(x) {}
    
    int operator()(){
        switch(_status){
            case 0: goto status_0;
            case 1: goto status_1;
            default: ; // something wrong
        }
        
        status_0:
        for(i = 0; i < x; i++) {
            _status = 1;
            return i;
            status_1: ;
        }
        _status = -1;
        return -1;
    }
    
    bool done() const { return _status < 0; }
private:
    int _status = 0;
    int x, i;
};
```

可以看出，一个协程函数可以用一个类实现：

* 将函数中所用到的**局部变量全部定义为类的成员变量**（很重要，否则程序运行可能不符合预期）
* 每当调用operator()时，判断当前状态，并goto到特定位置
* 每当出现一个yield语句，将其替换为：保存当前状态，函数返回，定义goto标签三部分。
* done()函数用于判断当前协程是否已经结束。

由于该代码中，switch-case后直接接goto语句，所以我们可以将程序改写成以下形式：

```c++
class generator{
public:
    explicit generator(int x) : x(x) {}
    
    int operator()(){
        switch(_status){
            case 0: ;
            for(i = 0; i < x; i++) {
                _status = 1;
                return i;
                case 1: ;
            }
            _status = -1;
            return -1;
        default: ; // something wrong
        }
    }
    
    bool done() const { return _status < 0; }
private:
    int _status = 0;
    int x, i;
};
```

对于上述代码，我们可以很容易的发现其中的重复代码，可以使用继承和宏消除重复代码：

```c++
#define COROUTINE_BEGIN             switch(_status) { case 0: ;
#define COROUTINE_END                default: ; }
#define COROUTINE_YIELD(n, expr)    do{ _status = n; return expr; case n: ; }while(0)
#define COROUTINE_RETURN(expr)      do{ _status = -1; return expr; }while(0)
class coroutine_base {
public:
    bool done() const { return _status < 0; }
protected:
    int _status = 0;
};
```

其中COROUTINE_BEGIN和COROUTINE_END包裹整个函数，用于提供switch-case的主体。

COROUTINE_YIELD(n, expr)为yield语句，n为第几个yield语句（从1开始编号），expr为返回值。

COROUTINE_RETURN(expr)代表协程结束，expr为返回值。

class coroutine_base; 定义了状态变量，以及done()函数。

经过上述封装，我们的生成器协程可以简化成如下形式：

```c++
class generator : public coroutine_base {
public:
    explicit generator(int x) : x(x) {}
    
    int operator()(){
        COROUTINE_BEGIN
        for(i = 0; i < x; i++) {
            COROUTINE_YIELD(1, i);
        }
        COROUTINE_RETURN(-1);
        COROUTINE_END
    }
    
private:
    int x, i;
};
```

是不是已经很有协程的味道了！

我们还可以进一步简化这个程序，目前的程序COROUTINE_YIELD语句需要程序员手动进行编号，利用宏\__COUNTER__，我们可以简化这个步骤，详细实现方法见源码[coroutine_utils.cpp](coroutine_utils.cpp)。这样，程序可以进一步简化为：

```c++
class generator : public coroutine_base {
public:
    explicit generator(int x) : x(x) {}
    
    int operator()(){
        COROUTINE_BEGIN
        for(i = 0; i < x; i++) {
            COROUTINE_YIELD(i);
        }
        COROUTINE_RETURN(-1);
        COROUTINE_END
    }
    
private:
    int x, i;
};
```

到此，我们的这个代码和python中的已经十分相似了。

---

利用无栈协程，除了可以实现生成器外，还可以实现以下功能：

* [get_value.cpp](get_value.cpp)
  * 与生成器从协程内部向外部返回数据相对应，协程同样可以从外部获取数据，结合C++模板，还可以使得每次获取到的数据类型不同。
* [async.cpp](async.cpp)
  * 使用协程实现异步调用链