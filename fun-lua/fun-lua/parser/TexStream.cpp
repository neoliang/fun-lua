//
//  TexStream.cpp
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//
#include <stdio.h>
#include <memory>
#include "TexStream.h"
namespace Parser{
    
    struct AutoFile{
        FILE* fp ;
        
        AutoFile(FILE* f)
        :fp(f)
        {
            
        }
        ~AutoFile()
        {
            fclose(fp);
        }
        static  std::shared_ptr<AutoFile> create(const std::string& file)
        {
            auto fp = fopen(file.c_str(), "r");
            if (fp == nullptr) {
                return nullptr;
            }
            return std::shared_ptr<AutoFile>(new AutoFile(fp));
        }
        
        typedef std::shared_ptr<AutoFile> Ptr;
    };
    
    class FileStream : public TexStream
    {

        
        unsigned int _lineNum = 0;
        unsigned int _currentCol =0;
        long _cpos = 0;
        long _endPos = -1;
        AutoFile::Ptr _file = nullptr;
        
        void moveToNext()
        {
            if (empty()) {
                throw std::bad_exception();
            }
            ++_cpos;
            if (!empty())
            {
                if(get() == '\n') {
                    ++_lineNum;
                    _currentCol = 0;
                }
                else
                {
                    ++_currentCol;
                }
            }
        }
        
    public:
        FileStream(const std::string& file)
        {
            _file = AutoFile::create(file);
            fseek(_file->fp,0,SEEK_END);
            _endPos = ftell(_file->fp);
            fseek(_file->fp,0,SEEK_SET);
        }
        
    public:
        bool empty()const
        {
            return _cpos >= _endPos;
        }
        char get()const
        {
            if (empty()) {
                throw std::bad_exception();
            }
            char c ;
            fseek(_file->fp, _cpos, SEEK_SET);
            fread(&c, sizeof(char), 1, _file->fp);
            return c;
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
            auto temp = new FileStream(*this);
            temp->moveToNext();
            return PtrType(temp);
        }
    };
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
            if (!empty()) {
                if (*iter == '\n') {
                    ++_lineNum;
                    _currentCol = 0;
                }
                else
                {
                    ++_currentCol;
                }
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
    TexStream::PtrType TexStream::fromFile(const std::string &file)
    {
        return PtrType(new FileStream(file));
    }
    TexStream::PtrType TexStream::fromString(const std::string &str)
    {
        return PtrType(new StringStream(str));
    }
}