//
//  spans.cpp
//  CF.STL_Containers_Span
//
//  Created by Alan Sampson on 3/22/21.
//
//  MARK: - Reference.
//  @see: https://en.cppreference.com/w/cpp/container
//  @see: https://en.cppreference.com/w/cpp/container/span
//

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <compare>
#include <memory>
#include <type_traits>
#include <span>
#include <array>
#include <vector>
#include <cassert>
#include <cstddef>
#if (__cplusplus > 201707L)
#include <ranges>
#endif  /* (__cplusplus > 201707L) */

using namespace std::literals::string_literals;

//  MARK: - Definitions

//  MARK: - Local Constants.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  MARK: namespace konst
namespace konst {

auto delimiter(char const dc = '-', size_t sl = 80) -> std::string const {
  auto const dlm = std::string(sl, dc);
  return dlm;
}

static
auto const dlm = delimiter();

static
auto const dot = delimiter('.');

} /* namespace konst */

#if (__cplusplus > 201707L)
#endif  /* (__cplusplus > 201707L) */

//  MARK: - Function Prototype.
auto C_span(int argc, const char * argv[]) -> decltype(argc);

//  MARK: - Implementation.
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  std::cout << "CF.STL_Containers_Span\n";
  std::cout << "C++ Version: "s << __cplusplus << std::endl;

  std::cout << '\n' << konst::dlm << std::endl;
  C_span(argc, argv);

  return 0;
}

//  MARK: - C_span
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  MARK: namespace cspan
namespace cspan {

template<class T, std::size_t N>
[[nodiscard]]
constexpr auto slide(std::span<T, N> span, std::size_t offset, std::size_t width) {
  return span.subspan(offset, offset + width <= span.size() ? width : 0U);
}

template<class T, std::size_t N, std::size_t M>
[[nodiscard]]
constexpr bool starts_with(std::span<T, N> data, std::span<T, M> prefix) {
  return data.size() >= prefix.size()
  && std::equal(prefix.begin(), prefix.end(), data.begin());
}

template<class T, std::size_t N, std::size_t M>
[[nodiscard]]
constexpr bool ends_with(std::span<T, N> data, std::span<T, M> suffix) {
  return data.size() >= suffix.size()
    && std::equal(data.end() - suffix.size(), data.end(),
                  suffix.end() - suffix.size());
}

template<class T, std::size_t N, std::size_t M>
[[nodiscard]]
constexpr bool contains(std::span<T, N> span, std::span<T, M> sub) {
  return std::search(span.begin(), span.end(), sub.begin(),
                     sub.end()) != span.end();
}

} /* namespace cspan */

//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: C_span()
 */
