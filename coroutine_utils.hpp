//
// Created by xinyang on 2021/5/3.
//

#ifndef _COROUTINE_UTILS_HPP_
#define _COROUTINE_UTILS_HPP_

#define COROUTINE_BEGIN         switch(_status) { case 0: ;
#define COROUTINE_END           default: ; }
#define COROUTINE_YIELD(expr)   do{ _status = __LINE__; return expr; case __LINE__: ; }while(0)
#define COROUTINE_RETURN(expr)  do{ _status = -1; return expr; }while(0)

class coroutine_base {
public:
    bool done() const { return _status < 0; }

protected:
    int _status = 0;
};

#endif /* _COROUTINE_UTILS_HPP_ */
