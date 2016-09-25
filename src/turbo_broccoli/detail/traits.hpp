/*
 * traits.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DETAIL_TRAITS_HPP_
#define SRC_TURBO_BROCCOLI_DETAIL_TRAITS_HPP_

#include <type_traits>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/fusion/include/struct.hpp>

namespace turbo_broccoli { namespace detail {

  template<class T>
  using enable_if_is_adapted_struct_t = typename std::enable_if<
      std::is_same<
        typename boost::fusion::traits::tag_of<T>::type,
        boost::fusion::struct_tag
      >::value
  ,T>::type;

}
}

#endif /* SRC_TURBO_BROCCOLI_DETAIL_TRAITS_HPP_ */
