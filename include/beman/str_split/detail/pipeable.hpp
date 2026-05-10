#ifndef BEMAN_STR_SPLIT_DETAIL_PIPEABLE_HPP
#define BEMAN_STR_SPLIT_DETAIL_PIPEABLE_HPP

#include <concepts>
#include <functional>

namespace beman::str_split::views::detail
{

template<class Fn, class Rhs>
class pipeable
{
    Rhs arg_;

public:
    template<class Arg>
        requires std::constructible_from<Rhs, Arg>
    constexpr explicit pipeable(Arg&& arg)
        : arg_(std::forward<Arg>(arg))
    {
    }

    template<class Lhs>
        requires std::invocable<Fn, Lhs, Rhs const&>
    constexpr friend auto operator |(Lhs&& lhs, pipeable const& rhs)
        noexcept(std::is_nothrow_invocable_v<Fn, Lhs, Rhs const&>)
        -> std::invoke_result_t<Fn, Lhs, Rhs const&>
     {
         return std::invoke(Fn{}, std::forward<Lhs>(lhs), rhs);
     }

    template<class Lhs>
        requires std::invocable<Fn, Lhs, Rhs>
    constexpr friend auto operator |(Lhs&& lhs, pipeable&& rhs)
        noexcept(std::is_nothrow_invocable_v<Fn, Lhs, Rhs>)
        -> std::invoke_result_t<Fn, Lhs, Rhs>
     {
         return std::invoke(Fn{}, std::forward<Lhs>(lhs), std::forward<Rhs>(rhs.arg_));
     }
};

}

#endif
