//
//  CPS.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_CPS_h
#define fun_lua_CPS_h
#include <string>
#include <list>
#include "CPSResult.h"
namespace CPS {
    template<typename T,typename U>
    struct CPSType
    {
        typedef std::shared_ptr<CPSResult<T,U>> Result;
        typedef std::function<Result(const U&)> FunT;
        typedef T ElemntT;
        typedef U RemainT;
        
        
        FunT fun;
        std::string label;
        //return
        static CPSType<T,U> ret(const T& t,const std::string& lable="unkown")
        {
            return {
                [t](const U& inp)->Result
                {
                    return Some(t,inp);
                },
                lable
            };
        }
        
        //failure or zero
        static CPSType<T,U> failure(const std::string& msg,const std::string& lable = "failure unkown")
        {
            return {
                [msg](const U& inp)->Result
                {
                    return None<T,U>(msg);
                },
                lable
            };
        }
    };
    
    //concat or bind
    template<typename T1,typename T2,typename U>
    inline CPSType<T2,U> Bind(const CPSType<T1,U>& x,const std::function<CPSType<T2,U> (const T1&)>& y,const std::string& lable = "")
    {
        
        return{
            [=](const U& inp)->typename CPSType<T2,U>::Result{
                auto r = x.fun(inp);
                if (r->isNone()) {
                    return None<T2,U>(r->errorMsg());
                }
                else
                {
                    return (y(r->value())).fun(r->remain());
                }
            },
            lable
        };
    }
#define CONSF(rt,it,e1,r) CPS::Bind<decltype(e1.fun(it()))::element_type::ElemntT,rt,it>(e1,[=](const decltype(e1.fun(it()))::element_type::ElemntT& r) ->CPS::CPSType<rt,it> {
#define CONS(rt,it,e1,r) return CPS::Bind<decltype(e1.fun(it()))::element_type::ElemntT,rt,it>(e1,[=](const decltype(e1.fun(it()))::element_type::ElemntT& r) ->CPS::CPSType<rt,it> {

#define RETF(e,it) CPS::CPSType<decltype(e),it>::ret(e)
#define RET(e,it) return CPS::CPSType<decltype(e),it>::ret(e)
#define ENDCONS })
    template<typename T,typename U>
    inline CPSType<T,U> ChooseN(const std::list<CPSType<T,U>>& ps,const std::string& lable="")
    {
        return {
            [=](const U& inp)->typename CPSType<T,U>::Result{
                for (auto iter = ps.begin(); iter != ps.end(); ++iter) {
                    auto r = (*iter).fun(inp);
                    if (!r->isNone()) {
                        return r;
                    }
                }
                return None<T,U>("choose error");
            },
            lable
        };
    }
    
    
    //regular expression Choice
    template<typename T,typename U>
    inline CPSType<T,U> Choice(const CPSType<T,U>& x,const CPSType<T,U>& y,const std::string& lable="")
    {
        return{
            [=](const U& inp)->typename CPSType<T,U>::Result{
                auto r = x.fun(inp);
                if (!r->isNone()) {
                    return r;
                }
                else
                {
                    return y.fun(inp);
                }
            },
            lable
        };
    }
    
    //reqular expression ?
    template<typename T,typename U>
    inline CPSType<T,U> Option(const CPSType<T,U>& x)
    {
        return Choice<T,U>(x, CPSType<T,U>::ret(T()));
    }
    
    //regular expression +
    template<typename T,typename U>
    CPSType<std::list<T>,U> Many1(const CPSType<T,U>& f);
    
    //regular expression *
    template<typename T,typename U>
    inline CPSType<std::list<T>,U> Many(const CPSType<T,U>& f)
    {
        return Option<std::list<T>,U>(Many1<T,U>(f));
    }
    
    template<typename T,typename U>
    inline  CPSType<std::list<T>,U> Many1(const CPSType<T,U>& f)
    {
        return Bind<T, std::list<T>,U>(f,[f](T v){
            return Bind<std::list<T>, std::list<T>,U>(Many<T,U>(f), [v](std::list<T> vs)
                                                      {
                                                          vs.push_front (v);
                                                          return CPSType<std::list<T>,U>::ret(vs);
                                                      });
        });
        
    }
    
    template<typename T ,typename T2,typename U>
    static CPSType<T2,U> fmap(CPSType<T,U>& x,const std::function<T2(const T& )>& f,const std::string& lable = "")
    {
        return{
            [x,f,lable](const U& inp)->typename CPSType<T2,U>::Result{
                auto r = x.fun(inp);
                if(r->isNone())
                {
                    return None<T2, U>(r->errorMsg());
                }
                else
                {
                    auto nv = f(r->value());
                    return Some(nv,r->remain());
                }
            },
            lable
        };
    }
    
}

#endif
