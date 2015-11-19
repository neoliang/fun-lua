//
//  CPSResult.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_CPSResult_h
#define fun_lua_CPSResult_h
#include <exception>
#include <memory>
#include <string>
#include <functional>

namespace CPS {
    
    
    template<typename T,typename U>
    class CPSResult
    {
    public:
        typedef T ElemntT;
        typedef U RemainT;
        virtual ~CPSResult(){}
        virtual bool isNone()const = 0;
        virtual const T& value() const = 0;
        virtual const U& remain() const = 0;
        virtual const std::string& errorMsg()const =0;
        
    };
    
    template<typename T,typename U>
    class ParserResultSucc : public CPSResult<T,U>
    {
        
        T _value;
        U _remain;
    public:
        
        ParserResultSucc(const T& v,const U& r)
        :_value(v)
        ,_remain(r){}
        
        
        bool isNone()const
        {
            return false;
        }
        
        const T& value()const
        {
            return _value;
        }
        const U& remain() const
        {
            return _remain;
        }
        const std::string& errorMsg()const
        {
            throw std::bad_exception();
        }
    };
    template<typename T,typename U>
    class ParserResultNone : public CPSResult<T,U>
    {
        std::string _errorMsg;
    public:
        ParserResultNone(const std::string& msg)
        :_errorMsg(msg){}
        bool isNone()const
        {
            return true;
        }
        
        const T& value()const
        {
            throw std::bad_exception();
        }
        const U& remain() const
        {
            throw std::bad_exception();
        }
        const std::string& errorMsg()const
        {
            return _errorMsg;
        }
    };
    
    
    template<typename T,typename U>
    inline std::shared_ptr<CPSResult<T,U>> Some(const T& v, const U& r)
    {
        return std::shared_ptr<CPSResult<T,U>> (new ParserResultSucc<T,U>(v,r));
    }
    template<typename T,typename U>
    inline std::shared_ptr<CPSResult<T,U>> None(const std::string& errorMsg)
    {
        return std::shared_ptr<CPSResult<T,U>> (new ParserResultNone<T,U>(errorMsg));
    }
}

#endif
