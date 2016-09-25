/*
 * utils.hpp
 *
 *  Created on: Sep 24, 2016
 *      Author: dan
 */

#ifndef SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_
#define SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_

#include <cstdint>
#include <string>
#include <array>

#include <nlohmann/json.hpp>

#include <boost/uuid/sha1.hpp>
#include <pre/bytes/utils.hpp>
#include <pre/json/to_json.hpp>
#include <pre/json/from_json.hpp>

#include <turbo_broccoli/type/key.hpp>

#include <turbo_broccoli/detail/traits.hpp>

namespace turbo_broccoli { namespace detail {



  inline turbo_broccoli::types::db_key to_key(const unsigned int hash[5]) {
    auto h = reinterpret_cast<const uint8_t*>(hash);
    turbo_broccoli::types::db_key k;
    for(int i = 0; i < 5; ++i)  {
      k[i*4]  = h[i*4+3];
      k[i*4+1] = h[i*4+2];
      k[i*4+2] = h[i*4+1];
      k[i*4+3] = h[i*4];
    }
    return k;
  }


  inline turbo_broccoli::types::db_key calculate_key(const std::string data) {
    try {
      boost::uuids::detail::sha1 hasher{};
      for(auto &c : data ) {
        hasher.process_byte(c);
      }

      unsigned int hash[5];
      hasher.get_digest(hash);
      return to_key(hash);
    }
    catch(...) {
      return turbo_broccoli::types::nil_key();
    }
  }


  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  inline std::string serialize(const T& b) {
    try {
      return pre::json::to_json(b).dump(2);
    }
    catch(...) {
      return {""};
    }
  }

  template<typename T, detail::enable_if_is_adapted_struct_t<T>* = nullptr>
  inline T deserialize(const std::string& data) {
    try {
      return pre::json::from_json<T>(nlohmann::json::parse(data));
    }
    catch(...) {
      return T{};
    }
  }

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

  inline void write_file(const std::string& filename, const std::string& data) {
    std::ofstream ofs(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    ofs.write(&data[0], data.size());
    ofs.flags();
    ofs.close();
  }


  inline void create_folder(const boost::filesystem::path& prefix, const turbo_broccoli::types::db_key& key) {
    namespace fs = boost::filesystem;
    fs::create_directories(prefix / fs::path(pre::bytes::to_hexstring(key.data(), 2)));
  }

  inline boost::filesystem::path to_filename(const boost::filesystem::path& prefix, const turbo_broccoli::types::db_key &k) {
    namespace fs = boost::filesystem;
    return prefix / fs::path(pre::bytes::to_hexstring(&k[0], 2)) / fs::path(pre::bytes::to_hexstring(&k[2] , 18));
  }

} //detail
} //turbo_broccoli



#endif /* SRC_TURBO_BROCCOLI_DETAIL_UTILS_HPP_ */
