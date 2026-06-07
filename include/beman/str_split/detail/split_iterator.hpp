// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_SPLIT_ITERATOR_HPP
#define BEMAN_STR_SPLIT_SPLIT_ITERATOR_HPP

#include <iterator>
#include <ranges>

namespace beman::str_split::detail {

template<class Parent>
class split_sentinel;

template<class Parent>
class split_iterator
{
    friend split_sentinel<Parent>;

    using base_iterator = Parent::base_iterator;

public:
    using iterator_concept = std::forward_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type = Parent::base_subrange;
    using difference_type = std::iter_difference_t<base_iterator>;

    split_iterator() = default;

    constexpr split_iterator(Parent& parent, base_iterator current, value_type next)
        : parent_(std::addressof(parent))
        , current_(std::move(current))
        , next_(std::move(next))
    {
    }

    constexpr base_iterator base() const
    {
        return current_;
    }

    constexpr value_type operator*() const
    {
        return {current_, next_.begin()};
    }

    constexpr split_iterator& operator++()
    {
        current_ = next_.begin();

        if (current_ == std::ranges::end(parent_->base_)) {
            trailing_empty_ = false;
        } else {
            current_ = next_.end();
            if (current_ == std::ranges::end(parent_->base_)) {
                trailing_empty_ = true;
                next_ = {current_, current_};
            } else {
                next_ = parent_->find_next(current_);
            }
        }

        return *this;
    }

    constexpr split_iterator operator++(int)
    {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    friend constexpr bool operator ==(const split_iterator& lhs, const split_iterator& rhs)
    {
        return lhs.current_ == rhs.current_ && lhs.trailing_empty_ == rhs.trailing_empty_;
    }

private:
    Parent* parent_ = nullptr;
    base_iterator current_{};
    value_type next_{};
    bool trailing_empty_ = false;
};

template<class Parent>
class split_sentinel
{
public:
    split_sentinel() = default;

    constexpr split_sentinel(Parent& parent)
        : end_(std::ranges::end(parent.base_))
    {
    }

    friend constexpr bool operator ==(const split_iterator<Parent>& lhs, const split_sentinel& rhs)
    {
        return lhs.current_ == rhs.end_ && !lhs.trailing_empty_;
    }

private:
    std::ranges::sentinel_t<decltype(std::declval<Parent>().base())> end_{};
};

}

#endif
