#ifndef AS_TUPLE_HPP_INCLUDED
#define AS_TUPLE_HPP_INCLUDED

#include <tuple>
#include <type_traits>

namespace ez::detail
{
    inline constexpr size_t max_fields = 106;

    // as_tuple returns the fields of an aggregate as a tuple, in order
    template <typename T>
    inline auto as_tuple(T&&, std::integral_constant<size_t, 0>)
    {
        return std::tuple{};
    }

    // Overlaying a tuple onto the object violates strict-aliasing, requires a custom tuple
    // implementation to ensure layout and has problems with nested/enum/non-pod types.
    // In short, it was a very risky way of doing things unsatisfactorily prior C++17.
    // So instead we brute force with "all" possible field counts with structured bindings.
    // Unfortunately, this means we have a limit on field counts.
    // Current limit of 64 has absolutely no reason whatsoever, 42 might have been better. Ah well.
    // The macro is undefined at the end.
    // Returns the fields as a tuple of lvalue references to them, in order.
#define DEFINE_AS_TUPLE(N, ...)                                                                    \
    template <typename T>                                                                          \
    inline auto as_tuple(T&& t, std::integral_constant<size_t, N>)                                 \
    {                                                                                              \
        auto& [__VA_ARGS__] = t;                                                                   \
        return std::forward_as_tuple(__VA_ARGS__);                                                 \
    }

    DEFINE_AS_TUPLE(1, x0)
    DEFINE_AS_TUPLE(2, x0, x1)
    DEFINE_AS_TUPLE(3, x0, x1, x2)
    DEFINE_AS_TUPLE(4, x0, x1, x2, x3)
    DEFINE_AS_TUPLE(5, x0, x1, x2, x3, x4)
    DEFINE_AS_TUPLE(6, x0, x1, x2, x3, x4, x5)
    DEFINE_AS_TUPLE(7, x0, x1, x2, x3, x4, x5, x6)
    DEFINE_AS_TUPLE(8, x0, x1, x2, x3, x4, x5, x6, x7)
    DEFINE_AS_TUPLE(9, x0, x1, x2, x3, x4, x5, x6, x7, x8)
    DEFINE_AS_TUPLE(10, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9)
    DEFINE_AS_TUPLE(11, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)
    DEFINE_AS_TUPLE(12, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)
    DEFINE_AS_TUPLE(13, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12)
    DEFINE_AS_TUPLE(14, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13)
    DEFINE_AS_TUPLE(15, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14)
    DEFINE_AS_TUPLE(16, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15)
    DEFINE_AS_TUPLE(17, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16)
    DEFINE_AS_TUPLE(18, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17)
    DEFINE_AS_TUPLE(19, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18)
    DEFINE_AS_TUPLE(20, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19)
    DEFINE_AS_TUPLE(21, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20)
    DEFINE_AS_TUPLE(22, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21)
    DEFINE_AS_TUPLE(23, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22)
    DEFINE_AS_TUPLE(24, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23)
    DEFINE_AS_TUPLE(25, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24)
    DEFINE_AS_TUPLE(26, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25)
    DEFINE_AS_TUPLE(27, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26)
    DEFINE_AS_TUPLE(28, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27)
    DEFINE_AS_TUPLE(29, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28)
    DEFINE_AS_TUPLE(30, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29)
    DEFINE_AS_TUPLE(31, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30)
    DEFINE_AS_TUPLE(32, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31)
    DEFINE_AS_TUPLE(33, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32)
    DEFINE_AS_TUPLE(34, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33)
    DEFINE_AS_TUPLE(35, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34)
    DEFINE_AS_TUPLE(36, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35)
    DEFINE_AS_TUPLE(37, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36)
    DEFINE_AS_TUPLE(38, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37)
    DEFINE_AS_TUPLE(39, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38)
    DEFINE_AS_TUPLE(40, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39)
    DEFINE_AS_TUPLE(41, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40)
    DEFINE_AS_TUPLE(42, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41)
    DEFINE_AS_TUPLE(43, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42)
    DEFINE_AS_TUPLE(44, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43)
    DEFINE_AS_TUPLE(45, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44)
    DEFINE_AS_TUPLE(46, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45)
    DEFINE_AS_TUPLE(47, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46)
    DEFINE_AS_TUPLE(48, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47)
    DEFINE_AS_TUPLE(49, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48)
    DEFINE_AS_TUPLE(50, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49)
    DEFINE_AS_TUPLE(51, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50)
    DEFINE_AS_TUPLE(52, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51)
    DEFINE_AS_TUPLE(53, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52)
    DEFINE_AS_TUPLE(54, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53)
    DEFINE_AS_TUPLE(55, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54)
    DEFINE_AS_TUPLE(56, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55)
    DEFINE_AS_TUPLE(57, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56)
    DEFINE_AS_TUPLE(58, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57)
    DEFINE_AS_TUPLE(59, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58)
    DEFINE_AS_TUPLE(60, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59)
    DEFINE_AS_TUPLE(61, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60)
    DEFINE_AS_TUPLE(62, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61)
    DEFINE_AS_TUPLE(63, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62)
    DEFINE_AS_TUPLE(64, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, x63)
    DEFINE_AS_TUPLE(106, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
                    x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32,
                    x33, x34, x35, x36, x37, x38, x39, x40, x41, x42, x43, x44, x45, x46, x47, x48,
                    x49, x50, x51, x52, x53, x54, x55, x56, x57, x58, x59, x60, x61, x62, x63, x64,
                    x65, x66, x67, x68, x69, x70, x71, x72, x73, x74, x75, x76, x77, x78, x79, x80,
                    x81, x82, x83, x84, x85, x86, x87, x88, x89, x90, x91, x92, x93, x94, x95, x96,
                    x97, x98, x99, x100, x101, x102, x103, x104, x105)

#undef DEFINE_AS_TUPLE
} // namespace ez::detail

#endif
