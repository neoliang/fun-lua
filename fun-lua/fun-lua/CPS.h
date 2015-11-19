//
//  CPS.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_CPS_h
#define fun_lua_CPS_h
namespace CPS {
    template<typename T,typename U>
    struct CPSType
    {
        typedef std::shared_ptr<CPSResult<T,U>> Result;
        typedef std::function<Result(const U&)> Fun;
        typedef T ElemntT;
        typedef U RemainT;
        
        //return
        static Fun ret(const T& t)
        {
            return [t](const U& inp)->Result
            {
                return Some(t,inp);
            };
        }
        
        //failure or zero
        static Fun failure(const std::string& msg)
        {
            return [msg](const U& inp)->Result
            {
                return None<T,U>(msg);
            };
        }
    };
    
    //concat or bind
    template<typename T1,typename T2,typename U>
    inline typename CPSType<T2,U>::Fun Bind(const typename CPSType<T1,U>::Fun& x,const std::function<typename CPSType<T2,U>::Fun (const T1&)>& y)
    {
        return [x,y](const U& inp)->typename CPSType<T2,U>::Result{
            auto r = x(inp);
            if (r->isNone()) {
                return None<T2,U>(r->errorMsg());
            }
            else
            {
                return (y(r->value()))(r->remain());
            }
            
        };
    }
    
    template<typename T,typename U>
    inline typename CPSType<T,U>::Fun ChooseN(const std::list<typename CPSType<T,U>::Fun>& ps)
    {
        return [ps](const U& inp)->typename CPSType<T,U>::Result{
            std::string errorMsg = "chooseerror : ";
            for (auto iter = ps.begin(); iter != ps.end(); ++iter) {
                auto r = (*iter)(inp);
                if (!r->isNone()) {
                    return r;
                }
                else
                {
                    errorMsg += r->errorMsg();
                }
            }
            return CPSResult<T,U>::None(errorMsg);
        };
    }
    
    
    //regular expression Choice
    template<typename T,typename U>
    inline typename CPSType<T,U>::Fun Choice(const typename CPSType<T,U>::Fun& x,const typename CPSType<T,U>::Fun& y)
    {
        return [x,y](const U& inp)->typename CPSType<T,U>::Result{
            auto r = x(inp);
            if (!r->isNone()) {
                return r;
            }
            else
            {
                return y(inp);
            }
        };
    }
    
    //reqular expression ?
    template<typename T,typename U>
    inline typename CPSType<T,U>::Fun Option(const typename CPSType<T,U>::Fun& x)
    {
        return Choice<T,U>(x, CPSType<T,U>::ret(T()));
    }
    
    //regular expression +
    template<typename T,typename U>
    typename CPSType<std::list<T>,U>::Fun Many1(const typename CPSType<T,U>::Fun& f);
    
    //regular expression *
    template<typename T,typename U>
    inline typename CPSType<std::list<T>,U>::Fun Many(const typename CPSType<T,U>::Fun& f)
    {
        return Option<std::list<T>,U>(Many1<T,U>(f));
    }
    
    template<typename T,typename U>
    inline typename CPSType<std::list<T>,U>::Fun Many1(const typename CPSType<T,U>::Fun& f)
    {
        return Bind<T, std::list<T>,U>(f,[f](T v){
            return Bind<std::list<T>, std::list<T>,U>(Many<T,U>(f), [v](std::list<T> vs)
                                                      {
                                                          vs.push_front (v);
                                                          return CPSType<std::list<T>,U>::ret(vs);
                                                      });
        });
        
    }
    
}

#endif
