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
            return std::make_shared<AutoFile>(fp);
        }
        
        typedef std::shared_ptr<AutoFile> Ptr;
    };
    
    class FileStream : public TexStream
    {
        unsigned int _lineNum = 1;
        unsigned int _currentCol =1;
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
                    _currentCol = 1;
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
        
        virtual char lookAhead(unsigned int s)
        {
            if (_cpos + s >= _endPos) {
                return  -1;
            }
            char c ;
            fseek(_file->fp, _cpos+s, SEEK_SET);
            fread(&c, sizeof(char), 1, _file->fp);
            return c;
        }
        
        TexStream::PtrType next(unsigned int i =1)const
        {
            auto temp = std::make_shared<FileStream>(*this);
            while (i >0) {
                temp->moveToNext();
                --i;
            }
            return temp;
        }
    };
    class StringStream : public TexStream
    {
    private:
        std::shared_ptr<std::string> _stream;
        unsigned int _lineNum = 1;
        std::string::iterator iter;
        unsigned int _currentCol =1;
        
        void moveToNext()
        {
            if (empty()) {
                throw std::bad_exception();
            }
            ++iter;
            if (!empty()) {
                if (*iter == '\n') {
                    ++_lineNum;
                    _currentCol = 1;
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
            _stream = std::make_shared<std::string>(str);
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
        virtual char lookAhead(unsigned int s)
        {
            if (iter + s >= _stream->end()) {
                return  -1;
            }
            return *(iter + s);
        }
        TexStream::PtrType next(unsigned int i = 1)const
        {
            auto temp = std::make_shared<StringStream>(*this);
            while (i > 0) {
                temp->moveToNext();
                --i;
            }
            return temp;
        }
    };
    TexStream::PtrType TexStream::fromFile(const std::string &file)
    {
        return std::make_shared<FileStream>(file);
    }
    TexStream::PtrType TexStream::fromString(const std::string &str)
    {
        return std::make_shared<StringStream>(str);
    }
}