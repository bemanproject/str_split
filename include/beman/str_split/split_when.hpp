// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_SPLIT_WHEN_HPP
#define BEMAN_STR_SPLIT_SPLIT_WHEN_HPP

#include <beman/str_split/detail/non_propagating_cache.hpp>
#include <beman/str_split/detail/split_iterator.hpp>

#include <concepts>
#include <ranges>
#include <utility>

namespace beman::str_split {

namespace detail {

template <class T, class V>
concept searcher_for = std::is_object_v<T> && std::ranges::view<V> &&
                       requires(T search, std::ranges::subrange<std::ranges::iterator_t<V>> base) {
                           { auto(search(base)) } -> std::same_as<std::ranges::subrange<std::ranges::iterator_t<V>>>;
                       };

} // namespace detail

template <std::ranges::forward_range V, detail::searcher_for<V> Search>
class split_when_view : public std::ranges::view_interface<split_when_view<V, Search>> {
  public:
    using iterator = detail::split_iterator<split_when_view>;
    friend iterator;

    using sentinel = detail::split_sentinel<split_when_view>;
    friend sentinel;

    split_when_view()
        requires std::default_initializable<V> && std::default_initializable<Search>
    = default;

    constexpr explicit split_when_view(V base, Search search) : base_(std::move(base)), search_(std::move(search)) {}

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
        return search_(std::ranges::subrange(it, std::ranges::end(base_)));
    }

    [[no_unique_address]] V                      base_{};
    [[no_unique_address]] Search                 search_{};
    detail::non_propagating_cache<base_subrange> cached_begin_{};
};

namespace detail {

template <class R, class Search>
using split_when_view_t = decltype(split_when_view(std::declval<R>(), std::declval<Search>()));

template <std::move_constructible Search>
class split_when_adaptor_closure : public std::ranges::range_adaptor_closure<split_when_adaptor_closure<Search>> {
  public:
    constexpr explicit split_when_adaptor_closure(Search search) noexcept(std::is_nothrow_move_constructible_v<Search>)
        : search_(std::move(search)) {}

    template <class R>
    constexpr split_when_view_t<R, Search> operator()(R&& range) const& noexcept(
        std::is_nothrow_constructible_v<split_when_view_t<R, Search>, R, const Search&>) {
        return split_when_view(std::forward<R>(range), search_);
    }

    template <class R>
    constexpr split_when_view_t<R, Search>
    operator()(R&& range) && noexcept(std::is_nothrow_constructible_v<split_when_view_t<R, Search>, R, Search>) {
        return split_when_view(std::forward<R>(range), std::move(search_));
    }

  private:
    Search search_{};
};

template <class Search>
using split_when_adaptor_closure_t = split_when_adaptor_closure<std::decay_t<Search>>;

namespace split_when {

struct fn {
    template <class R, class Search>
    static constexpr split_when_view_t<R, Search>
    operator()(R&&      range,
               Search&& search) noexcept(std::is_nothrow_constructible_v<split_when_view_t<R, Search>, R, Search>) {
        return split_when_view(std::forward<R>(range), std::forward<Search>(search));
    }

    template <class Search>
    static constexpr split_when_adaptor_closure_t<Search> operator()(Search&& search) noexcept(
        std::is_nothrow_constructible_v<split_when_adaptor_closure_t<Search>, Search>) {
        return split_when_adaptor_closure(std::forward<Search>(search));
    }
};

} // namespace split_when

} // namespace detail

namespace views {

inline namespace rao {

inline constexpr auto split_when = detail::split_when::fn{};

}

} // namespace views

} // namespace beman::str_split

#endif // BEMAN_STR_SPLIT_SPLIT_WHEN_HPP
