//
// Created by xinyang on 2021/5/3.
//

#ifndef _COROUTINE_UTILS_HPP_
#define _COROUTINE_UTILS_HPP_

#include <stdexcept>

#define COROUTINE_CHECK_IMPL(n)     if(_status == n) goto status_##n;
#define COROUTINE_CHECK_0           COROUTINE_CHECK_IMPL(0)
#define COROUTINE_CHECK_1           COROUTINE_CHECK_0 else COROUTINE_CHECK_IMPL(1)
#define COROUTINE_CHECK_2           COROUTINE_CHECK_1 else COROUTINE_CHECK_IMPL(2)
#define COROUTINE_CHECK_3           COROUTINE_CHECK_2 else COROUTINE_CHECK_IMPL(3)
#define COROUTINE_CHECK_4           COROUTINE_CHECK_3 else COROUTINE_CHECK_IMPL(4)
#define COROUTINE_CHECK_5           COROUTINE_CHECK_4 else COROUTINE_CHECK_IMPL(5)
#define COROUTINE_CHECK_6           COROUTINE_CHECK_5 else COROUTINE_CHECK_IMPL(6)
#define COROUTINE_CHECK_7           COROUTINE_CHECK_6 else COROUTINE_CHECK_IMPL(7)
#define COROUTINE_CHECK_8           COROUTINE_CHECK_7 else COROUTINE_CHECK_IMPL(8)
#define COROUTINE_CHECK_9           COROUTINE_CHECK_8 else COROUTINE_CHECK_IMPL(9)

#define COROUTINE_CHECK(n)          do{ COROUTINE_CHECK_##n else throw std::logic_error("unreachable status!"); status_0: (void)0; }while(0)
#define COROUTINE_YIELD(n, expr)    do{ _status = n; return expr; status_##n: (void)0; }while(0)
#define COROUTINE_RETURN(expr)      do{ _status = -1; return expr; }while(0)

class coroutine_base {
public:
    bool done() const { return _status < 0; }

protected:
    int _status = 0;
};

#endif /* _COROUTINE_UTILS_HPP_ */
