// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ApkEntry.proto

#include "ApkEntry.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace com {
namespace android {
namespace fastdeploy {
PROTOBUF_CONSTEXPR APKDump::APKDump(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.name_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.cd_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.signature_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.absolute_path_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct APKDumpDefaultTypeInternal {
  PROTOBUF_CONSTEXPR APKDumpDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~APKDumpDefaultTypeInternal() {}
  union {
    APKDump _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 APKDumpDefaultTypeInternal _APKDump_default_instance_;
PROTOBUF_CONSTEXPR APKEntry::APKEntry(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.md5_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.dataoffset_)*/int64_t{0}
  , /*decltype(_impl_.datasize_)*/int64_t{0}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct APKEntryDefaultTypeInternal {
  PROTOBUF_CONSTEXPR APKEntryDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~APKEntryDefaultTypeInternal() {}
  union {
    APKEntry _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 APKEntryDefaultTypeInternal _APKEntry_default_instance_;
PROTOBUF_CONSTEXPR APKMetaData::APKMetaData(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.entries_)*/{}
  , /*decltype(_impl_.absolute_path_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct APKMetaDataDefaultTypeInternal {
  PROTOBUF_CONSTEXPR APKMetaDataDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~APKMetaDataDefaultTypeInternal() {}
  union {
    APKMetaData _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 APKMetaDataDefaultTypeInternal _APKMetaData_default_instance_;
}  // namespace fastdeploy
}  // namespace android
}  // namespace com
static ::_pb::Metadata file_level_metadata_ApkEntry_2eproto[3];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_ApkEntry_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_ApkEntry_2eproto = nullptr;

const uint32_t TableStruct_ApkEntry_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKDump, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKDump, _impl_.name_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKDump, _impl_.cd_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKDump, _impl_.signature_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKDump, _impl_.absolute_path_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKEntry, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKEntry, _impl_.md5_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKEntry, _impl_.dataoffset_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKEntry, _impl_.datasize_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKMetaData, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKMetaData, _impl_.absolute_path_),
  PROTOBUF_FIELD_OFFSET(::com::android::fastdeploy::APKMetaData, _impl_.entries_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::com::android::fastdeploy::APKDump)},
  { 10, -1, -1, sizeof(::com::android::fastdeploy::APKEntry)},
  { 19, -1, -1, sizeof(::com::android::fastdeploy::APKMetaData)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::com::android::fastdeploy::_APKDump_default_instance_._instance,
  &::com::android::fastdeploy::_APKEntry_default_instance_._instance,
  &::com::android::fastdeploy::_APKMetaData_default_instance_._instance,
};

const char descriptor_table_protodef_ApkEntry_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016ApkEntry.proto\022\026com.android.fastdeploy"
  "\"M\n\007APKDump\022\014\n\004name\030\001 \001(\t\022\n\n\002cd\030\002 \001(\014\022\021\n"
  "\tsignature\030\003 \001(\014\022\025\n\rabsolute_path\030\004 \001(\t\""
  "=\n\010APKEntry\022\013\n\003md5\030\001 \001(\014\022\022\n\ndataOffset\030\002"
  " \001(\003\022\020\n\010dataSize\030\003 \001(\003\"W\n\013APKMetaData\022\025\n"
  "\rabsolute_path\030\001 \001(\t\0221\n\007entries\030\002 \003(\0132 ."
  "com.android.fastdeploy.APKEntryB)\n\026com.a"
  "ndroid.fastdeployB\rApkEntryProtoP\001b\006prot"
  "o3"
  ;
static ::_pbi::once_flag descriptor_table_ApkEntry_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_ApkEntry_2eproto = {
    false, false, 322, descriptor_table_protodef_ApkEntry_2eproto,
    "ApkEntry.proto",
    &descriptor_table_ApkEntry_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_ApkEntry_2eproto::offsets,
    file_level_metadata_ApkEntry_2eproto, file_level_enum_descriptors_ApkEntry_2eproto,
    file_level_service_descriptors_ApkEntry_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_ApkEntry_2eproto_getter() {
  return &descriptor_table_ApkEntry_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_ApkEntry_2eproto(&descriptor_table_ApkEntry_2eproto);
namespace com {
namespace android {
namespace fastdeploy {

// ===================================================================

class APKDump::_Internal {
 public:
};

APKDump::APKDump(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:com.android.fastdeploy.APKDump)
}
APKDump::APKDump(const APKDump& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  APKDump* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.name_){}
    , decltype(_impl_.cd_){}
    , decltype(_impl_.signature_){}
    , decltype(_impl_.absolute_path_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_name().empty()) {
    _this->_impl_.name_.Set(from._internal_name(), 
      _this->GetArenaForAllocation());
  }
  _impl_.cd_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.cd_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_cd().empty()) {
    _this->_impl_.cd_.Set(from._internal_cd(), 
      _this->GetArenaForAllocation());
  }
  _impl_.signature_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.signature_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_signature().empty()) {
    _this->_impl_.signature_.Set(from._internal_signature(), 
      _this->GetArenaForAllocation());
  }
  _impl_.absolute_path_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.absolute_path_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_absolute_path().empty()) {
    _this->_impl_.absolute_path_.Set(from._internal_absolute_path(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:com.android.fastdeploy.APKDump)
}

inline void APKDump::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.name_){}
    , decltype(_impl_.cd_){}
    , decltype(_impl_.signature_){}
    , decltype(_impl_.absolute_path_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.cd_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.cd_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.signature_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.signature_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.absolute_path_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.absolute_path_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

APKDump::~APKDump() {
  // @@protoc_insertion_point(destructor:com.android.fastdeploy.APKDump)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void APKDump::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.name_.Destroy();
  _impl_.cd_.Destroy();
  _impl_.signature_.Destroy();
  _impl_.absolute_path_.Destroy();
}

void APKDump::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void APKDump::Clear() {
// @@protoc_insertion_point(message_clear_start:com.android.fastdeploy.APKDump)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.name_.ClearToEmpty();
  _impl_.cd_.ClearToEmpty();
  _impl_.signature_.ClearToEmpty();
  _impl_.absolute_path_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* APKDump::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string name = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_name();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "com.android.fastdeploy.APKDump.name"));
        } else
          goto handle_unusual;
        continue;
      // bytes cd = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          auto str = _internal_mutable_cd();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // bytes signature = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          auto str = _internal_mutable_signature();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string absolute_path = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_absolute_path();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "com.android.fastdeploy.APKDump.absolute_path"));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* APKDump::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:com.android.fastdeploy.APKDump)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "com.android.fastdeploy.APKDump.name");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_name(), target);
  }

  // bytes cd = 2;
  if (!this->_internal_cd().empty()) {
    target = stream->WriteBytesMaybeAliased(
        2, this->_internal_cd(), target);
  }

  // bytes signature = 3;
  if (!this->_internal_signature().empty()) {
    target = stream->WriteBytesMaybeAliased(
        3, this->_internal_signature(), target);
  }

  // string absolute_path = 4;
  if (!this->_internal_absolute_path().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_absolute_path().data(), static_cast<int>(this->_internal_absolute_path().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "com.android.fastdeploy.APKDump.absolute_path");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_absolute_path(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:com.android.fastdeploy.APKDump)
  return target;
}

size_t APKDump::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:com.android.fastdeploy.APKDump)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 1;
  if (!this->_internal_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // bytes cd = 2;
  if (!this->_internal_cd().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_cd());
  }

  // bytes signature = 3;
  if (!this->_internal_signature().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_signature());
  }

  // string absolute_path = 4;
  if (!this->_internal_absolute_path().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_absolute_path());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData APKDump::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    APKDump::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*APKDump::GetClassData() const { return &_class_data_; }


void APKDump::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<APKDump*>(&to_msg);
  auto& from = static_cast<const APKDump&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:com.android.fastdeploy.APKDump)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _this->_internal_set_name(from._internal_name());
  }
  if (!from._internal_cd().empty()) {
    _this->_internal_set_cd(from._internal_cd());
  }
  if (!from._internal_signature().empty()) {
    _this->_internal_set_signature(from._internal_signature());
  }
  if (!from._internal_absolute_path().empty()) {
    _this->_internal_set_absolute_path(from._internal_absolute_path());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void APKDump::CopyFrom(const APKDump& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:com.android.fastdeploy.APKDump)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool APKDump::IsInitialized() const {
  return true;
}

void APKDump::InternalSwap(APKDump* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.name_, lhs_arena,
      &other->_impl_.name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.cd_, lhs_arena,
      &other->_impl_.cd_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.signature_, lhs_arena,
      &other->_impl_.signature_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.absolute_path_, lhs_arena,
      &other->_impl_.absolute_path_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata APKDump::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_ApkEntry_2eproto_getter, &descriptor_table_ApkEntry_2eproto_once,
      file_level_metadata_ApkEntry_2eproto[0]);
}

// ===================================================================

class APKEntry::_Internal {
 public:
};

APKEntry::APKEntry(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:com.android.fastdeploy.APKEntry)
}
APKEntry::APKEntry(const APKEntry& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  APKEntry* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.md5_){}
    , decltype(_impl_.dataoffset_){}
    , decltype(_impl_.datasize_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.md5_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.md5_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_md5().empty()) {
    _this->_impl_.md5_.Set(from._internal_md5(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.dataoffset_, &from._impl_.dataoffset_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.datasize_) -
    reinterpret_cast<char*>(&_impl_.dataoffset_)) + sizeof(_impl_.datasize_));
  // @@protoc_insertion_point(copy_constructor:com.android.fastdeploy.APKEntry)
}

inline void APKEntry::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.md5_){}
    , decltype(_impl_.dataoffset_){int64_t{0}}
    , decltype(_impl_.datasize_){int64_t{0}}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.md5_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.md5_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

APKEntry::~APKEntry() {
  // @@protoc_insertion_point(destructor:com.android.fastdeploy.APKEntry)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void APKEntry::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.md5_.Destroy();
}

void APKEntry::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void APKEntry::Clear() {
// @@protoc_insertion_point(message_clear_start:com.android.fastdeploy.APKEntry)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.md5_.ClearToEmpty();
  ::memset(&_impl_.dataoffset_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.datasize_) -
      reinterpret_cast<char*>(&_impl_.dataoffset_)) + sizeof(_impl_.datasize_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* APKEntry::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bytes md5 = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_md5();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 dataOffset = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.dataoffset_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int64 dataSize = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.datasize_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* APKEntry::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:com.android.fastdeploy.APKEntry)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bytes md5 = 1;
  if (!this->_internal_md5().empty()) {
    target = stream->WriteBytesMaybeAliased(
        1, this->_internal_md5(), target);
  }

  // int64 dataOffset = 2;
  if (this->_internal_dataoffset() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(2, this->_internal_dataoffset(), target);
  }

  // int64 dataSize = 3;
  if (this->_internal_datasize() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt64ToArray(3, this->_internal_datasize(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:com.android.fastdeploy.APKEntry)
  return target;
}

size_t APKEntry::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:com.android.fastdeploy.APKEntry)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes md5 = 1;
  if (!this->_internal_md5().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_md5());
  }

  // int64 dataOffset = 2;
  if (this->_internal_dataoffset() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_dataoffset());
  }

  // int64 dataSize = 3;
  if (this->_internal_datasize() != 0) {
    total_size += ::_pbi::WireFormatLite::Int64SizePlusOne(this->_internal_datasize());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData APKEntry::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    APKEntry::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*APKEntry::GetClassData() const { return &_class_data_; }


void APKEntry::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<APKEntry*>(&to_msg);
  auto& from = static_cast<const APKEntry&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:com.android.fastdeploy.APKEntry)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_md5().empty()) {
    _this->_internal_set_md5(from._internal_md5());
  }
  if (from._internal_dataoffset() != 0) {
    _this->_internal_set_dataoffset(from._internal_dataoffset());
  }
  if (from._internal_datasize() != 0) {
    _this->_internal_set_datasize(from._internal_datasize());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void APKEntry::CopyFrom(const APKEntry& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:com.android.fastdeploy.APKEntry)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool APKEntry::IsInitialized() const {
  return true;
}

void APKEntry::InternalSwap(APKEntry* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.md5_, lhs_arena,
      &other->_impl_.md5_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(APKEntry, _impl_.datasize_)
      + sizeof(APKEntry::_impl_.datasize_)
      - PROTOBUF_FIELD_OFFSET(APKEntry, _impl_.dataoffset_)>(
          reinterpret_cast<char*>(&_impl_.dataoffset_),
          reinterpret_cast<char*>(&other->_impl_.dataoffset_));
}

::PROTOBUF_NAMESPACE_ID::Metadata APKEntry::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_ApkEntry_2eproto_getter, &descriptor_table_ApkEntry_2eproto_once,
      file_level_metadata_ApkEntry_2eproto[1]);
}

// ===================================================================

class APKMetaData::_Internal {
 public:
};

APKMetaData::APKMetaData(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:com.android.fastdeploy.APKMetaData)
}
APKMetaData::APKMetaData(const APKMetaData& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  APKMetaData* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.entries_){from._impl_.entries_}
    , decltype(_impl_.absolute_path_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.absolute_path_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.absolute_path_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_absolute_path().empty()) {
    _this->_impl_.absolute_path_.Set(from._internal_absolute_path(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:com.android.fastdeploy.APKMetaData)
}

inline void APKMetaData::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.entries_){arena}
    , decltype(_impl_.absolute_path_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.absolute_path_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.absolute_path_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

APKMetaData::~APKMetaData() {
  // @@protoc_insertion_point(destructor:com.android.fastdeploy.APKMetaData)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void APKMetaData::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.entries_.~RepeatedPtrField();
  _impl_.absolute_path_.Destroy();
}

void APKMetaData::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void APKMetaData::Clear() {
// @@protoc_insertion_point(message_clear_start:com.android.fastdeploy.APKMetaData)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.entries_.Clear();
  _impl_.absolute_path_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* APKMetaData::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string absolute_path = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_absolute_path();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "com.android.fastdeploy.APKMetaData.absolute_path"));
        } else
          goto handle_unusual;
        continue;
      // repeated .com.android.fastdeploy.APKEntry entries = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_entries(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* APKMetaData::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:com.android.fastdeploy.APKMetaData)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string absolute_path = 1;
  if (!this->_internal_absolute_path().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_absolute_path().data(), static_cast<int>(this->_internal_absolute_path().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "com.android.fastdeploy.APKMetaData.absolute_path");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_absolute_path(), target);
  }

  // repeated .com.android.fastdeploy.APKEntry entries = 2;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_entries_size()); i < n; i++) {
    const auto& repfield = this->_internal_entries(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(2, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:com.android.fastdeploy.APKMetaData)
  return target;
}

size_t APKMetaData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:com.android.fastdeploy.APKMetaData)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .com.android.fastdeploy.APKEntry entries = 2;
  total_size += 1UL * this->_internal_entries_size();
  for (const auto& msg : this->_impl_.entries_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // string absolute_path = 1;
  if (!this->_internal_absolute_path().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_absolute_path());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData APKMetaData::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    APKMetaData::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*APKMetaData::GetClassData() const { return &_class_data_; }


void APKMetaData::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<APKMetaData*>(&to_msg);
  auto& from = static_cast<const APKMetaData&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:com.android.fastdeploy.APKMetaData)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.entries_.MergeFrom(from._impl_.entries_);
  if (!from._internal_absolute_path().empty()) {
    _this->_internal_set_absolute_path(from._internal_absolute_path());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void APKMetaData::CopyFrom(const APKMetaData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:com.android.fastdeploy.APKMetaData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool APKMetaData::IsInitialized() const {
  return true;
}

void APKMetaData::InternalSwap(APKMetaData* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.entries_.InternalSwap(&other->_impl_.entries_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.absolute_path_, lhs_arena,
      &other->_impl_.absolute_path_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata APKMetaData::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_ApkEntry_2eproto_getter, &descriptor_table_ApkEntry_2eproto_once,
      file_level_metadata_ApkEntry_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace fastdeploy
}  // namespace android
}  // namespace com
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::com::android::fastdeploy::APKDump*
Arena::CreateMaybeMessage< ::com::android::fastdeploy::APKDump >(Arena* arena) {
  return Arena::CreateMessageInternal< ::com::android::fastdeploy::APKDump >(arena);
}
template<> PROTOBUF_NOINLINE ::com::android::fastdeploy::APKEntry*
Arena::CreateMaybeMessage< ::com::android::fastdeploy::APKEntry >(Arena* arena) {
  return Arena::CreateMessageInternal< ::com::android::fastdeploy::APKEntry >(arena);
}
template<> PROTOBUF_NOINLINE ::com::android::fastdeploy::APKMetaData*
Arena::CreateMaybeMessage< ::com::android::fastdeploy::APKMetaData >(Arena* arena) {
  return Arena::CreateMessageInternal< ::com::android::fastdeploy::APKMetaData >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
