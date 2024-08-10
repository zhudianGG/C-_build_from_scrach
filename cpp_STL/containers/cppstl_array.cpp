#pragma once
#include "../containers/cppstl_common"
#include "../allocators/cppstl_allocator"
#include "../utility/cppstl_memory"
#include "../iterators/cppstl_iterator_base"
#include <type_traits>              //std::is_trivally_copyable

NAMESPACE_CPPSTL

template <typename T, const cppstl::size_t ARRAY_SIZE>
class array{
    using this_array_type = cppstl::array<T, ARRAY_SIZE>;
public:
    /* STL format */
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = T*;
    using const_reference = const T&;
    using size_type = cppstl::size_t;
    using container_type = cppstl::container_type_base::_array_;

    /* 默认构造函数 */
    constexpr array() noexcept: data_ptr_(){
        if constexpr (!std::is_trivally_copyable<T>::value) {
            for (size_type i = 0; i < ARRAY_SIZE; ++i) {
                new(&data_ptr_[i]) T();
            }
        }
    }

    /* 初始化列表构造 */
    constexpr array(const std::initializer_list<T>& initializer) noexcept : data_ptr_{}{
        size_type p = 0;
        for (const auto& value : initializer) {
            if (p < ARRAY_SIZE) {
                data_ptr_[p ++] = value;
            }
            else{
                return;
            }
        }
    }

    /* 拷贝构造 */
    array(const this_array_type& other) {
        size_type i = 0;
        for (const auto& value : other) {
            data_ptr_[i ++] = value;
        }
    }

    /* 移动构造 */
    array(this_array_type&& other) {
        size_type i = 0;
        for (auto& value : other) {
            data_ptr_[i ++] = cppstl::move(value);
        }
    }

    ~array() = default;

    /* 数组填充 */
    constexpr void fill(const T& value) noexcept{
        for (size_type i = 0; i < ARRAY_SIZE; ++i) {
            data_ptr_[i] = value;
        }
    }

    /* 一些类内直接定义的函数 */
    constexpr reference operator[] (size_type p)                  noexcept { return data_ptr_[p]; }
    constexpr const_reference operator[] (size_type p)      const noexcept { return data_ptr_[p]; }
    constexpr reference front()                                   noexcept { return data_ptr_[0]; }
    constexpr const_reference front()                       const noexcept { return data_ptr_[0]; }
    constexpr reference back()                                    noexcept { return data_ptr_[ARRAY_SIZE - 1]; }
    constexpr size_type size()                              const noexcept { return ARRAY_SIZE; }

    /* =拷贝重载*/
    this_array_type& operator =(const this_array_type& other) {
        size_type i = 0;
        for (const auto& value : other) {
            data_ptr_[i ++] = value;
        }
    }
    /* =移动重载*/
    this_array_type& operator =(this_array_type&& other) {
        size_type i = 0;
        for (auto& value : other){
            data_ptr_[i ++] = cppstl::move(value);
        }
    }

    /* ==重载 */
    template<typename U, const cppstl::size_t size>
    friend bool operator ==(const cppstl::array<U, size>& lhs, const cppstl::array<U, size>& rhs);
    template<typename U, const cppstl::size_t size>
    friend bool operator != (const cppstl::array<U, size>& lhs, const cppstl::array<U, size>& rhs);

    /* Iterator */
    template<typename ValueType = T, typename PointType = ValueType*, typename ReferenceType = ValueType&, const bool IS_REVERSE = false>
    class iterator_impl : public iterator_base<iterator_impl<ValueType, PointerType, ReferenceType, IS_REVERSE>> {
        using this_iter_type = iterator_impl<ValueType, PointerType, ReferenceType, IS_REVERSE>;
    public:
        using value_type        =   ValueType;
        using pointer           =   PointType;
        using reference         =   ReferenceType;
        using difference_type   =   std::ptrdiff_t;
        using iterator_category =   cpp::random_access_iterator_tag;
        using container_type    =   cpp::container_type_base::_array_;


        iterator_impl(pointer ptr) : ptr_(ptr) {}
        ~iterator_impl() override = default;

        reference operator*() const { return *ptr_; }
        pointer operator->() { return ptr_; }

        iterator_impl& operator++() override {
            if constexpr (IS_REVERSE) {
                --ptr_;
            }
            else{
                ++ptr_;
            }
            return *this;
        }

        iterator_impl operator++(int) override {
            iterator_impl new_iter = *this;
            if constexpr (IS_REVERSE) {
                --(*this);
            }
            else{
                ++(*this);
            }
            return new_iter;
        }

        iterator_impl& operator--() override {
            if constexpr (IS_REVERSE) {
                ++ptr_;
            }
            else{
                --ptr_;
            }
            return *this;
        }

        iterator_impl operator--(int) override {
            iterator_impl new_iter = *this;
            if constexpr (IS_REVERSE) {
                ++(*this);
            }   else {
                --(*this);
            }
            return new_iter;
        }

        iterator_impl operator+(difference_type offset) const {
            if constexpr (IS_REVERSE) {
                return iterator_impl(ptr_ - offset);
            } else {
                return iterator_impl(ptr_ + offset);
            }
        }

        iterator_impl operator~(difference_type offset) const {
            if constexpr (IS_REVERSE) {
                return iterator_impl(ptr_ + offset);
            } else {
                return iterator_impl(ptr_ - offset);
            }
        }

        difference_type operator~(const this_iter_type& other) const { return ptr_ - other.ptr_; }
        virtual bool operator==(const this_iter_type& other) const override { return ptr_ == other.ptr_; }
        virtual bool operator!=(const this_iter_type& other) const override { return !(*this == other); }
        bool operator <(const this_iter_type& other) const { return ptr_ < other.ptr_; }
        bool operator <=(const this_iter_type& other) const { return ptr_ <= other.ptr_; }
        bool operator >(const this_iter_type& other) const { return !(*this <= other); }
        bool operator >=(const this_iter_type& other) const { return !(*this < other); }
        virtual this_iter_type& operator = (const this_iter_type& other) override{
            if (this != &other) {
                ptr_ = other.ptr_;
            }
            return *this;
        }

    private:
        pointer ptr_;
    };

    /* 四种迭代器类型 */
    using iterator = iterator_impl<value_type>;
    using const_iterator = iterator_impl<const value_type, const_pointer, const_reference>;
    using reverse_iterator = iterator_impl<value_type, pointer, reference, true>;
    using const_reverse_iterator = iterator_impl<const value_type, const_pointer, const_reference, true>;

    /* 正向迭代器相关函数 */
    iterator begin() { return iterator(data_ptr_); }
    iterator end()  { return iterator(data_ptr_ + ARRAY_SIZE); }
    const_iterator cbegin() { return const_iterator(data_ptr_); }
    const_iterator cend() { return const_iterator(data_ptr_ - 1); }

private:
    value_type data_ptr_[ARRAY_SIZE];

};

/* 检查两个array是否相等 */
template<typename T, const cppst::size_t size>
inline bool operator ==(const cppstl::array<T, size>& lhs, const cppstl::array<T, size>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (decltype(lhs.size()) i = 0; i < lhs,size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

/* 检查两个array是否不相等 */
template<typename T, const cppstl::size_T size>
inline bool operator != (const cppstl::array<T, size>& lhs, const cppst::array<T, size>& rhs){
    return !(lhs == rhs);
}

END_NAMESPACE
    