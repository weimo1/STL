#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>


template<typename T>
class Vector{
public:
    using value_type=T;
    using reference=T&;
    using difference_type=std::ptrdiff_t;
    using iterator=T*;
    using const_reference=const T&;
    using const_iterator=const T *;

private:
    T * m_start;
    T * m_finish;
    T * m_end_of_storage;

    T* allocate(size_t n){
        return static_cast<T*> (operator new(n*sizeof(value_type)));
    }
    void deallocate(T *p){
        operator delete (p);
    }

public:
    void show() const
    {
        for(const value_type &val: *this){
            std::cout<<val<<" ";
        }
        std::cout<<'\n';
        std::cout<<size()<<"    "<<capacity()<<"\n";

    }
    Vector()
    :m_start(nullptr),m_finish(nullptr),m_end_of_storage(nullptr){}


    Vector(size_t n,const value_type& val=value_type()){
        m_start=allocate(n);
        std::uninitialized_fill_n(m_start, n, val);
        m_finish=m_end_of_storage=m_start+n;
    }
    template<class InputIt>
    Vector(InputIt first,InputIt last){
        size_t n=std::distance(first, last);
        m_start=allocate(n);
        std::uninitialized_copy(first, last, m_start);
        m_finish=m_end_of_storage=m_start+n;
    }
    Vector(const Vector& other):Vector(other.begin(),other.end()){}

    Vector(Vector&& other) noexcept{
        m_start=other.m_start;
        m_finish=other.m_finish;
        m_end_of_storage=other.m_end_of_storage;
        other.m_start=other.m_end_of_storage=other.m_finish=nullptr;
    }

    void swap(Vector&other){
        std::swap(m_start,other.m_start);
        std::swap(m_finish,other.m_finish);  
        std::swap(m_end_of_storage,other.m_end_of_storage);
    }
    void reserve(size_t n){
        if(capacity()>=n){
            return ;

        }
        iterator new_start=allocate(n);
        iterator new_finish=std::uninitialized_copy(begin(), end(), new_start);
        std:destroy(begin(),end());
        deallocate(m_start);
        m_start=new_start;
        m_finish=new_finish;
        m_end_of_storage=m_start+n;
    }
    
    Vector& operator=(const Vector& other){
        if(this==&other)return *this;
        if(other.size()>capacity()){
            Vector tmp(other);
            tmp.swap(*this);
            return *this;
        }else if(other.size()>size()){
            std::copy(other.begin(), other.begin()+size(),m_start);
            m_finish=std::uninitialized_copy(other.begin()+size(), other.end(),m_finish); 
        }else{
            iterator new_finish=std::copy(other.begin(), other.end(),m_start);
            std:destroy(new_finish,end());
            m_finish=new_finish;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept{
        if(this==&other){
            return *this;
        }
        std:destroy(begin(),end());
        deallocate(m_start);
        m_start=other.m_start;
        m_finish=other.m_finish;
        m_end_of_storage=other.m_end_of_storage;
        other.m_start=other.m_end_of_storage=other.m_finish=nullptr;
        return *this;
    }

    reference operator[](size_t n){
        return *(begin()+n);
    }
    const_reference operator[](size_t n)const {
        return *(begin()+n);
    }

    reference front(){
        return *begin();
    }

    const_reference fron()const{
        return *begin();
    }

    reference back(){
        return *(end()-1);
    }

    const_reference back()const{
        return *(end()-1);
    }

    T* data(){
        return begin();
    }


    bool empty()const{
        return begin()==end();
    }

    ~Vector(){
        std:destroy(begin(),end());
        deallocate(m_start);
    }
    size_t size()const{
        return m_finish-m_start;
    }
    size_t capacity()const{
        return m_end_of_storage-m_start;
    }
    iterator begin(){
        return  m_start;
    }
    iterator end(){
        return m_finish;
    }
    iterator begin() const{
        return  m_start;
    }
    iterator end() const{
        return m_finish;
    }
};