/*
 * tags.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_TAGS_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_TAGS_HPP_

#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>
#include <turbo_broccoli/type/tag_list.hpp>

namespace turbo_broccoli { namespace types {



struct tag_list {
  tag_list_type tags;
};

}}

BOOST_FUSION_ADAPT_STRUCT(turbo_broccoli::types::tag_list, tags);


template<typename OStream>
inline OStream& operator<<(OStream& os, const turbo_broccoli::types::tag_list& tl) {
  for(auto& t : tl.tags)
  os << t << ':';
  return os;
}

#endif /* SRC_TURBO_BROCCOLI_TYPE_TAGS_HPP_ */
