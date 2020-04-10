#pragma once
#include "string"

using std::string;

class SimpleClass
{
private:
    string _name;


public:
    SimpleClass() = default;

    explicit SimpleClass(string name) : _name(std::move(name)) {}

    SimpleClass(const SimpleClass& right)
    {
        _name = right._name;
    }

    SimpleClass(SimpleClass&& right) noexcept
    {
        _name = std::move_if_noexcept(right._name);
    }

    SimpleClass& operator=(const SimpleClass& right) = default;

    SimpleClass& operator=(SimpleClass&& right) noexcept
    {
        _name = std::move_if_noexcept(right._name);
        return *this;
    }


public:
    _NODISCARD string name() const noexcept
    {
        return _name;
    }

    void name(const string& name) noexcept
    {
        _name = name;
    }

};