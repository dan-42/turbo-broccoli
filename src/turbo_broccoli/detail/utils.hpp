/*
 * utils.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_
#define SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_

#include <cstdint>
#include <string>
#include <array>

#include <nlohmann/json.hpp>

#include <boost/uuid/sha1.hpp>
#include <pre/bytes/utils.hpp>
#include <pre/json/to_json.hpp>
#include <pre/json/from_json.hpp>

#include <turbo_broccoli/type/key.hpp>

#include <turbo_broccoli/detail/traits.hpp>

namespace turbo_broccoli { namespace detail {



  inline turbo_broccoli::types::hash_t to_key(const unsigned int hash[5]) {
    auto h = reinterpret_cast<const uint8_t*>(hash);
    turbo_broccoli::types::hash_t k;
    for(int i = 0; i < 5; ++i)  {
      k[i*4]   = h[i*4+3];
      k[i*4+1] = h[i*4+2];
      k[i*4+2] = h[i*4+1];
      k[i*4+3] = h[i*4];
    }
    return k;
  }


  inline turbo_broccoli::types::hash_t hash_data(const std::string data) {
    try {
      boost::uuids::detail::sha1 hasher{};
      for(auto &c : data ) {
        hasher.process_byte(c);
      }

      unsigned int hash[5];
      hasher.get_digest(hash);
      return to_key(hash);
    }
    catch(...) {
      return turbo_broccoli::types::nil_key();
    }
  }


  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  inline std::string serialize(const T& b) {
    try {
      return pre::json::to_json(b).dump(2);
    }
    catch(...) {
      return {""};
    }
  }

  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  inline T deserialize(const std::string& data) {
    try {
      return pre::json::from_json<T>(nlohmann::json::parse(data));
    }
    catch(...) {
      std::cout << "detail::deserialize() error " << data << std::endl;
      throw std::runtime_error("detail::deserialize() error ");
    }
    return T{};
  }


  /*
   * \brief return list of all elements that are only in a
   * a{0, 1, 2, 3, 4}
   * b{3, 4, 5, 6, 7}
   * d{0, 1, 2}
   */
  inline std::vector<std::string> diff(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    std::vector<std::string> d;
    for(auto& a_i : a) {
       bool contains_b_i{false};
       for(auto& b_i : b) {
         if(a_i.compare(b_i) == 0) {
           contains_b_i = true;
           break;
         }
       }

       if(!contains_b_i) {
         d.push_back(a_i);
       }
    }
    return d;
  }

} //detail
} //turbo_broccoli



#endif /* SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_ */
