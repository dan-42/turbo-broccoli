/*
 * tag.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: frieddan
 */

#ifndef SRC_TURBO_BROCCOLI_TYPE_TAGIMPL_HPP_
#define SRC_TURBO_BROCCOLI_TYPE_TAGIMPL_HPP_



namespace turbo_broccoli {

template<typename Database>
struct tag_impl {

  tag_impl(Database& db, const std::string& key) : db_(db) {

  }

  inline bool store() {
    return false;
  }

  inline bool load() {
    return false;
  }


  /**
   * stores data directly into the db
   */
  inline bool put(const std::string& data) {
    return false;
  }

  /**
   * loads data from db and returns it
   */
  inline std::string get() const {
    return {};
  }


  inline bool add_entrie(const std::string& tag) {
    return false;
  }

  template<typename List>
  inline bool add_entries(const List& tags) {

    return false;
  }

  inline bool remove_entrie(const std::string& tag) {
    return false;
  }

  inline std::list<std::string> entries() {
    return {};
  }

  inline std::string version() const {
    return {};
  }

  inline std::list<std::string> parents() {
    return {};
  }

private:

  Database& db_;

  std::string                   data_;
  std::list<std::string>        tag_list_;
  std::vector<std::string>      parents_;

  types::blob_storage blob_;
  std::string version_;

};

}


#endif /* SRC_TURBO_BROCCOLI_TYPE_TAGIMPL_HPP_ */
