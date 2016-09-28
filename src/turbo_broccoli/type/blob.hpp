/*
 * blob.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_BLOB_HPP_
#define SRC_TURBO_BROCCOLI_BLOB_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <turbo_broccoli/type/tags.hpp>

namespace turbo_broccoli { namespace types {


struct blob {

  blob(): version_(0) {

  }

  blob(const std::string& k, const nlohmann::json& d, const tag_list& ts) : key_(k), data_(d.dump()), tag_list_(ts), version_(0) {
  }

  blob(const std::string& k) : key_(k), data_(""), tag_list_(), version_(0) {
  }


  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  void put( const T& t)  {
    std::string tmp = detail::serialize(t);
    if(data_.compare(tmp) == 0 ) {
       return;
    }
    data_ = tmp;
    version_++;
  }

  void add_tag(const std::string& tag) {
    for(auto& t : tag_list_.tags) {
      if(t.compare(tag) == 0) {
        return;
      }
    }
    tag_list_.tags.push_back(tag);
  }

  void remove_tag(const std::string& tag) {
    tag_list_.tags.erase(std::remove(tag_list_.tags.begin(), tag_list_.tags.end(), tag), tag_list_.tags.end());
  }

  std::string key() const {
    return key_;
  }

  inline db_key key_hash() const {
    return detail::calculate_key(key_);
  }

  inline tag_list tags() const {
    return tag_list_;
  }

  inline ::size_t version() const {
    return version_;
  }

  inline std::string data() const {
    return data_;
  }

  inline void data(const std::string& d)  {
    data_ = d;
  }


  std::string                   data_;
  tag_list                      tag_list_;
  std::string                   parent_1;
  boost::optional<std::string>  parent_2;
};

} // type
} //trubo_broccoli

BOOST_FUSION_ADAPT_STRUCT(turbo_broccoli::types::blob, data_, tag_list_, parent_1, parent_2);


#endif /* SRC_TURBO_BROCCOLI_BLOB_HPP_ */
