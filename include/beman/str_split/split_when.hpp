// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_SPLIT_WHEN_HPP
#define BEMAN_STR_SPLIT_SPLIT_WHEN_HPP

// TODO(bartholomaios): properly support conditional compilation with modules
// TODO(bartholomaios): review noexcept specifications

#include <beman/str_split/detail/non_propagating_cache.hpp>
#include <beman/str_split/detail/pipeable.hpp>

#include <concepts>
#include <ranges>
#include <utility>

namespace beman::str_split
{

namespace detail
{

template<class R>
using subrange_t = std::ranges::subrange<std::ranges::iterator_t<R>>;

template<class T, class View>
concept view_searcher_for =
    std::is_object_v<T>
    && std::ranges::view<View>
    && requires(T searcher, View view)
    {
        { searcher(view) } -> std::same_as<subrange_t<View>>;
        { searcher(std::move(view)) } -> std::same_as<subrange_t<View>>;
    };

}

template<std::ranges::forward_range View, detail::view_searcher_for<View> Searcher>
class split_when_view : public std::ranges::view_interface<split_when_view<View, Searcher>>
{
    [[no_unique_address]] View base_;
    [[no_unique_address]] Searcher searcher_;
    detail::non_propagating_cache<detail::subrange_t<View>> cache_;

    constexpr auto find_next(std::ranges::iterator_t<View> it) -> detail::subrange_t<View>
    {
        return searcher_(std::ranges::subrange(it, std::ranges::end(base_)));
    }

public:
    class iterator;
    class sentinel;

    split_when_view()
        requires std::default_initializable<View> && std::default_initializable<Searcher>
    = default;

    explicit constexpr split_when_view(View view, Searcher searcher)
        : base_(std::move(view))
        , searcher_(std::move(searcher))
    {
    }

    constexpr auto base() const& -> View
        requires std::copy_constructible<View>
    {
        return base_;
    }

    constexpr auto base() && -> View
    {
        return std::move(base_);
    }

    constexpr auto searcher() const& -> Searcher const&
    {
        return searcher_;
    }

    constexpr auto searcher() && -> Searcher&&
    {
        return std::move(searcher_);
    }

    constexpr auto begin() -> iterator
    {
        if (!cache_)
        {
            cache_.emplace(find_next(std::ranges::begin(base_)));
        }

        return {*this, std::ranges::begin(base_), *cache_};
    }

    constexpr auto end() noexcept -> sentinel
    {
        return {*this};
    }
};

template<class View, class Searcher>
split_when_view(View&&, Searcher) -> split_when_view<std::ranges::views::all_t<View>, Searcher>;

template<std::ranges::forward_range View, detail::view_searcher_for<View> Searcher>
class split_when_view<View, Searcher>::iterator
{
    friend class sentinel;

    split_when_view* parent_ = nullptr;
    [[no_unique_address]] std::ranges::iterator_t<View> current_{};
    [[no_unique_address]] detail::subrange_t<View> next_{};
    bool has_trailing_empty_ = false;

public:
    using iterator_concept = std::forward_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type = detail::subrange_t<View>;
    using difference_type = std::ranges::range_difference_t<View>;

    iterator() = default;

    constexpr iterator(split_when_view& parent, std::ranges::iterator_t<View> current, value_type next)
        : parent_(std::addressof(parent))
        , current_(std::move(current))
        , next_(std::move(next))
    {
    }

    constexpr auto base() const -> std::ranges::iterator_t<View>
    {
        return current_;
    }

    constexpr auto operator *() const -> value_type
    {
        return {current_, next_.begin()};
    }

    constexpr auto operator ++() -> iterator&
    {
        current_ = next_.begin();

        if (current_ == std::ranges::end(parent_->base_))
        {
            has_trailing_empty_ = false;
        }
        else
        {
            current_ = next_.end();
            if (current_ == std::ranges::end(parent_->base_))
            {
                has_trailing_empty_ = true;
                next_ = {current_, current_};
            }
            else
            {
                next_ = parent_->find_next(current_);
            }
        }

        return *this;
    }

    constexpr auto operator ++(int) -> iterator
    {
        auto prev = *this;
        ++*this;
        return prev;
    }

    friend constexpr auto operator ==(iterator const& lhs, iterator const& rhs) -> bool
    {
        return lhs.current_ == rhs.current_ && lhs.has_trailing_empty_ == rhs.has_trailing_empty_;
    }
};

template<std::ranges::forward_range View, detail::view_searcher_for<View> Searcher>
class split_when_view<View, Searcher>::sentinel
{
    std::ranges::sentinel_t<View> end_{};

public:
    sentinel() = default;

    constexpr sentinel(split_when_view& parent)
        : end_(std::ranges::end(parent.base_))
    {
    }

    friend auto operator ==(iterator const& lhs, sentinel const& rhs) -> bool
    {
        return lhs.current_ == rhs.end_ && !lhs.has_trailing_empty_;
    }
};


namespace views
{

namespace detail
{

template<class View, class Searcher>
using split_when_view_t = decltype(split_when_view(std::declval<View>(), std::declval<Searcher>()));

struct split_when_fn
{
    template<class View, class Searcher>
    static constexpr auto operator ()(View&& view, Searcher&& searcher)
        noexcept(std::is_nothrow_constructible_v<split_when_view_t<View, Searcher>, View, Searcher>)
        -> split_when_view_t<View, Searcher>
    {
        return split_when_view(std::forward<View>(view), std::forward<Searcher>(searcher));
    }

    template<class Searcher>
        requires std::constructible_from<std::remove_cvref_t<Searcher>, Searcher>
    static constexpr auto operator ()(Searcher&& searcher)
        noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<Searcher>, Searcher>)
    {
        return detail::pipeable<split_when_fn, Searcher>(std::forward<Searcher>(searcher));
    }
};

}

inline constexpr auto split_when = detail::split_when_fn{};

}

}

#endif // BEMAN_STR_SPLIT_SPLIT_WHEN_HPP
