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
#include <turbo_broccoli/type/blob.hpp>
#include <turbo_broccoli/type/tags.hpp>
#include <turbo_broccoli/type/tagged_records.hpp>
#include <turbo_broccoli/type/result_find.hpp>
#include <turbo_broccoli/type/result_key.hpp>

#include <turbo_broccoli/detail/utils.hpp>
#include <turbo_broccoli/detail/storage.hpp>

namespace turbo_broccoli {

struct database;

struct blob {
  //tag_list
  //data
  //parent_1
  //parent_2
  //how to handle merge issues?

private:
  database& db_;
};



using types::db_key;
using types::result_key;
using types::result_find;

struct database {

  database(const std::string& path) : storage_(path) {
  }


  result_find find(const std::string& key) {
    return find(detail::calculate_key(key));
  }

  result_find find(const db_key& key) {

    result_find result{};
    result.success = false;



    if(!value_exists(key)) {
      std::cout << "no record with key" << types::to_string(key) << std::endl;
      return result;
    }

    auto value = load_value(key);


    if(!is_reference(value)) {
      std::cout << "no is_reference with key" << types::to_string(key) <<  " DATA INCONSISTENT "<< std::endl;
      return result;
    }

    auto head_version = types::string_to_key(value.data);
    if(!value_exists(head_version)) {
      std::cout << "no head_version with key" << types::to_string(key) <<  " DATA INCONSISTENT "<< std::endl;
      return result;
    }

    auto head_version_value = load_value(head_version);
>>>>>>>
    if( is_blob(head_version_value)) {
      result.success = true;
      result.results.push_back(detail::deserialize<types::blob>(head_version_value.data));
      return result;
    }

    if(is_tag_list(head_version_value)) {

      types::tagged_records records = detail::deserialize<types::tagged_records>(head_version_value.data);
      for(auto& t : records.keys) {

        auto k = types::string_to_key(t);
        if(value_exists(k)) {

          auto r = read_record(k);
          if( is_blob(r)) {
            result.success = true;
            result.results.push_back(detail::deserialize<types::blob>(r.data));
          }
          else {
            std::cout << "inconsistent: record is not blob " << t << std::endl;
          }
        }
        else {
          std::cout << "inconsistent no record from tag list " << t << std::endl;
        }
      }
    }


    return result;
  }


  result_key store(const types::blob& new_blob) {

    static const result_key failed_result{false, turbo_broccoli::types::nil_key() };

    if(value_exists(new_blob.key_hash())) {
      /*
       * read all tags and update them!
       */
      auto r = read_record(new_blob.key_hash());
      auto old_blob = detail::deserialize<types::blob>(r.data);

      types::tag_list::list_type to_delete = detail::diff( old_blob.tags().tags, new_blob.tags().tags);
      types::tag_list::list_type to_add    = detail::diff( new_blob.tags().tags, old_blob.tags().tags);

      for(auto& t : to_add ) {
        update_tag_add(t, types::to_string(new_blob.key_hash()));
      }
      for(auto& t : to_delete ) {
        update_tag_remove(t, types::to_string(new_blob.key_hash()));
      }
    }
    else {
      for(auto& t : new_blob.tags().tags ) {
        update_tag_add(t, types::to_string(new_blob.key_hash()));
      }
    }
    write_blob(new_blob);
    return {true, new_blob.key_hash()};



    return failed_result;
  }



private:

  inline bool value_exists(const db_key& key) {
    return storage_.record_exists(key);
  }

  inline types::value_t load_value(const db_key& key) {
    auto data = storage_.load(key);
    return detail::deserialize<types::value_t>(data);
  }

  inline void write_blob(const types::blob& b) {
    namespace fs = boost::filesystem;
    types::value_t v;
    v.data = detail::serialize(b);
    v.reccord_type = types::value_type::blob;
    v.key = b.key();

    storage_.store(b.key_hash(), detail::serialize(v));
  }

  inline types::value_t read_record(const db_key& k) {
    namespace fs = boost::filesystem;
    auto tmp = storage_.load(k);
    return detail::deserialize<types::value_t>(tmp);
  }

  inline void update_tag_add(const std::string& tag_name, const std::string& record_key) {
    auto tag_key = detail::calculate_key(tag_name);
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
    auto tag_key = detail::calculate_key(tag_name);
    types::value_t v = read_record(tag_key);

    if(types::is_tag_list(v)) {
      types::tagged_records records = detail::deserialize<types::tagged_records>(v.data);
      records.keys.erase(std::remove(records.keys.begin(), records.keys.end(), record_key), records.keys.end());
      v.data = detail::serialize(records);
      storage_.store(tag_key,  detail::serialize(v));
    }
  }


  detail::storage storage_;

};


}




#endif /* SRC_TURBO_BROCCOLI_DATABASE_HPP_ */
