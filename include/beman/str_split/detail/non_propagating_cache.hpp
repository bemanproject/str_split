#ifndef BEMAN_STR_SPLIT_DETAIL_NON_PROPAGATING_CACHE_HPP
#define BEMAN_STR_SPLIT_DETAIL_NON_PROPAGATING_CACHE_HPP

#include <optional>
#include <type_traits>

namespace beman::str_split::detail {

template <class T>
    requires std::is_object_v<T>
struct non_propagating_cache : std::optional<T> {
    non_propagating_cache() = default;

    constexpr non_propagating_cache(const non_propagating_cache&) noexcept {}

    constexpr non_propagating_cache(non_propagating_cache&& from) noexcept { this->reset(); }

    ~non_propagating_cache() = default;

    constexpr non_propagating_cache& operator=(const non_propagating_cache& from) noexcept {
        if (this != std::addressof(from)) {
            this->reset();
        }
        return *this;
    }

    constexpr non_propagating_cache& operator=(non_propagating_cache&& from) noexcept {
        this->reset();
        from.reset();
        return *this;
    }

    template <class It>
        requires std::is_constructible_v<T, std::iter_reference_t<const It&>>
    constexpr T&
    emplace_deref(const It& it) noexcept(std::is_nothrow_constructible_v<T, std::iter_reference_t<const It&>>) {
        return this->emplace(*it);
    }
};

} // namespace beman::str_split::detail

#endif
