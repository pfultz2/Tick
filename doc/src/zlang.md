ZLang support
=============

[ZLang](https://github.com/pfultz2/ZLang) is supported for some of the macros. The macros are in the `tick` namespace. For example,
```cpp
$(trait is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
