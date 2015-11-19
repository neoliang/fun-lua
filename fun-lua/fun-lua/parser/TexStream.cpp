//
//  TexStream.cpp
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "TexStream.h"
namespace Parser{
    class StringStream : public TexStream
    {
    private:
        std::shared_ptr<std::string> _stream;
        unsigned int _lineNum = 0;
        std::string::iterator iter;
        unsigned int _currentCol =0;
        
        void moveToNext()
        {
            if (empty()) {
                throw std::bad_exception();
            }
            ++iter;
            if (*iter == '\n') {
                ++_lineNum;
                _currentCol = 0;
            }
            else
            {
                ++_currentCol;
            }
        }
    public:
        StringStream(const std::string& str)
        {
            _stream = std::shared_ptr<std::string>(new std::string(str));
            iter = _stream->begin();
        }

    public:
        bool empty()const
        {
            return iter == _stream->end();
        }
        char get()const
        {
            if (empty()) {
                throw std::bad_exception();
            }
            return *iter;
        }
        unsigned int lineNum()const
        {
            return _lineNum;
        }
        unsigned int colNum()const
        {
            return _currentCol;
        }
        TexStream::PtrType next()const
        {
            auto temp = new StringStream(*this);
            temp->moveToNext();
            return PtrType(temp);
        }
    };
    
    TexStream::PtrType TexStream::fromString(const std::string &str)
    {
        return PtrType(new StringStream(str));
    }
}