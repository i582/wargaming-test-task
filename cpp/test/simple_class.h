#pragma once
#include "string"
#include "vcruntime.h"

using _STD string;

class SimpleClass
{
    string _name;

public:

    SimpleClass() = default;
    SimpleClass& operator=(const SimpleClass& right) = default;

    SimpleClass(const SimpleClass& right)
    {
        _name = right._name;
    }

    SimpleClass(SimpleClass&& right) noexcept
    {
        _name = std::move(right._name);
    }

    SimpleClass& operator=(SimpleClass&& right) noexcept
    {
        _name = std::move(right._name);
        return *this;
    }

    explicit SimpleClass(string name) : _name(std::move(name))
    {}

    _NODISCARD string name() const
    {
        return _name;
    }

    void name(const string& name)
    {
        _name = name;
    }
};