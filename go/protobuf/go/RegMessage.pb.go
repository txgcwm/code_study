// Code generated by protoc-gen-go.
// source: RegMessage.proto
// DO NOT EDIT!

/*
Package cn_vicky_model_seri is a generated protocol buffer package.

It is generated from these files:
	RegMessage.proto

It has these top-level messages:
	RegMessage
*/
package main

import proto "proto"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = math.Inf

type RegMessage struct {
	Id               *int32  `protobuf:"varint,1,req,name=id" json:"id,omitempty"`
	Username         *string `protobuf:"bytes,2,req,name=username" json:"username,omitempty"`
	Password         *string `protobuf:"bytes,3,req,name=password" json:"password,omitempty"`
	Email            *string `protobuf:"bytes,4,opt,name=email" json:"email,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *RegMessage) Reset()         { *m = RegMessage{} }
func (m *RegMessage) String() string { return proto.CompactTextString(m) }
func (*RegMessage) ProtoMessage()    {}

func (m *RegMessage) GetId() int32 {
	if m != nil && m.Id != nil {
		return *m.Id
	}
	return 0
}

func (m *RegMessage) GetUsername() string {
	if m != nil && m.Username != nil {
		return *m.Username
	}
	return ""
}

func (m *RegMessage) GetPassword() string {
	if m != nil && m.Password != nil {
		return *m.Password
	}
	return ""
}

func (m *RegMessage) GetEmail() string {
	if m != nil && m.Email != nil {
		return *m.Email
	}
	return ""
}

func init() {
}
