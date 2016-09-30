/*
 * blob.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: frieddan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_BLOBIMPL_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_BLOBIMPL_HPP_

#include <turbo_broccoli/type/blob_storage.hpp>

namespace turbo_broccoli {

template<typename Database>
struct blob_impl {

  blob_impl(Database& db, const std::string& key) : db_(db), key_(key) {

  }

  /**
   * store all data in db, if the version is a successor of the last one
   * and return true
   * if there is a different change on the data, the false is returned and no data is stored
   */
  inline bool store() {
    auto success = db_.store_blob(key_, data_, tag_list_, version_, parents_);
    if(success.first) {
      parents_.clear();
      parents_.push_back(version_);
      version_ = success.second;
      return true;
    }
    //xxx handle conflict

    return false;
  }

  /**
   * load data from data, overwrites all data in object
   * return false if no object is found or could not be loaded
   */
  inline bool load() {
    auto success = db_.load_blob(key_, data_, tag_list_, version_, parents_);
    if(success) {
      return true;
    }
    // xxx hanlde issues
    return false;
  }


  /**
   * stores data in blob, no db access
   */
  inline bool set(const std::string& data) {
    try {
      data_ = data;
      return true;
    }
    catch(...) {
      return false;
    }
  }

  /**
   * return current data from blob, no db access
   */
  inline std::string get() const {
    return data_;
  }


  inline bool add_tag(const std::string& tag) {
    try {
      for(auto& t : tag_list_) {
        if(t.compare(tag) == 0) {
          return true;
        }
      }
      tag_list_.push_back(tag);
      return true;
    }
    catch(...) {
      return false;
    }
  }

  template<typename List>
  inline bool add_tags(const List& tags) {
    try {
      for(auto& new_tag : tags) {
        if(!add_tag(new_tag)) {
          return false;
        }
      }
      return true;
    }
    catch(...) {
      return false;
    }
  }

  inline bool remove_tag(const std::string& tag) {
    try {
      tag_list_.erase(std::remove(tag_list_.begin(), tag_list_.end(), tag), tag_list_.end());
      return true;
    }
    catch(...) {
      return false;
    }
  }

  inline std::vector<std::string> tags() {
    return tag_list_;
  }

  inline std::string version() const {
    return version_;
  }

  inline std::vector<std::string> parents() {
    return parents_;
  }

private:

  Database& db_;
  std::string                   key_;
  std::string                   data_;
  std::vector<std::string>      tag_list_;
  std::vector<std::string>      parents_;
  std::string                   version_;

};

}

#endif /* SRC_TURBO_BROCCOLI_TYPE_BLOBIMPL_HPP_ */
