// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ApkEntry.proto

package com.android.fastdeploy;

public interface APKDumpOrBuilder extends
    // @@protoc_insertion_point(interface_extends:com.android.fastdeploy.APKDump)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>string name = 1;</code>
   * @return The name.
   */
  java.lang.String getName();
  /**
   * <code>string name = 1;</code>
   * @return The bytes for name.
   */
  com.google.protobuf.ByteString
      getNameBytes();

  /**
   * <code>bytes cd = 2;</code>
   * @return The cd.
   */
  com.google.protobuf.ByteString getCd();

  /**
   * <code>bytes signature = 3;</code>
   * @return The signature.
   */
  com.google.protobuf.ByteString getSignature();

  /**
   * <code>string absolute_path = 4;</code>
   * @return The absolutePath.
   */
  java.lang.String getAbsolutePath();
  /**
   * <code>string absolute_path = 4;</code>
   * @return The bytes for absolutePath.
   */
  com.google.protobuf.ByteString
      getAbsolutePathBytes();
}