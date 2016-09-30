/*
 * blob.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_BLOB_STORAGE_HPP_
#define SRC_TURBO_BROCCOLI_BLOB_STORAGE_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <turbo_broccoli/type/tags.hpp>

namespace turbo_broccoli { namespace types {


struct blob_storage {


  blob_storage(const std::string& d, const std::vector<std::string>& ts, const std::vector<std::string>& parents)
              : data_(d),
                tag_list_(ts),
                parents_(parents) {
  }

  blob_storage() : data_(""), parents_({""}) {
  }


  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  void put( const T& t)  {
    std::string tmp = detail::serialize(t);
    if(data_.compare(tmp) == 0 ) {
       return;
    }
    data_ = tmp;
  }

  void add_tag(const std::string& tag) {
    for(auto& t : tag_list_) {
      if(t.compare(tag) == 0) {
        return;
      }
    }
    tag_list_.push_back(tag);
  }

  void remove_tag(const std::string& tag) {
    tag_list_.erase(std::remove(tag_list_.begin(), tag_list_.end(), tag), tag_list_.end());
  }


  inline std::vector<std::string> tags() const {
    return tag_list_;
  }


  inline std::string data() const {
    return data_;
  }

  inline void data(const std::string& d)  {
    data_ = d;
  }

  inline std::vector<std::string> parents() const {
    return parents_;
  }

  inline void parents(const std::vector<std::string>& ps) {
    parents_ = ps;
  }


  std::string                   data_;
  std::vector<std::string>      tag_list_;
  std::vector<std::string>      parents_;
};

} // type
} //trubo_broccoli

BOOST_FUSION_ADAPT_STRUCT(turbo_broccoli::types::blob_storage, data_, tag_list_, parents_);


#endif /* SRC_TURBO_BROCCOLI_BLOB_STORAGE_HPP_ */
