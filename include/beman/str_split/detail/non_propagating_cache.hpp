#ifndef BEMAN_STR_SPLIT_DETAIL_NON_PROPAGATING_CACHE_HPP
#define BEMAN_STR_SPLIT_DETAIL_NON_PROPAGATING_CACHE_HPP

#include <optional>
#include <type_traits>

namespace beman::str_split::detail
{

template<class T>
    requires std::is_object_v<T>
class non_propagating_cache
{
    std::optional<T> cache_;

public:
    non_propagating_cache() = default;

    constexpr non_propagating_cache(non_propagating_cache const&) noexcept
    {
    }

    constexpr non_propagating_cache(non_propagating_cache&& from) noexcept
    {
        from.cache_.reset();
    }

    constexpr auto operator =(non_propagating_cache const& from) noexcept -> non_propagating_cache&
    {
        if (this != std::addressof(from))
        {
            cache_.reset();
        }
        return *this;
    }

    constexpr auto operator =(non_propagating_cache&& from) noexcept -> non_propagating_cache&
    {
        from.cache_.reset();
        cache_.reset();
        return *this;
    }

    auto operator *() const -> T const&
    {
        return *cache_;
    }

    auto operator *() -> T&
    {
        return *cache_;
    }

    constexpr explicit operator bool() const noexcept
    {
        return cache_.has_value();
    }

    constexpr auto reset() noexcept -> void
    {
        cache_.reset();
    }

    template<class... Args>
    constexpr auto emplace(Args&&... args) noexcept -> T&
    {
        return cache_.emplace(std::forward<Args>(args)...);
    }
};

}

#endif
