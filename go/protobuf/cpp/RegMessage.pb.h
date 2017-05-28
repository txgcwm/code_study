// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: RegMessage.proto

#ifndef PROTOBUF_RegMessage_2eproto__INCLUDED
#define PROTOBUF_RegMessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace cn {
namespace vicky {
namespace model {
namespace seri {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_RegMessage_2eproto();
void protobuf_AssignDesc_RegMessage_2eproto();
void protobuf_ShutdownFile_RegMessage_2eproto();

class RegMessage;

// ===================================================================

class RegMessage : public ::google::protobuf::Message {
 public:
  RegMessage();
  virtual ~RegMessage();

  RegMessage(const RegMessage& from);

  inline RegMessage& operator=(const RegMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RegMessage& default_instance();

  void Swap(RegMessage* other);

  // implements Message ----------------------------------------------

  RegMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RegMessage& from);
  void MergeFrom(const RegMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // required string username = 2;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 2;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  inline ::std::string* release_username();
  inline void set_allocated_username(::std::string* username);

  // required string password = 3;
  inline bool has_password() const;
  inline void clear_password();
  static const int kPasswordFieldNumber = 3;
  inline const ::std::string& password() const;
  inline void set_password(const ::std::string& value);
  inline void set_password(const char* value);
  inline void set_password(const char* value, size_t size);
  inline ::std::string* mutable_password();
  inline ::std::string* release_password();
  inline void set_allocated_password(::std::string* password);

  // optional string email = 4;
  inline bool has_email() const;
  inline void clear_email();
  static const int kEmailFieldNumber = 4;
  inline const ::std::string& email() const;
  inline void set_email(const ::std::string& value);
  inline void set_email(const char* value);
  inline void set_email(const char* value, size_t size);
  inline ::std::string* mutable_email();
  inline ::std::string* release_email();
  inline void set_allocated_email(::std::string* email);

  // @@protoc_insertion_point(class_scope:cn.vicky.model.seri.RegMessage)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_username();
  inline void clear_has_username();
  inline void set_has_password();
  inline void clear_has_password();
  inline void set_has_email();
  inline void clear_has_email();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* username_;
  ::std::string* password_;
  ::std::string* email_;
  ::google::protobuf::int32 id_;
  friend void  protobuf_AddDesc_RegMessage_2eproto();
  friend void protobuf_AssignDesc_RegMessage_2eproto();
  friend void protobuf_ShutdownFile_RegMessage_2eproto();

  void InitAsDefaultInstance();
  static RegMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// RegMessage

// required int32 id = 1;
inline bool RegMessage::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RegMessage::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RegMessage::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RegMessage::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 RegMessage::id() const {
  // @@protoc_insertion_point(field_get:cn.vicky.model.seri.RegMessage.id)
  return id_;
}
inline void RegMessage::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:cn.vicky.model.seri.RegMessage.id)
}

// required string username = 2;
inline bool RegMessage::has_username() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RegMessage::set_has_username() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RegMessage::clear_has_username() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RegMessage::clear_username() {
  if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_->clear();
  }
  clear_has_username();
}
inline const ::std::string& RegMessage::username() const {
  // @@protoc_insertion_point(field_get:cn.vicky.model.seri.RegMessage.username)
  return *username_;
}
inline void RegMessage::set_username(const ::std::string& value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(value);
  // @@protoc_insertion_point(field_set:cn.vicky.model.seri.RegMessage.username)
}
inline void RegMessage::set_username(const char* value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(value);
  // @@protoc_insertion_point(field_set_char:cn.vicky.model.seri.RegMessage.username)
}
inline void RegMessage::set_username(const char* value, size_t size) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:cn.vicky.model.seri.RegMessage.username)
}
inline ::std::string* RegMessage::mutable_username() {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:cn.vicky.model.seri.RegMessage.username)
  return username_;
}
inline ::std::string* RegMessage::release_username() {
  clear_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = username_;
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RegMessage::set_allocated_username(::std::string* username) {
  if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete username_;
  }
  if (username) {
    set_has_username();
    username_ = username;
  } else {
    clear_has_username();
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:cn.vicky.model.seri.RegMessage.username)
}

// required string password = 3;
inline bool RegMessage::has_password() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RegMessage::set_has_password() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RegMessage::clear_has_password() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RegMessage::clear_password() {
  if (password_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    password_->clear();
  }
  clear_has_password();
}
inline const ::std::string& RegMessage::password() const {
  // @@protoc_insertion_point(field_get:cn.vicky.model.seri.RegMessage.password)
  return *password_;
}
inline void RegMessage::set_password(const ::std::string& value) {
  set_has_password();
  if (password_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    password_ = new ::std::string;
  }
  password_->assign(value);
  // @@protoc_insertion_point(field_set:cn.vicky.model.seri.RegMessage.password)
}
inline void RegMessage::set_password(const char* value) {
  set_has_password();
  if (password_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    password_ = new ::std::string;
  }
  password_->assign(value);
  // @@protoc_insertion_point(field_set_char:cn.vicky.model.seri.RegMessage.password)
}
inline void RegMessage::set_password(const char* value, size_t size) {
  set_has_password();
  if (password_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    password_ = new ::std::string;
  }
  password_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:cn.vicky.model.seri.RegMessage.password)
}
inline ::std::string* RegMessage::mutable_password() {
  set_has_password();
  if (password_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    password_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:cn.vicky.model.seri.RegMessage.password)
  return password_;
}
inline ::std::string* RegMessage::release_password() {
  clear_has_password();
  if (password_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = password_;
    password_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RegMessage::set_allocated_password(::std::string* password) {
  if (password_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete password_;
  }
  if (password) {
    set_has_password();
    password_ = password;
  } else {
    clear_has_password();
    password_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:cn.vicky.model.seri.RegMessage.password)
}

// optional string email = 4;
inline bool RegMessage::has_email() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void RegMessage::set_has_email() {
  _has_bits_[0] |= 0x00000008u;
}
inline void RegMessage::clear_has_email() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void RegMessage::clear_email() {
  if (email_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    email_->clear();
  }
  clear_has_email();
}
inline const ::std::string& RegMessage::email() const {
  // @@protoc_insertion_point(field_get:cn.vicky.model.seri.RegMessage.email)
  return *email_;
}
inline void RegMessage::set_email(const ::std::string& value) {
  set_has_email();
  if (email_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    email_ = new ::std::string;
  }
  email_->assign(value);
  // @@protoc_insertion_point(field_set:cn.vicky.model.seri.RegMessage.email)
}
inline void RegMessage::set_email(const char* value) {
  set_has_email();
  if (email_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    email_ = new ::std::string;
  }
  email_->assign(value);
  // @@protoc_insertion_point(field_set_char:cn.vicky.model.seri.RegMessage.email)
}
inline void RegMessage::set_email(const char* value, size_t size) {
  set_has_email();
  if (email_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    email_ = new ::std::string;
  }
  email_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:cn.vicky.model.seri.RegMessage.email)
}
inline ::std::string* RegMessage::mutable_email() {
  set_has_email();
  if (email_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    email_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:cn.vicky.model.seri.RegMessage.email)
  return email_;
}
inline ::std::string* RegMessage::release_email() {
  clear_has_email();
  if (email_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = email_;
    email_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RegMessage::set_allocated_email(::std::string* email) {
  if (email_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete email_;
  }
  if (email) {
    set_has_email();
    email_ = email;
  } else {
    clear_has_email();
    email_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:cn.vicky.model.seri.RegMessage.email)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace seri
}  // namespace model
}  // namespace vicky
}  // namespace cn

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_RegMessage_2eproto__INCLUDED