// Copyright 2024 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>

#include <openssl/crypto.h>

namespace eprosima {
namespace fastrtps {
namespace rtps {
namespace security {

class OpenSSLInit
{
public:

    OpenSSLInit()
    {
        uint64_t opts = OPENSSL_INIT_NO_ATEXIT;
        OPENSSL_init_crypto(opts, NULL);
    }

    ~OpenSSLInit()
    {
        OPENSSL_cleanup();
    }

    static std::shared_ptr<OpenSSLInit> get_instance()
    {
        static auto instance = std::make_shared<OpenSSLInit>();
        return instance;
    }

};

} // namespace security
} // namespace rtps
} // namespace fastrtps
} // namespace eprosima
