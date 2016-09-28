/*
 * key.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_KEY_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_KEY_HPP_

#include <array>
#include <cstring>
#include <pre/bytes/utils.hpp>


namespace turbo_broccoli { namespace types {

  using hash_t        = std::array<uint8_t, 20>;

  inline bool operator==( const hash_t& lhs,  const hash_t& rhs ) {
    return  ( 0 == std::memcmp(lhs.data(), rhs.data(), 20) );
  }

  inline bool operator!=( const hash_t& lhs,  const hash_t& rhs ) {
    return  !( 0 == std::memcmp(lhs.data(), rhs.data(), 20) );
  }

  inline std::string to_string(const hash_t& k) {
    std::string s = pre::bytes::to_hexstring(k.data(), k.size());
    return s;
  }

  inline hash_t string_to_key(const std::string& ks) {
    hash_t k{};
    auto t = pre::bytes::from_hexstring(ks);
    if(t.size() == k.size() ) {
      for(::size_t i = 0; i < k.size(); ++i) {
        k[i] = t[i];
      }
    }
    return k;
  }

  inline hash_t nil_key() {
    static const hash_t nil{0};
    return nil;
  }



} //type
} //trubo_broccoli


template<typename OStream>
inline OStream& operator<<(OStream& os, const turbo_broccoli::types::hash_t& key) {
  os << turbo_broccoli::types::to_string(key);
  return os;
}

#endif /* SRC_TURBO_BROCCOLI_TYPE_KEY_HPP_ */
