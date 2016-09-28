/*
 * value.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_VALUE_HPP_
#define SRC_TURBO_BROCCOLI_VALUE_HPP_

#include <vector>
#include <string>

#include <boost/fusion/include/adapt_struct.hpp>
#include <turbo_broccoli/detail/utils.hpp>

namespace turbo_broccoli { namespace types {

namespace value_type {
  static const std::string tag_list {"tag_list"};
  static const std::string blob     {"blob"};
  static const std::string reference{"reference"};
}



struct value_t {
  std::string reccord_type;
  std::string data;
};


inline bool is_blob(const value_t& v) {
  return (v.reccord_type == value_type::blob) ;
}

inline bool is_tag_list(const value_t& v) {
  return (v.reccord_type == value_type::tag_list) ;
}

inline bool is_reference(const value_t& v) {
  return (v.reccord_type == value_type::reference) ;
}

} //type
} //trubo_broccoli

BOOST_FUSION_ADAPT_STRUCT(turbo_broccoli::types::value_t, reccord_type, data);

#endif /* SRC_TURBO_BROCCOLI_VALUE_HPP_ */
