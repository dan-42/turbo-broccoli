/*
 * database.hpp
 *
 *  Created on: Sep 26, 2016
 *      Author: frieddan
 */

#ifndef SRC_DATABASE_HPP_
#define SRC_DATABASE_HPP_

#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <functional>
#include <thread>
#include <cassert>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>
#include <pre/json/to_json.hpp>
#include <turbo_broccoli/database.hpp>

namespace database { namespace tag {

template<typename T> std::vector<std::string> inline get_tags() { return {}; }

}}


namespace database {

  using blob = ::turbo_broccoli::blob;

  struct database {

    database(const std::string& path) : db_(path) {

    }

    template<typename T, ::turbo_broccoli::detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    inline void store(const std::string& key, const T& t) {
      ::turbo_broccoli::blob b(key, pre::json::to_json(t), { tag::get_tags<T>() });
      db_.store(b);
    }


    template<typename T, ::turbo_broccoli::detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    inline std::vector<T> find(const std::string& key) {
      std::vector<T> result{};

      auto r = db_.find(key);
      if(r.success) {
        for(auto& b : r.results) {
          result.push_back(::turbo_broccoli::detail::deserialize<T>(b.data()) );
        }
      }
      return result;
    }

  private:
    ::turbo_broccoli::database db_;

  };


}


#endif /* SRC_DATABASE_HPP_ */
