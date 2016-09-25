/*
 * tags.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_TAGGED_RECORDS_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_TAGGED_RECORDS_HPP_

#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>

namespace turbo_broccoli { namespace types {

struct tagged_records {
  std::vector<std::string> keys;
};

}}

BOOST_FUSION_ADAPT_STRUCT(turbo_broccoli::types::tagged_records, keys);


template<typename OStream>
inline OStream& operator<<(OStream& os, const turbo_broccoli::types::tagged_records& tr) {
  for(auto& t : tr.keys)
  os << t << ':';
  return os;
}

#endif /* SRC_TURBO_BROCCOLI_TYPE_TAGGED_RECORDS_HPP_ */
