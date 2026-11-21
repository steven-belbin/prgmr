#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace prgmr {

template <typename Base, typename Key = std::string, typename... Args>
class factory {
public:
    using creator_type = std::function<std::unique_ptr<Base>(Args...)>;

    void register_creator(Key key, creator_type creator) {
        if (!creator) {
            throw std::invalid_argument("factory creator must not be empty");
        }
        creators_.insert_or_assign(std::move(key), std::move(creator));
    }

    std::unique_ptr<Base> create(const Key& key, Args... args) const {
        return creators_.at(key)(std::forward<Args>(args)...);
    }

private:
    std::unordered_map<Key, creator_type> creators_;
};

} // namespace prgmr
