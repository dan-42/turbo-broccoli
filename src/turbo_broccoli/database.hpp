/*
 * database.hpp
 *
 *  Created on: Sep 22, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DATABASE_HPP_
#define SRC_TURBO_BROCCOLI_DATABASE_HPP_

#include <turbo_broccoli/type/key.hpp>
#include <turbo_broccoli/type/value.hpp>
#include <turbo_broccoli/type/tags.hpp>
#include <turbo_broccoli/type/tagged_records.hpp>
#include <turbo_broccoli/type/result_find.hpp>
#include <turbo_broccoli/type/result_key.hpp>

#include <turbo_broccoli/detail/utils.hpp>
#include <turbo_broccoli/detail/storage.hpp>
#include <turbo_broccoli/type/blob_storage.hpp>
#include <turbo_broccoli/type/blobimpl.hpp>

#include <turbo_broccoli/type/tagimpl.hpp>

namespace turbo_broccoli {


using types::hash_t;
using types::result_key;


struct database {

  using blob_t = blob_impl<database>;
  using tag_t  = tag_impl<database>;

  friend blob_t;
  friend tag_t;

  database(const std::string& path) : storage_(path) {

  }

  inline blob_t blob(const std::string& key) {
    return blob_t{*this, key};
  }

  inline tag_t tag(const std::string& key) {
    return tag_t{*this, key};
  }



private:

  inline std::pair<bool, std::string> store_blob(   const std::string&                key,
                                                    const std::string&                data,
                                                    const std::vector<std::string>&   tag_list,
                                                    const std::string&                version,
                                                    const std::vector<std::string>&   parents) {

    auto hash_key = detail::hash_data(key);
    if(storage_.record_exists(hash_key)) {

      auto head_version_key = load_reference(hash_key);
      if(head_version_key.compare(version) == 0) {

        types::tag_list tags{tag_list};
        types::blob_storage new_blob{data, tag_list,  parents};
        auto new_version = store_new_blob(new_blob);
        if(new_version.empty()) {
          std::cout << "database store_blob new_version empty" << std::endl;
          return {false, ""};
        }

        if( update_reference(hash_key, new_version) == false) {
          std::cout << "database store_blob update ref" << std::endl;
          return {false, ""};
        }
        return {true , new_version};
      }
      else {
        // conflict!
        std::cout << "database store_blob  confilict" << std::endl;
      }

      return {false, ""};
    }
    else {
      types::tag_list tags{tag_list};
      types::blob_storage new_blob{data, tag_list, {""} };
      auto new_version = store_new_blob(new_blob);
      if(new_version.empty()) {

        return {false, ""};
      }
      if(new_reference(hash_key, new_version) == false) {

        return {false, ""};
      }
      return {true , new_version};
    }
  }



  inline bool load_blob(const std::string&              key,
                              std::string&              data,
                              std::vector<std::string>& tag_list,
                              std::string&              version,
                              std::vector<std::string>& parents) {

    auto hash_key = detail::hash_data(key);
    if(!storage_.record_exists(hash_key)) {
      std::cout << "database load_blob  !record_exists" << std::endl;
      return false;
    }

    auto value = load_value(hash_key);
    if(!types::is_reference(value)) {
      std::cout << "database load_blob  !is_reference" << std::endl;
      return false;
    }

    auto head_version = types::string_to_key(value.data);
    if(!storage_.record_exists(head_version)) {
      std::cout << "database load_blob  !record_exists version " << value.data << std::endl;
      return false;
    }

    auto blob = load_existing_blob(head_version);
    std::cout << "database load_blob   value.data " << value.data << std::endl;
    std::cout << "database load_blob   blob.data(); " << blob.data() << std::endl;

    data      = blob.data();
    tag_list  = blob.tags();
    version   = value.data;
    parents   = blob.parents();

    return true;
  }


/*

  inline types::value_t read_record(const hash_t& k) {
    namespace fs = boost::filesystem;
    auto tmp = storage_.load(k);
    return detail::deserialize<types::value_t>(tmp);
  }

  inline void update_tag_add(const std::string& tag_name, const std::string& record_key) {
    auto tag_key = detail::hash_data(tag_name);
    types::value_t v;
    types::tagged_records records;

    if(storage_.record_exists(tag_key)) {
      v = read_record(tag_key);
      if(types::is_tag_list(v)) {
        records = detail::deserialize<types::tagged_records>(v.data);
        for(auto& r : records.keys) {
          if(record_key.compare(r) == 0) {
            return;
          }
        }
        records.keys.push_back(record_key);
      }
      else {
        throw std::runtime_error("record exissts and is not a tagged_list: " + tag_name);
      }
    }
    else {
      records.keys.push_back(record_key);
      v.key           = tag_name;
      v.reccord_type  = types::value_type::tag_list;
      v.data          = detail::serialize(records);
    }
    v.data          = detail::serialize(records);
    storage_.store(tag_key,  detail::serialize(v));
  }

  inline void update_tag_remove(const std::string& tag_name, const std::string& record_key) {
    auto tag_key = detail::hash_data(tag_name);
    types::value_t v = read_record(tag_key);

    if(types::is_tag_list(v)) {
      types::tagged_records records = detail::deserialize<types::tagged_records>(v.data);
      records.keys.erase(std::remove(records.keys.begin(), records.keys.end(), record_key), records.keys.end());
      v.data = detail::serialize(records);
      storage_.store(tag_key,  detail::serialize(v));
    }
  }

*/


  inline std::string store_new_blob(const turbo_broccoli::types::blob_storage &blob) {

    auto data = detail::serialize(blob);
    if(data.empty()) {
      return "";
    }
    return store_hashed_data(data);
  }

  inline std::string store_hashed_data(const std::string& data) {
    try {
      auto key = detail::hash_data(data);
      if(storage_.store(key, data)) {
        return types::to_string(key);
      }
      else {
        return {""};
      }
    }
    catch(...) {
      return {""};
    }
  }

  inline types::blob_storage  load_existing_blob(const turbo_broccoli::types::hash_t &key) {
    auto value = load_value(key);
    std::cout << "load_existing_blob: " << value.data << std::endl;
    if(types::is_blob(value)) {
      return detail::deserialize<types::blob_storage>(value.data);
    }
    else {
      return {};
    }
  }

  inline bool new_reference(const turbo_broccoli::types::hash_t& key, const std::string& version) {
    types::value_t value{types::value_type::reference, version};
    if(store_value(key, value)) {

      return true;
    }
    std::cout << "database update_reference fail "<< std::endl;
    return false;
  }


  inline std::string  load_reference(const turbo_broccoli::types::hash_t &key) {
    auto value = load_value(key);
    if(types::is_reference(value)) {
      return value.data;
    }
    else {
      return {""};
    }
  }


  inline bool update_reference(const turbo_broccoli::types::hash_t& key, const std::string& version) {
    auto value = load_value(key);
    if(types::is_reference(value)) {
      value.data = version;
      store_value(key, value);
    }
    std::cout << "database update_reference fail "<< std::endl;
    return false;
  }

  inline types::value_t load_value(const turbo_broccoli::types::hash_t &key) {
    auto data = storage_.load(key);
    std::cout << "load_value(" << types::to_string(key) << ") "  << data << std::endl << std::endl;
    return detail::deserialize<types::value_t>(data);
  }

  inline bool store_value(const turbo_broccoli::types::hash_t &key, const turbo_broccoli::types::value_t &value) {
    auto data = detail::serialize(value);
    return storage_.store(key, data);
  }


  detail::storage storage_;

};


}




#endif /* SRC_TURBO_BROCCOLI_DATABASE_HPP_ */
