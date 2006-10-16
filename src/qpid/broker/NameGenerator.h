/*
 *
 * Copyright (c) 2006 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _NameGenerator_
#define _NameGenerator_

#include "qpid/broker/Message.h"

namespace qpid {
    namespace broker {
        class NameGenerator{
            const std::string base;
            unsigned int counter;
        public:
            NameGenerator(const std::string& base);
            std::string generate();
        };
    }
}


#endif