auto C_span(int argc, const char * argv[]) -> decltype(argc) {
  std::cout << "In "s << __func__ << std::endl;

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span"s << '\n';
  {
    auto print = [](auto const & seq) {
      for (auto const & elem : seq) {
        std::cout << elem << ' ';
      }
      std::cout << '\n';
    };

    int constexpr ary_a[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, };
    int constexpr ary_b[] { 8, 7, 6, };

    for (std::size_t offset{}; ; ++offset) {
      std::size_t constexpr width{6};
      auto sspan = cspan::slide(std::span { ary_a }, offset, width);
        if (sspan.empty()) {
        break;
      }
      print(sspan);
    }

    [[maybe_unused]]
    auto t1 = cspan::starts_with(std::span { ary_a }, std::span {ary_a, 4, });
    [[maybe_unused]]
    auto t2 = cspan::starts_with(std::span { ary_a + 1, 4}, std::span{ ary_a + 1, 3 });
    [[maybe_unused]]
    auto t3 = cspan::starts_with(std::span { ary_a }, std::span { ary_b });
    [[maybe_unused]]
    auto t4 = cspan::starts_with(std::span { ary_a, 8 }, std::span { ary_a + 1, 3 });
    [[maybe_unused]]
    auto t5 = cspan::ends_with(std::span { ary_a }, std::span { ary_a + 6, 3 });
    [[maybe_unused]]
    auto t6 = cspan::ends_with(std::span { ary_a }, std::span { ary_a + 6, 2 });
    [[maybe_unused]]
    auto t7 = cspan::contains(std::span { ary_a }, std::span { ary_a + 1, 4 });
    [[maybe_unused]]
    auto t8 = cspan::contains(std::span { ary_a, 8 }, std::span { ary_a, 9 });

    std::cout << std::boolalpha;
    std::cout << "test 1: "s << t1 << '\n'
              << "test 2: "s << t2 << '\n'
              << "test 3: "s << t3 << '\n'
              << "test 4: "s << t4 << '\n'
              << "test 5: "s << t5 << '\n'
              << "test 6: "s << t6 << '\n'
              << "test 7: "s << t7 << '\n'
              << "test 8: "s << t8 << '\n';
    std::cout << std::noboolalpha;

    static_assert(cspan::starts_with(std::span { ary_a }, std::span {ary_a, 4, })
        && cspan::starts_with(std::span { ary_a + 1, 4}, std::span{ ary_a + 1, 3 })
        && !cspan::starts_with(std::span { ary_a }, std::span { ary_b })
        && !cspan::starts_with(std::span { ary_a, 8 }, std::span { ary_a + 1, 3 })
        && cspan::ends_with(std::span { ary_a }, std::span { ary_a + 6, 3 })
        && !cspan::ends_with(std::span { ary_a }, std::span { ary_a + 6, 2 })
        && cspan::contains(std::span { ary_a }, std::span { ary_a + 1, 4 })
        && !cspan::contains(std::span { ary_a, 8 }, std::span { ary_a, 9 }));

    auto aspan = std::span { ary_a };
    std::cout << "span size: " << aspan.size() << '\n'
              << "span extent: " << aspan.extent << '\n';

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, operator="s << '\n';
  {
    auto print = [](std::string_view info = ""s,
                    std::span<int const> span = {},
                    std::size_t extent = 0,
                    std::size_t size_of = 0) {
      if (span.size() == 0) {
        std::cout << info << '\n';
        return;
      }
      std::cout << info << '[' << span.size() << "] {"s;
#if (__cplusplus > 201707L)
      std::ranges::for_each(span, [](int const x_) { std::cout << ' ' << x_; });
#else
      std::for_each(span.cbegin(), span.cend(), [](int const x_) {
        std::cout << ' ' << x_;
      });
#endif  /* (__cplusplus > 201707L) */
      std::cout << " }"s;
      if (extent != 0) {
        std::cout << " extent = "s;
        if (extent == std::dynamic_extent) {
          std::cout << "dynamic"s;
        }
        else {
          std::cout << extent;
        }
      }

      if (size_of != 0) {
        std::cout << ", sizeof = "s << size_of;
      }
      std::cout << '\n';
    };

    std::array<int, 6> a1;
    std::array<int, 6> a2;
    a1.fill(3);
    a2.fill(4);

    auto s1 = std::span(a1);
    auto s2 = std::span(a2);
    print("s1"s, s1, s1.extent, sizeof(s1));
    print("s2"s, s2, s2.extent, sizeof(s2));

    // Check that assignment performs a shallow copy.
    s1 = s2;
    (s1.data() == s2.data() && s1.size() == s2.size())
      ? print("s1 = s2; is a shallow copy!"s)
      : print("s1 = s2; is a deep copy!"s);
    print("s1"s, s1);

    print("Fill s1 with 5:"s);
#if (__cplusplus > 201707L)
    std::ranges::fill(s1, 5);
    // s2 is also 'updated' since s1 and s2 point to the same data
    assert(std::ranges::equal(s1, s2));
#else
    std::fill(s1.begin(), s1.end(), 5);
    assert(std::equal(s1.cbegin(), s1.cend(), s2.cbegin()));
#endif  /* (__cplusplus > 201707L) */
    print("s1"s, s1);
    print("s2"s, s2);
    print();

    int a3[] {1, 2, 3, 4};
    int a4[] {2, 3, 4, 5};
    int a5[] {3, 4, 5};

    std::span<int, std::dynamic_extent> dynamic_1 {a3};
    std::span<int, std::dynamic_extent> dynamic_2 {a4, 3};
    std::span<int, 4> static_1 {a3};
    std::span<int, 4> static_2 {a4};
    std::span<int, 3> static_3 {a5};

    print("dynamic_1"s, dynamic_1, dynamic_1.extent, sizeof(dynamic_1));
    print("dynamic_2"s, dynamic_2, dynamic_2.extent, sizeof(dynamic_2));
    print("static_1"s, static_1, static_1.extent, sizeof(static_1));
    print("static_2"s, static_2, static_2.extent, sizeof(static_2));
    print("static_3"s, static_3, static_3.extent, sizeof(static_3));

    dynamic_1 = dynamic_2; // OK
    dynamic_1 = static_1;  // OK
//    static_1  = dynamic_1; // ERROR: no match for ‘operator=’
    static_1  = static_2;  // OK: same extents = 4
//    static_1  = static_3;  // ERROR: different extents: 4 and 3

    std::cout << '\n';
  }

  /// Iterators
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, begin, end"s << '\n';
  {
    auto print = [](std::span<const int> sp) {
      for(auto it = sp.begin(); it != sp.end(); ++it) {
        std::cout << *it << ' ';
      }
      std::cout << '\n';
    };
     
    auto transmogrify = [](std::span<int> sp) {
      if (!sp.empty()) {
        std::cout << *sp.begin() << '\n';
        *sp.begin() = 2;
      }
    };

    int array[] { 1, 3, 4, 5, };
    print(array);
    transmogrify(array);
    print(array);

    std::cout << '\n';
  }

  /// Iterators
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, rbegin"s << '\n';
  {
    std::span<char const> constexpr code{ "@droNE_T0P_w$s@s#_SECRET_a,p^42!" };

    auto hacker = [](unsigned const Ou) {
      return Ou - 0141 < 120;
    };

    std::copy_if(code.rbegin(), code.rend(),
    std::ostream_iterator<char const>(std::cout), hacker);

    std::cout << '\n';
  }

  /// Iterators
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, rend"s << '\n';
  {
    auto ascending = [](std::span<std::string_view const> const data,
                        std::string_view const term) {
        std::for_each(data.begin(), data.end(),
                      [](const std::string_view x_) {
          std::cout << x_ << " "s;
        });
        std::cout << term;
    };

    auto descending = [](const std::span<const std::string_view> data,
                         const std::string_view term) {
        std::for_each(data.rbegin(), data.rend(),
                      [](const std::string_view x_) {
          std::cout << x_ << " ";
        });
      std::cout << term;
    };

#define UTF16_
#if (defined(UTF8_))
    [[maybe_unused]]
    std::string_view constexpr bars[] = {
      "\xE2\x96\x81", "\xE2\x96\x82", "\xE2\x96\x83", "\xE2\x96\x84",
      "\xE2\x96\x85", "\xE2\x96\x86", "\xE2\x96\x87", "\xE2\x96\x88",
    };
#elif (defined(UTF16_))
    [[maybe_unused]]
    std::string_view constexpr bars[] = {
      "\u2581", "\u2582", "\u2583", "\u2584", "\u2585", "\u2586", "\u2587", "\u2588",
    };
#else   /* (!defined(UTF8) && !defined(UTF16)) */
    [[maybe_unused]]
    std::string_view constexpr bars[] = {
      "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█",
    };
#endif  /* (!defined(UTF8) && !defined(UTF16)) */
      
    ascending(bars, " "s);
    descending(bars, "\n"s);

    std::cout << '\n';
  }

  ///  Element access
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, front"s << '\n';
  {
    auto print = [](std::span<int const> const data) {
      for (auto offset{0U}; offset != data.size(); ++offset) {
        std::cout << data.subspan(offset).front() << ' ';
      }
      std::cout << '\n';
    };

    int constexpr data[] { 0, 1, 2, 3, 4, 5, 6, };
    print({data, 4});

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, back"s << '\n';
  {
    auto print_forward = [](std::span<int const> const span) {
      for (auto nr { span.size() }; nr != 0; --nr ) {
        std::cout << span.last(nr).front() << ' ';
      }
      std::cout << '\n';
    };

    auto print_backward = [](std::span<int const> const span) {
      for (auto nr { span.size() }; nr != 0; --nr) {
        std::cout << span.first(nr).back() << ' ';
      }
      std::cout << '\n';
    };

    int constexpr numbers[] { 0, 1, 2, 3, 4, };
    print_forward(numbers);
    print_backward(numbers);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, operator[]"s << '\n';
  {
    auto reverse = [](std::span<int> span) {
      for (std::size_t ix = 0, jx = std::size(span); ix < jx; ++ix) {
        --jx;
        std::swap(span[ix], span[jx]);
      }
    };

    auto print = [](std::span<int const> const span) {
    for (int element : span) {
      std::cout << element << ' ';
    }
    std::cout << '\n';
    };

    int data[] { 1, 2, 3, 4, 5, };
    print(data);
    reverse(data);
    print(data);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, data"s << '\n';
  {
    char constexpr str[] = "ABCDEF\n";

    const std::span sp { str };

    for (auto nr{sp.size()}; nr != 2; --nr) {
      std::cout << sp.last(nr).data();
    }

    std::cout << '\n';
  }

  /// Observers
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, size"s << '\n';
  {
    auto show_sizes = [](std::span<const int> span) {
      std::cout
        << span                 .size() << ' ' // 8
        << span.first(7)        .size() << ' ' // 7
        << span.first<6>()      .size() << ' ' // 6
        << span.last(5)         .size() << ' ' // 5
        << span.last<4>()       .size() << ' ' // 4
        << span.subspan(2, 3)   .size() << ' ' // 3
        << span.subspan<3, 2>() .size() << ' ' // 2
        << '\n';
    };

    int constexpr antique_array[] { 1, 2, 3, 4, 5, 6, 7, 8 };
    show_sizes(antique_array);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, size_bytes"s << '\n';
  {
    static std::int32_t constexpr ary[] { 1, 2, 3, 4, 5, };
    std::span constexpr spn { ary };

    std::cout << "sizeof(int32_t)  : "s << sizeof(int32_t)  << '\n'
              << "std::size(ary)   : "s << std::size(ary)   << '\n'
              << "sizeof(ary)      : "s << sizeof(ary)      << '\n'
              << "spn.size()       : "s << spn.size()       << '\n'
              << "spn.size_bytes() : "s << spn.size_bytes() << '\n';

    static_assert( sizeof(int32_t)  == 4  );
    static_assert( std::size(ary)   == 5  );
    static_assert( sizeof(ary)      == 20 );
    static_assert( spn.size()       == 5  );
    static_assert( spn.size_bytes() == 20 );

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, empty"s << '\n';
  {
    std::span<char const> span{ "ABCDEF" };

    while (!span.empty()) {
      std::cout << std::quoted(span.data()) << '\n';
      span = span.last(span.size() - 1);
    }

    std::cout << '\n';
  }

  /// Subviews
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, first"s << '\n';
  {
    auto print = [](std::string_view const title,
               /* std::ranges::forward_range */
                    auto const & container) {
      std::cout << title << "["s << std::size(container) << "]{ "s;
      for (auto const & elem : container) {
        std::cout << elem << ", "s;
      }
      std::cout << "};\n"s;
    };

    auto run_game = [&](std::span<int const> span)
    {
      print("span: "s, span);

      std::span<int const, 5> span_first = span.first<5>();
      print("span.first<5>(): "s, span_first);

      std::span<const int, std::dynamic_extent> span_first_dynamic = span.first(4);
      print("span.first(4):   "s, span_first_dynamic);
    };

    int constexpr ary[8] { 1, 2, 3, 4, 5, 6, 7, 8, };
    print("int  ary", ary);
    run_game(ary);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, last"s << '\n';
  {
    auto print = [](std::string_view const title, auto const & container) {
    std::cout << title << "["s << std::size(container) << "]{ "s;
    for (auto const & elem : container)  {
      std::cout << elem << ", "s;
    }
    std::cout << "};\n"s;
    };
     
    auto run = [&print](std::span<int const> span)
    {
        print("span: "s, span);
     
        std::span<int const, 3> span_last = span.last<3>();
        print("span.last<3>(): "s, span_last);
     
        std::span<int const, std::dynamic_extent> span_last_dynamic = span.last(2);
        print("span.last(2):   "s, span_last_dynamic);
    };

    int constexpr ary[8] { 1, 2, 3, 4, 5, 6, 7, 8, };
    print("int  ary"s, ary);
    run(ary);

    std::cout << '\n';
  }

  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::span, subspan"s << '\n';
  {
    auto display = [](std::span<char const> abc) {
      auto const columns{ 20U };
      auto const rows{ abc.size() - columns + 1 };

      for (auto offset{ 0U }; offset < rows; ++offset) {
#if (__cplusplus > 201707L)
        std::ranges::for_each(
          abc.subspan(offset, columns),
          std::putchar
        );
#else
        std::for_each(abc.subspan(offset, columns).cbegin(),
                      abc.subspan(offset, columns).cend(),
                      std::putchar);
#endif  /* (__cplusplus > 201707L) */
        std::putchar('\n');
      }
    };

    char abc[26];
    std::iota(std::begin(abc), std::end(abc), 'A');
    display(abc);

    std::cout << '\n';
  }

  /// Non-member functions
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::std::as_bytes, std::as_writable_bytes"s << '\n';
  {
    auto print = [](float const x_val, std::span<std::byte const> const bytes) {
      std::cout << std::setprecision(6) << std::setw(8) << x_val << " = { "s
                << std::hex << std::uppercase << std::setfill('0');
      for (auto const b_ : bytes) {
        std::cout << std::setw(2) << std::to_integer<int>(b_) << ' ';
      }
      std::cout << std::dec << "}\n"s;
    };

    /* mutable */ float data[1] { 3.141592f };

    auto const const_bytes = std::as_bytes(std::span{ data });

    print(data[0], const_bytes);

    auto const writable_bytes = std::as_writable_bytes(std::span{ data });

    // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
    writable_bytes[3] |= std::byte{ 0b1000'0000 };

    print(data[0], const_bytes);

    std::cout << '\n';
  }

  /// Non-member constant
  // ....+....!....+....!....+....!....+....!....+....!....+....!
  std::cout << konst::dot << '\n';
  std::cout << "std::std::dynamic_extent"s << '\n';
  {
    auto print = [](std::string_view const name, std::size_t ex) {
      std::cout << name << ", "s;
      if (ex == std::dynamic_extent) {
        std::cout << "dynamic extent\n"s;
      }
      else {
        std::cout << "static extent = "s << ex << '\n';
      }
    };

    int ary1[] { 1, 2, 3, 4, 5, };

    std::span span1{ary1};
    print("span1"s, span1.extent);

    std::span<int, std::dynamic_extent> span2{ary1};
    print("span2"s, span2.extent);

    std::array ary2 { 1, 2, 3, 4, 5, };
    std::span span3{ary2};
    print("span3"s, span3.extent);

    std::vector vec1 { 1, 2, 3, 4, 5, };
    std::span span4{vec1};
    print("span4"s, span4.extent);

    std::cout << '\n';
  };

  std::cout << std::endl; //  make sure cout is flushed.

  return 0;
}
