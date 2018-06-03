#pragma once

template<class KeyType, class ValueType>
class DenseOrderedMap
{
public:
  using KVPair = std::pair<KeyType, ValueType>;
  using storage_type = std::vector<KVPair>;
  using key_type = KeyType;
  using value_type = ValueType;
  
private:
  storage_type const m_vec;

public:
  DenseOrderedMap(std::initializer_list<KVPair> init)
    : DenseOrderedMap(init.begin(), init.end())
  {
  }

  explicit DenseOrderedMap(storage_type vec)
    : m_vec(SortedVec(vec))
  {
  }

  template<class InputIt>
  DenseOrderedMap(InputIt first, InputIt last)
    : m_vec(SortedVec(first, last))
  {
  }

  auto find(KeyType key) const
  {
    auto Comp = [](KVPair p, KeyType k) { return p.first < k; };
    auto const end = cend();
    
    auto it = std::lower_bound(cbegin(), end, key, Comp);
    if (it == end || key < it->first)
      {
        return end;
      }

    return it;
  }

  auto cbegin() const { return m_vec.cbegin(); }

  auto cend() const { return m_vec.cend(); }
  
private:

  static void SortStorage(storage_type& storage)
  {
    auto Less = [](const KVPair& p1, const KVPair p2) { return p1.first < p2.first; };
    std::sort(storage.begin(), storage.end(), Less);
  }

  static storage_type SortedVec(storage_type vec)
  {
    SortStorage(vec);
    return vec;
  }

  template<class InputIt>
  static storage_type SortedVec(InputIt first, InputIt last)
  {
    storage_type storage(first, last);
    return SortedVec(storage);
  }
};
