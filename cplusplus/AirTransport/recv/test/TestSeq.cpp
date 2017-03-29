#include <iostream>
#include <tuple>
#include <vector>

template <class ForwardIt>
auto GetFirstCombination(ForwardIt first, ForwardIt last) {
  std::vector<std::tuple<ForwardIt, ForwardIt, ForwardIt> > it_pair; // begin, curr, end
  auto Equal = [](auto x, auto y){
        return static_cast<uint64_t>(x)>>4 == static_cast<uint64_t>(y)>>4;
      };

  while (first != last) {
    auto curr = first;
    auto next = std::next(curr);
    while (next!=last && Equal(*curr, *next)) {
      ++curr;
      ++next;
    }
    
    it_pair.emplace_back(first, first, curr);
    first = next;
  }

  return it_pair;
}

template <class ForwardIt>
bool NextCombination(ForwardIt first, ForwardIt last) {
  for (; first!=last; ++first) {
    if (std::get<0>(*first) == std::get<2>(*first)){
      ;
    } else if (std::get<1>(*first) == std::get<2>(*first)) {
      std::get<1>(*first) = std::get<0>(*first);
    } else {
      ++std::get<1>(*first);
      break;
    }
  }
  
  return first != last;
}

int main(int argc, char **argv)
{
  std::vector<uint32_t> input = {0x01, 0x03, 0x07, 0x23, 0x25, 0x70, 0x80};
  auto result = GetFirstCombination(input.begin(), input.end());

  std::cout << std::hex;
  do {
    for (const auto &ele : result)
      std::cout << *std::get<1>(ele) << " ";
    std::cout << std::endl;
  } while (NextCombination(result.begin(), result.end()));

  return 0;
}


