// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_SPLIT_HPP
#define BEMAN_STR_SPLIT_SPLIT_HPP

#include <beman/str_split/detail/non_propagating_cache.hpp>
#include <beman/str_split/detail/split_iterator.hpp>

#include <ranges>
#include <functional>

namespace beman::str_split {

namespace detail {

template <class V, class Pattern>
concept splittable_view =
    std::ranges::view<V> && std::ranges::view<Pattern> &&
    std::indirectly_comparable<std::ranges::iterator_t<V>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>;

template <class Range>
using single_value_view_t = std::ranges::single_view<std::ranges::range_value_t<Range>>;

} // namespace detail

template <std::ranges::forward_range V, std::ranges::forward_range Pattern>
    requires detail::splittable_view<V, Pattern>
class split_view : public std::ranges::view_interface<split_view<V, Pattern>> {
  public:
    using iterator = detail::split_iterator<split_view>;
    friend iterator;

    using sentinel = detail::split_sentinel<split_view>;
    friend sentinel;

    split_view()
        requires std::default_initializable<V> && std::default_initializable<Pattern>
    = default;

    constexpr explicit split_view(V base, Pattern pattern) : base_(std::move(base)), pattern_(std::move(pattern)) {}

    template <std::ranges::forward_range R>
        requires std::constructible_from<V, std::ranges::views::all_t<R>> &&
                     std::constructible_from<Pattern, detail::single_value_view_t<R>>
    constexpr explicit split_view(R&& range, std::ranges::range_value_t<R> value)
        : base_(std::ranges::views::all(std::forward<R>(range))),
          pattern_(std::ranges::views::single(std::move(value))) {}

    constexpr V base() const&
        requires std::copy_constructible<V>
    {
        return base_;
    }

    constexpr V base() && { return std::move(base_); }

    constexpr iterator begin() {
        if (!cached_begin_) {
            cached_begin_.emplace(find_next(std::ranges::begin(base_)));
        }
        return {*this, std::ranges::begin(base_), *cached_begin_};
    }

    constexpr auto end() {
        if constexpr (std::ranges::common_range<V>) {
            return iterator(*this, std::ranges::end(base_), {});
        } else {
            return sentinel(*this);
        }
    }

  private:
    using base_iterator = std::ranges::iterator_t<V>;
    using base_subrange = std::ranges::subrange<base_iterator>;

    constexpr base_subrange find_next(base_iterator it) {
        auto [b, e] = std::ranges::search(std::ranges::subrange(it, std::ranges::end(base_)), pattern_);
        if (b != std::ranges::end(base_) && std::ranges::empty(pattern_)) {
            ++b;
            ++e;
        }
        return {b, e};
    }

    [[no_unique_address]] V                      base_{};
    [[no_unique_address]] Pattern                pattern_{};
    detail::non_propagating_cache<base_subrange> cached_begin_{};
};

template <class R, class Pattern>
split_view(R&&, Pattern&&) -> split_view<std::ranges::views::all_t<R>, std::ranges::views::all_t<Pattern>>;

template <class R>
split_view(R&&, std::ranges::range_value_t<R>)
    -> split_view<std::ranges::views::all_t<R>, detail::single_value_view_t<R>>;

namespace detail {

template <class R, class Pattern>
using split_view_t = decltype(split_view(std::declval<R>(), std::declval<Pattern>()));

template <std::move_constructible Pattern>
class split_adaptor_closure : public std::ranges::range_adaptor_closure<split_adaptor_closure<Pattern>> {
  public:
    constexpr explicit split_adaptor_closure(Pattern pattern) noexcept(std::is_nothrow_move_constructible_v<Pattern>)
        : pattern_(std::move(pattern)) {}

    template <class R>
    constexpr split_view_t<R, Pattern> operator()(R&& range) const& noexcept(
        std::is_nothrow_constructible_v<split_view_t<R, Pattern>, R, const Pattern&>) {
        return split_view(std::forward<R>(range), pattern_);
    }

    template <class R>
    constexpr split_view_t<R, Pattern>
    operator()(R&& range) && noexcept(std::is_nothrow_constructible_v<split_view_t<R, Pattern>, R, Pattern>) {
        return split_view(std::forward<R>(range), std::move(pattern_));
    }

  private:
    Pattern pattern_{};
};

template <class Pattern>
using split_adaptor_closure_t = split_adaptor_closure<std::decay_t<Pattern>>;

namespace split {

struct fn {
    template <class R, class Pattern>
    static constexpr split_view_t<R, Pattern>
    operator()(R&&       range,
               Pattern&& pattern) noexcept(std::is_nothrow_constructible_v<split_view_t<R, Pattern>, R, Pattern>) {
        return split_view(std::forward<R>(range), std::forward<Pattern>(pattern));
    }

    template <class Pattern>
    static constexpr split_adaptor_closure_t<Pattern> operator()(Pattern&& pattern) noexcept(
        std::is_nothrow_constructible_v<split_adaptor_closure_t<Pattern>, Pattern>) {
        return split_adaptor_closure(std::forward<Pattern>(pattern));
    }
};

} // namespace split

} // namespace detail

namespace views {

inline namespace rao {

inline constexpr auto split = detail::split::fn{};

}

} // namespace views

} // namespace beman::str_split

#endif
