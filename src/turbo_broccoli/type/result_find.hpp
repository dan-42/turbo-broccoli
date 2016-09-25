/*
 * result_find.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_RESULT_FIND_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_RESULT_FIND_HPP_

#include <list>
#include <turbo_broccoli/type/blob.hpp>

namespace turbo_broccoli { namespace types {

struct result_find {

  bool success{false};
  std::list<blob> results{};

};


} //type
} //turbo_broccloi

#endif /* SRC_TURBO_BROCCOLI_TYPE_RESULT_FIND_HPP_ */
