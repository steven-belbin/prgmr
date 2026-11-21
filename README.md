# prgmr

`prgmr` (namespace `prgrmr`) is a C++20 header-only library focused on providing reusable implementations of common design patterns and coding building blocks. By encapsulating boilerplate around delegates, factories, optional invocation, and type/invocable constraints, it lets C++20 applications focus on implementing domain features and functionality.

## Overview

- **C++20 Standard**: Built with C++20 concepts, `requires` clauses, `consteval` utilities, and standard library facilities (`<concepts>`, `<functional>`, `<memory>`, `<optional>`, `<tuple>`, `<unordered_map>`).
- **Header-Only**: Drop-in `.hpp` headers with no separate build step required.
- **Namespace**: All components reside in `namespace prgrmr` (with concepts under `prgrmr::concepts`).

## Components

### Delegates
- [`delegate.hpp`](delegate.hpp) — `prgrmr::delegate<TInvocable>`
  A movable, non-copyable wrapper around `std::function` that explicitly models an empty (unassigned) state. Supports `invoke` / `operator()`, `invoke_optional` (returning `std::optional`, or `std::nullopt` when unassigned), `reset`, `swap`, and `operator bool()`.
- [`delegates.hpp`](delegates.hpp) — `prgrmr::delegates<TInvocables...>`
  A tuple-backed collection of one or more `delegate` instances configurable at runtime. Delegates can be registered, reset, and invoked by signature/invocable type, by tuple index, or by automatic argument-type deduction.
- [`delegates_map.hpp`](delegates_map.hpp) — `prgrmr::delegates_map<TKey, TInvocables...>`
  An `std::unordered_map`-backed container associating keys of type `TKey` with `delegates<TInvocables...>`, enabling runtime key-based registration, lookup (`get`, `at`, `operator[]`), and invocation (`invoke`, `invoke_optional`).

### Factory Pattern
- [`factory.hpp`](factory.hpp) — `prgrmr::factory<TClass, TInvocables...>`
  A delegate-composed factory for constructing `TClass` instances across one or more constructor signatures. Constrains all invocables to share the same result type (`TClass*`, `std::unique_ptr<TClass>`, `std::shared_ptr<TClass>`, or a type convertible to `TClass*`) and provides `make`, `make_unique`, and `make_shared` (by signature, deduced arguments, or index), returning a default-constructed result (`nullptr`) when a delegate is unassigned.
- [`factory_map.hpp`](factory_map.hpp) — `prgrmr::factory_map<TClass, TKey, TInvocables...>`
  An `std::unordered_map`-backed registry mapping keys of type `TKey` to `factory<TClass, TInvocables...>` instances for runtime key-based registration and object creation (`make`, `make_unique`, `make_shared`).

### Functional & Utility Helpers
- [`functional.hpp`](functional.hpp) — `prgrmr::invoke_optional`, `prgrmr::apply_optional`
  Utility functions that invoke or apply an `InvocableCanBeEmptyWithResult` callable when defined and return its result wrapped in `std::optional` (`std::nullopt` when empty).
- [`class_name.hpp`](class_name.hpp) — `prgrmr::class_name`
  `constexpr` helper extracting a type's unqualified name from `typeid(TClass).name()`.

### C++20 Concepts & Traits
- [`concepts/concepts.hpp`](concepts/concepts.hpp) — Umbrella header including all concept definitions.
- [`concepts/arguments.hpp`](concepts/arguments.hpp) — `prgrmr::concepts::arguments`
  Variadic template parameter pack count concepts (`IsCountEqual`, `IsEmpty`, `IsNotEmpty`, `IsAtLeast`, `IsSingle`, `IsPair`, `IsAtMost`).
- [`concepts/invocable.hpp`](concepts/invocable.hpp) — `prgrmr::concepts::invocable`
  Traits and concepts for inspecting and constraining invocables (`invocable_traits`, `invocable_result_type`, `invocable_parameters_type`, `invocable_signature_type`, `Invocable`, `CanInvoke`, `CanInvokeAny`, `IsSameResultType`, `AreInvocablesSignaturesUnique`, `AreInvocablesParametersUnique`, `InvocableCanBeEmpty`, `InvocableWithoutResult`, `InvocableWithResult`, `InvocableCanBeEmptyWithResult`).

## Quick Examples

### Delegate & Optional Invocation
```cpp
#include "delegate.hpp"

prgrmr::delegate<int(int, int)> on_add;

if (auto result = on_add.invoke_optional(2, 3); !result) {
    // Delegate is currently unassigned
}

on_add.reset([](int a, int b) { return a + b; });
auto sum = on_add.invoke_optional(2, 3); // std::optional<int>{5}
```

### Multi-Signature Delegates
```cpp
#include "delegates.hpp"
#include <string>

prgrmr::delegates<int(int), std::string(const std::string&)> handlers;
handlers.reset([](int x) { return x * 2; });
handlers.reset([](const std::string& s) { return s + "!"; });

int doubled = handlers.invoke(21);              // 42
auto text = handlers.invoke_optional(std::string{"hi"}); // std::optional<std::string>{"hi!"}
```

### Factory & Factory Map
```cpp
#include "factory_map.hpp"
#include <memory>
#include <string>

struct Shape { virtual ~Shape() = default; };
struct Circle : Shape {};

prgrmr::factory_map<Shape, std::string, std::unique_ptr<Shape>()> shapes;
shapes.reset("circle", []() -> std::unique_ptr<Shape> {
    return std::make_unique<Circle>();
});

std::unique_ptr<Shape> circle = shapes.make_unique("circle");
std::shared_ptr<Shape> shared_circle = shapes.make_shared("circle");
```

## License

Licensed under the [Boost Software License - Version 1.0](LICENSE).

