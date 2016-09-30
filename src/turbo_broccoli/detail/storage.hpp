/*
 * storage.hpp
 *
 *  Created on: Sep 26, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DETAIL_STORAGE_HPP_
#define SRC_TURBO_BROCCOLI_DETAIL_STORAGE_HPP_

#include <boost/filesystem.hpp>
#include <turbo_broccoli/type/key.hpp>

namespace turbo_broccoli { namespace detail {


struct storage {

  storage(const std::string& db) : prefix_(db) {
    namespace fs = boost::filesystem;
    if(!fs::exists(prefix_) ) {
     if(!fs::create_directories(prefix_)) {
       throw std::runtime_error("cannot open db, cannot create directory: " + prefix_.generic_string());
     }
    }
    else {
     if(!fs::is_directory(prefix_)) {
       throw std::runtime_error("cannot open db, is not a directory: " + prefix_.generic_string());
     }
    }
  }

  inline bool store(const turbo_broccoli::types::hash_t &key, const std::string& data) {
    namespace fs = boost::filesystem;
    auto path = to_filename(key);
    if(fs::exists(path) && fs::is_regular_file(path)) {
      return write_file(path.generic_string(), data);
    }
    else {
      if(create_folder(key) ) {
        return write_file(path.generic_string(), data);
      }
    }
    return false;
  }

  inline std::string load(const turbo_broccoli::types::hash_t &key) {
    namespace fs = boost::filesystem;
    auto path = to_filename(key);
    if(fs::exists(path) && fs::is_regular_file(path)) {
      return read_file(path.generic_string());
    }
    return {};
  }

  inline bool remove(const turbo_broccoli::types::hash_t &key) {
    namespace fs = boost::filesystem;
    auto path = to_filename(key);
    if(fs::exists(path) && fs::is_regular_file(path)) {
      return fs::remove(path);
    }
    return false;
  }

  inline bool record_exists(const turbo_broccoli::types::hash_t &key) {
    namespace fs = boost::filesystem;
    auto file = to_filename(key);
    return fs::exists(file) && fs::is_regular_file(file);
  }

private:

  // fastes c++ impl according to  http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html
  inline std::string read_file(const std::string& filename) {
   std::ifstream in(filename, std::ios::in | std::ios::binary);
     if (in) {
       std::string contents;
       in.seekg(0, std::ios::end);
       contents.resize(in.tellg());
       in.seekg(0, std::ios::beg);
       in.read(&contents[0], contents.size());
       in.close();
       return contents;
     }
     throw(errno);
   return {};
  }

  inline bool write_file(const std::string& filename, const std::string& data) {
    try {
     std::ofstream ofs(filename, std::ios::out | std::ios::binary | std::ios::trunc);
     ofs.write(&data[0], data.size());
     ofs.flags();
     ofs.close();
     return true;
    } catch(...) {
      return false;
    }
  }


  inline bool create_folder(const turbo_broccoli::types::hash_t& key) {
   namespace fs = boost::filesystem;
   return fs::create_directories(prefix_ / fs::path(pre::bytes::to_hexstring(key.data(), 2)));
  }

  inline boost::filesystem::path to_filename(const turbo_broccoli::types::hash_t &k) {
   namespace fs = boost::filesystem;
   return prefix_ / fs::path(pre::bytes::to_hexstring(&k[0], 2)) / fs::path(pre::bytes::to_hexstring(&k[2] , 18));
  }

  boost::filesystem::path prefix_;

};


}}

#endif /* SRC_TURBO_BROCCOLI_DETAIL_STORAGE_HPP_ */
