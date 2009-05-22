#ifndef _QmfValueImpl_
#define _QmfValueImpl_

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <qmf/Value.h>
#include <qmf/ObjectIdImpl.h>
#include <qmf/Object.h>
#include <qpid/framing/Buffer.h>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace qmf {

    // TODO: set valid flag on all value settors
    // TODO: add a modified flag and accessors

    struct ValueImpl {
        typedef boost::shared_ptr<Value> VPtr;
        typedef boost::shared_ptr<Object> OPtr;
        Value* envelope;
        const Typecode typecode;
        bool valid;

        ObjectId refVal;
        std::string stringVal;
        OPtr objectVal;
        std::map<std::string, VPtr> mapVal;
        std::vector<VPtr> vectorVal;
        Typecode arrayTypecode;

        union {
            uint32_t u32;
            uint64_t u64;
            int32_t  s32;
            int64_t  s64;
            bool     boolVal;
            float    floatVal;
            double   doubleVal;
            uint8_t  uuidVal[16];
        } value;

        ValueImpl(Value* e, Typecode t, Typecode at) :
            envelope(e), typecode(t), valid(false), arrayTypecode(at) {}
        ValueImpl(Typecode t, qpid::framing::Buffer& b);
        ValueImpl(Typecode t);
        ~ValueImpl();

        void encode(qpid::framing::Buffer& b) const;

        Typecode getType() const { return typecode; }
        bool isNull() const { return !valid; }
        void setNull() { valid = false; }

        bool isObjectId() const { return typecode == TYPE_REF; }
        const ObjectId& asObjectId() const { return refVal; }
        void setObjectId(const ObjectId& o) { refVal = o; }  // TODO

        bool isUint() const { return typecode >= TYPE_UINT8 && typecode <= TYPE_UINT32; }
        uint32_t asUint() const { return value.u32; }
        void setUint(uint32_t val) { value.u32 = val; }

        bool isInt() const { return typecode >= TYPE_INT8 && typecode <= TYPE_INT32; }
        int32_t asInt() const { return value.s32; }
        void setInt(int32_t val) { value.s32 = val; }

        bool isUint64() const { return typecode == TYPE_UINT64 || typecode == TYPE_DELTATIME; }
        uint64_t asUint64() const { return value.u64; }
        void setUint64(uint64_t val) { value.u64 = val; }

        bool isInt64() const { return typecode == TYPE_INT64 || typecode == TYPE_ABSTIME; }
        int64_t asInt64() const { return value.s64; }
        void setInt64(int64_t val) { value.s64 = val; }

        bool isString() const { return typecode == TYPE_SSTR || typecode == TYPE_LSTR; }
        const char* asString() const { return stringVal.c_str(); }
        void setString(const char* val) { stringVal = val; }

        bool isBool() const { return typecode == TYPE_BOOL; }
        bool asBool() const { return value.boolVal; }
        void setBool(bool val) { value.boolVal = val; }

        bool isFloat() const { return typecode == TYPE_FLOAT; }
        float asFloat() const { return value.floatVal; }
        void setFloat(float val) { value.floatVal = val; }

        bool isDouble() const { return typecode == TYPE_DOUBLE; }
        double asDouble() const { return value.doubleVal; }
        void setDouble(double val) { value.doubleVal = val; }

        bool isUuid() const { return typecode == TYPE_UUID; }
        const uint8_t* asUuid() const { return value.uuidVal; }
        void setUuid(const uint8_t* val) { ::memcpy(value.uuidVal, val, 16); }

        bool isObject() const { return typecode == TYPE_OBJECT; }
        Object* asObject() const { return objectVal.get(); }
        void setObject(Object* val) { objectVal.reset(val); }

        bool isMap() const { return typecode == TYPE_MAP; }
        bool keyInMap(const char* key) const;
        Value* byKey(const char* key);
        const Value* byKey(const char* key) const;
        void deleteKey(const char* key);
        void insert(const char* key, Value* val);
        uint32_t keyCount() const { return mapVal.size(); }
        const char* key(uint32_t idx) const;

        bool isList() const { return typecode == TYPE_LIST; }
        uint32_t listItemCount() const { return vectorVal.size(); }
        Value* listItem(uint32_t idx);
        void appendToList(Value* val);
        void deleteListItem(uint32_t idx);

        bool isArray() const { return typecode == TYPE_ARRAY; }
        Typecode arrayType() const { return arrayTypecode; }
        uint32_t arrayItemCount() const { return vectorVal.size(); }
        Value* arrayItem(uint32_t idx);
        void appendToArray(Value* val);
        void deleteArrayItem(uint32_t idx);
    };
}

#endif

