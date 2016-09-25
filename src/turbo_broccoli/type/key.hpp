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

  using db_key        = std::array<uint8_t, 20>;

  inline bool operator==( const db_key& lhs,  const db_key& rhs ) {
    return  ( 0 == std::memcmp(lhs.data(), rhs.data(), 20) );
  }

  inline bool operator!=( const db_key& lhs,  const db_key& rhs ) {
    return  !( 0 == std::memcmp(lhs.data(), rhs.data(), 20) );
  }

  inline std::string to_string(const db_key& k) {
    std::string s = pre::bytes::to_hexstring(k.data(), k.size());
    return s;
  }

  inline db_key string_to_key(const std::string& ks) {
    db_key k{};
    auto t = pre::bytes::from_hexstring(ks);
    if(t.size() == k.size() ) {
      for(::size_t i = 0; i < k.size(); ++i) {
        k[i] = t[i];
      }
    }
    return k;
  }

  inline db_key nil_key() {
    static const db_key nil{0};
    return nil;
  }



} //type
} //trubo_broccoli


template<typename OStream>
inline OStream& operator<<(OStream& os, const turbo_broccoli::types::db_key& key) {
  os << turbo_broccoli::types::to_string(key);
  return os;
}

#endif /* SRC_TURBO_BROCCOLI_TYPE_KEY_HPP_ */
