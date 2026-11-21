#include <prgmr/delegate.hpp>
#include <prgmr/factory.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

void require(bool condition) {
    if (!condition) {
        throw std::runtime_error("test failed");
    }
}

int twice(int value) {
    return value * 2;
}

struct product {
    virtual ~product() = default;
    virtual int value() const = 0;
};

struct first_product : product {
    explicit first_product(int value) : value_(value) {}
    int value() const override { return value_; }
    int value_;
};

struct second_product : product {
    explicit second_product(int value) : value_(value) {}
    int value() const override { return value_ * 2; }
    int value_;
};

void test_delegate() {
    prgmr::delegate<int(int)> callback = twice;
    require(callback(3) == 6);

    int offset = 4;
    callback = [&offset](int value) { return value + offset; };
    require(callback(3) == 7);
    offset = 5;
    require(callback(3) == 8);

    callback = {};
    require(!callback);
    try {
        callback(3);
        require(false);
    } catch (const std::bad_function_call&) {
    }
}

void test_factory() {
    prgmr::factory<product, std::string, int> products;
    products.register_creator("default", [](int value) {
        return std::make_unique<first_product>(value);
    });
    require(products.create("default", 3)->value() == 3);

    products.register_creator("default", [](int value) {
        return std::make_unique<second_product>(value);
    });
    require(products.create("default", 3)->value() == 6);

    try {
        products.create("missing", 3);
        require(false);
    } catch (const std::out_of_range&) {
    }
    try {
        products.register_creator("default", {});
        require(false);
    } catch (const std::invalid_argument&) {
    }
    require(products.create("default", 3)->value() == 6);
}

void test_factory_arguments() {
    prgmr::factory<product, int, std::unique_ptr<int>, int&> products;
    products.register_creator(1, [](std::unique_ptr<int> value, int& count) {
        ++count;
        return std::make_unique<first_product>(*value);
    });
    int count = 0;
    require(products.create(1, std::make_unique<int>(9), count)->value() == 9);
    require(count == 1);
}

} // namespace

int main() {
    test_delegate();
    test_factory();
    test_factory_arguments();
}
