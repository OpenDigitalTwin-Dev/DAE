// Copyright 2009, Willow Garage, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the  Willow Garage, Inc. nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef RESOURCE_RETRIEVER__RETRIEVER_HPP_
#define RESOURCE_RETRIEVER__RETRIEVER_HPP_

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "resource_retriever/visibility_control.hpp"

using CURL = void;

namespace resource_retriever
{
class Exception : public std::runtime_error
{
public:
  Exception(const std::string & file, const std::string & error_msg)
  : std::runtime_error("Error retrieving file [" + file + "]: " + error_msg)
  {
  }
};

/**
 * \brief A combination of a pointer to data in memory along with the data's size.
 */
struct MemoryResource
{
  std::shared_ptr<uint8_t> data;
  size_t size {0};
};

/**
 * \brief Retrieves files from from a url.  Caches a CURL handle so multiple accesses to a single url
 * will keep connections open.
 */
class RESOURCE_RETRIEVER_PUBLIC Retriever
{
public:
  Retriever();

  ~Retriever();

  Retriever(const Retriever & ret) = delete;
  Retriever & operator=(const Retriever & other) = delete;

  Retriever(Retriever && other) noexcept;
  Retriever & operator=(Retriever && other) noexcept;

  /**
   * \brief Get a file and store it in memory
   * \param url The url to retrieve.  package://package/file will be turned into the correct file:// invocation
   * \return The file, loaded into memory
   * \throws resource_retriever::Exception if anything goes wrong.
   */
  MemoryResource get(const std::string & url);

private:
  CURL * curl_handle_ {nullptr};
};

}  //  namespace resource_retriever

#endif  //  RESOURCE_RETRIEVER__RETRIEVER_HPP_
