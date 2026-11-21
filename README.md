# prgmr

prgmr is a header-only C++17 library providing delegates and a
runtime-configurable factory.

## Delegates

`prgmr::delegate<Signature>` is a `std::function` with the given function
signature. It can hold functions, lambdas, and other compatible callables.
As with `std::function`, calling an empty delegate throws
`std::bad_function_call`.

```cpp
#include <prgmr/delegate.hpp>

prgmr::delegate<int(int)> double_value = [](int value) { return 2 * value; };
int result = double_value(3); // 6
```

## Factory

`prgmr::factory<Base, Key, Args...>` maps keys to creators returning
`std::unique_ptr<Base>`. The key defaults to `std::string`; constructor
arguments are specified after the key type. Registering an existing key
replaces its creator, allowing implementations to be selected at runtime.
Creators can capture configuration, and return a compatible derived type.
When a creator returns a derived object, `Base` must have a virtual destructor
so that deleting the resulting `std::unique_ptr<Base>` is safe.

```cpp
#include <prgmr/factory.hpp>
#include <memory>

struct service {
    virtual ~service() = default;
};
struct concrete_service : service {};

prgmr::factory<service> services;
services.register_creator("default", [] {
    return std::make_unique<concrete_service>();
});
auto instance = services.create("default");
```

Creating an unregistered key throws `std::out_of_range`. Registering an
empty creator throws `std::invalid_argument` without changing the existing
registration. Creators are responsible for the value they return, including
whether it is null. The factory uses `std::unordered_map`, so key types must
be hashable and equality-comparable. Synchronize concurrent access if any
thread modifies the factory or a creator's state.

## Build and test

The headers can be used directly from `include/`, or through the CMake
interface target `prgmr`. To build and run the tests:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
