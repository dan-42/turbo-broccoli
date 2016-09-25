/*
 * result_key.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_RESULT_KEY_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_RESULT_KEY_HPP_

#include <tuple>
#include <turbo_broccoli/type/key.hpp>

namespace turbo_broccoli { namespace types {

using result_key   = std::pair<bool, db_key>;

} //type
} //trubo_broccoli

#endif /* SRC_TURBO_BROCCOLI_TYPE_RESULT_KEY_HPP_ */
