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
      auto b = db_.blob(key);
      b.add_tags(tag::get_tags<T>());
      b.set(pre::json::to_json(t).dump(2));
      b.store();
    }


    template<typename T, ::turbo_broccoli::detail::enable_if_is_adapted_struct_t<T>* = nullptr>
    inline std::vector<T> find(const std::string& key) {
      std::vector<T> result{};

      auto blob = db_.blob(key);
      if(blob.load() ) {
        T tmp = ::turbo_broccoli::detail::deserialize<T>(blob.get());
        result.push_back(tmp);
        return result;
      }

      auto tag = db_.tag(key);
      if(tag.load()) {
        for(auto& entry : tag.entries()) {
          auto tmp_blob = db_.blob(entry);
          if(tmp_blob.load()) {
            T tmp = ::turbo_broccoli::detail::deserialize<T>(tmp_blob.get());
            result.push_back(tmp);
          }
          else {
            std::cout << "find() inconsistent data" << std::endl;
          }
        }
      }
      return result;
    }

  private:
    ::turbo_broccoli::database db_;

  };


}


#endif /* SRC_DATABASE_HPP_ */
