/*
 * tag_list.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: frieddan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_TAG_LIST_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_TAG_LIST_HPP_



#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>

namespace turbo_broccoli { namespace types {

typedef std::vector<std::string> tag_list_type;

}}


template<typename OStream>
inline OStream& operator<<(OStream& os, const turbo_broccoli::types::tag_list_type& tl) {
  for(auto& t : tl)
  os << t << ':';
  return os;
}


#endif /* SRC_TURBO_BROCCOLI_TYPE_TAG_LIST_HPP_ */
