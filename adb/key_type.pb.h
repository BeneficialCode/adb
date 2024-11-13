// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: key_type.proto
// Protobuf C++ Version: 4.25.0

#ifndef GOOGLE_PROTOBUF_INCLUDED_key_5ftype_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_key_5ftype_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/port_def.inc"
#if PROTOBUF_VERSION < 4025000
#error "This file was generated by a newer version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please update"
#error "your headers."
#endif  // PROTOBUF_VERSION

#if 4025000 < PROTOBUF_MIN_PROTOC_VERSION
#error "This file was generated by an older version of protoc which is"
#error "incompatible with your Protocol Buffer headers. Please"
#error "regenerate this file with a newer version of protoc."
#endif  // PROTOBUF_MIN_PROTOC_VERSION
#include "google/protobuf/port_undef.inc"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/generated_enum_reflection.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_key_5ftype_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_key_5ftype_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_key_5ftype_2eproto;
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace adb {
namespace proto {
enum KeyType : int {
  RSA_2048 = 0,
  KeyType_INT_MIN_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::min(),
  KeyType_INT_MAX_SENTINEL_DO_NOT_USE_ =
      std::numeric_limits<::int32_t>::max(),
};

bool KeyType_IsValid(int value);
extern const uint32_t KeyType_internal_data_[];
constexpr KeyType KeyType_MIN = static_cast<KeyType>(0);
constexpr KeyType KeyType_MAX = static_cast<KeyType>(0);
constexpr int KeyType_ARRAYSIZE = 0 + 1;
const ::google::protobuf::EnumDescriptor*
KeyType_descriptor();
template <typename T>
const std::string& KeyType_Name(T value) {
  static_assert(std::is_same<T, KeyType>::value ||
                    std::is_integral<T>::value,
                "Incorrect type passed to KeyType_Name().");
  return KeyType_Name(static_cast<KeyType>(value));
}
template <>
inline const std::string& KeyType_Name(KeyType value) {
  return ::google::protobuf::internal::NameOfDenseEnum<KeyType_descriptor,
                                                 0, 0>(
      static_cast<int>(value));
}
inline bool KeyType_Parse(absl::string_view name, KeyType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<KeyType>(
      KeyType_descriptor(), name, value);
}

// ===================================================================



// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
}  // namespace adb


namespace google {
namespace protobuf {

template <>
struct is_proto_enum<::adb::proto::KeyType> : std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor<::adb::proto::KeyType>() {
  return ::adb::proto::KeyType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_key_5ftype_2eproto_2epb_2eh
