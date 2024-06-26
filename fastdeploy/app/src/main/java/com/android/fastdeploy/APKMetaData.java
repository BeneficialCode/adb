// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ApkEntry.proto

package com.android.fastdeploy;

/**
 * Protobuf type {@code com.android.fastdeploy.APKMetaData}
 */
public final class APKMetaData extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:com.android.fastdeploy.APKMetaData)
    APKMetaDataOrBuilder {
private static final long serialVersionUID = 0L;
  // Use APKMetaData.newBuilder() to construct.
  private APKMetaData(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private APKMetaData() {
    absolutePath_ = "";
    entries_ = java.util.Collections.emptyList();
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new APKMetaData();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private APKMetaData(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
    int mutable_bitField0_ = 0;
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          case 10: {
            java.lang.String s = input.readStringRequireUtf8();

            absolutePath_ = s;
            break;
          }
          case 18: {
            if (!((mutable_bitField0_ & 0x00000001) != 0)) {
              entries_ = new java.util.ArrayList<com.android.fastdeploy.APKEntry>();
              mutable_bitField0_ |= 0x00000001;
            }
            entries_.add(
                input.readMessage(com.android.fastdeploy.APKEntry.parser(), extensionRegistry));
            break;
          }
          default: {
            if (!parseUnknownField(
                input, unknownFields, extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (com.google.protobuf.UninitializedMessageException e) {
      throw e.asInvalidProtocolBufferException().setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e).setUnfinishedMessage(this);
    } finally {
      if (((mutable_bitField0_ & 0x00000001) != 0)) {
        entries_ = java.util.Collections.unmodifiableList(entries_);
      }
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKMetaData_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKMetaData_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.android.fastdeploy.APKMetaData.class, com.android.fastdeploy.APKMetaData.Builder.class);
  }

  public static final int ABSOLUTE_PATH_FIELD_NUMBER = 1;
  private volatile java.lang.Object absolutePath_;
  /**
   * <code>string absolute_path = 1;</code>
   * @return The absolutePath.
   */
  @java.lang.Override
  public java.lang.String getAbsolutePath() {
    java.lang.Object ref = absolutePath_;
    if (ref instanceof java.lang.String) {
      return (java.lang.String) ref;
    } else {
      com.google.protobuf.ByteString bs = 
          (com.google.protobuf.ByteString) ref;
      java.lang.String s = bs.toStringUtf8();
      absolutePath_ = s;
      return s;
    }
  }
  /**
   * <code>string absolute_path = 1;</code>
   * @return The bytes for absolutePath.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString
      getAbsolutePathBytes() {
    java.lang.Object ref = absolutePath_;
    if (ref instanceof java.lang.String) {
      com.google.protobuf.ByteString b = 
          com.google.protobuf.ByteString.copyFromUtf8(
              (java.lang.String) ref);
      absolutePath_ = b;
      return b;
    } else {
      return (com.google.protobuf.ByteString) ref;
    }
  }

  public static final int ENTRIES_FIELD_NUMBER = 2;
  private java.util.List<com.android.fastdeploy.APKEntry> entries_;
  /**
   * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
   */
  @java.lang.Override
  public java.util.List<com.android.fastdeploy.APKEntry> getEntriesList() {
    return entries_;
  }
  /**
   * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
   */
  @java.lang.Override
  public java.util.List<? extends com.android.fastdeploy.APKEntryOrBuilder> 
      getEntriesOrBuilderList() {
    return entries_;
  }
  /**
   * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
   */
  @java.lang.Override
  public int getEntriesCount() {
    return entries_.size();
  }
  /**
   * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
   */
  @java.lang.Override
  public com.android.fastdeploy.APKEntry getEntries(int index) {
    return entries_.get(index);
  }
  /**
   * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
   */
  @java.lang.Override
  public com.android.fastdeploy.APKEntryOrBuilder getEntriesOrBuilder(
      int index) {
    return entries_.get(index);
  }

  private byte memoizedIsInitialized = -1;
  @java.lang.Override
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  @java.lang.Override
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(absolutePath_)) {
      com.google.protobuf.GeneratedMessageV3.writeString(output, 1, absolutePath_);
    }
    for (int i = 0; i < entries_.size(); i++) {
      output.writeMessage(2, entries_.get(i));
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(absolutePath_)) {
      size += com.google.protobuf.GeneratedMessageV3.computeStringSize(1, absolutePath_);
    }
    for (int i = 0; i < entries_.size(); i++) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(2, entries_.get(i));
    }
    size += unknownFields.getSerializedSize();
    memoizedSize = size;
    return size;
  }

  @java.lang.Override
  public boolean equals(final java.lang.Object obj) {
    if (obj == this) {
     return true;
    }
    if (!(obj instanceof com.android.fastdeploy.APKMetaData)) {
      return super.equals(obj);
    }
    com.android.fastdeploy.APKMetaData other = (com.android.fastdeploy.APKMetaData) obj;

    if (!getAbsolutePath()
        .equals(other.getAbsolutePath())) return false;
    if (!getEntriesList()
        .equals(other.getEntriesList())) return false;
    if (!unknownFields.equals(other.unknownFields)) return false;
    return true;
  }

  @java.lang.Override
  public int hashCode() {
    if (memoizedHashCode != 0) {
      return memoizedHashCode;
    }
    int hash = 41;
    hash = (19 * hash) + getDescriptor().hashCode();
    hash = (37 * hash) + ABSOLUTE_PATH_FIELD_NUMBER;
    hash = (53 * hash) + getAbsolutePath().hashCode();
    if (getEntriesCount() > 0) {
      hash = (37 * hash) + ENTRIES_FIELD_NUMBER;
      hash = (53 * hash) + getEntriesList().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.android.fastdeploy.APKMetaData parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.android.fastdeploy.APKMetaData parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKMetaData parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKMetaData parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }

  @java.lang.Override
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder() {
    return DEFAULT_INSTANCE.toBuilder();
  }
  public static Builder newBuilder(com.android.fastdeploy.APKMetaData prototype) {
    return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
  }
  @java.lang.Override
  public Builder toBuilder() {
    return this == DEFAULT_INSTANCE
        ? new Builder() : new Builder().mergeFrom(this);
  }

  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * Protobuf type {@code com.android.fastdeploy.APKMetaData}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:com.android.fastdeploy.APKMetaData)
      com.android.fastdeploy.APKMetaDataOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKMetaData_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKMetaData_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.android.fastdeploy.APKMetaData.class, com.android.fastdeploy.APKMetaData.Builder.class);
    }

    // Construct using com.android.fastdeploy.APKMetaData.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessageV3
              .alwaysUseFieldBuilders) {
        getEntriesFieldBuilder();
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      absolutePath_ = "";

      if (entriesBuilder_ == null) {
        entries_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
      } else {
        entriesBuilder_.clear();
      }
      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKMetaData_descriptor;
    }

    @java.lang.Override
    public com.android.fastdeploy.APKMetaData getDefaultInstanceForType() {
      return com.android.fastdeploy.APKMetaData.getDefaultInstance();
    }

    @java.lang.Override
    public com.android.fastdeploy.APKMetaData build() {
      com.android.fastdeploy.APKMetaData result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.android.fastdeploy.APKMetaData buildPartial() {
      com.android.fastdeploy.APKMetaData result = new com.android.fastdeploy.APKMetaData(this);
      int from_bitField0_ = bitField0_;
      result.absolutePath_ = absolutePath_;
      if (entriesBuilder_ == null) {
        if (((bitField0_ & 0x00000001) != 0)) {
          entries_ = java.util.Collections.unmodifiableList(entries_);
          bitField0_ = (bitField0_ & ~0x00000001);
        }
        result.entries_ = entries_;
      } else {
        result.entries_ = entriesBuilder_.build();
      }
      onBuilt();
      return result;
    }

    @java.lang.Override
    public Builder clone() {
      return super.clone();
    }
    @java.lang.Override
    public Builder setField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.setField(field, value);
    }
    @java.lang.Override
    public Builder clearField(
        com.google.protobuf.Descriptors.FieldDescriptor field) {
      return super.clearField(field);
    }
    @java.lang.Override
    public Builder clearOneof(
        com.google.protobuf.Descriptors.OneofDescriptor oneof) {
      return super.clearOneof(oneof);
    }
    @java.lang.Override
    public Builder setRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        int index, java.lang.Object value) {
      return super.setRepeatedField(field, index, value);
    }
    @java.lang.Override
    public Builder addRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.addRepeatedField(field, value);
    }
    @java.lang.Override
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof com.android.fastdeploy.APKMetaData) {
        return mergeFrom((com.android.fastdeploy.APKMetaData)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.android.fastdeploy.APKMetaData other) {
      if (other == com.android.fastdeploy.APKMetaData.getDefaultInstance()) return this;
      if (!other.getAbsolutePath().isEmpty()) {
        absolutePath_ = other.absolutePath_;
        onChanged();
      }
      if (entriesBuilder_ == null) {
        if (!other.entries_.isEmpty()) {
          if (entries_.isEmpty()) {
            entries_ = other.entries_;
            bitField0_ = (bitField0_ & ~0x00000001);
          } else {
            ensureEntriesIsMutable();
            entries_.addAll(other.entries_);
          }
          onChanged();
        }
      } else {
        if (!other.entries_.isEmpty()) {
          if (entriesBuilder_.isEmpty()) {
            entriesBuilder_.dispose();
            entriesBuilder_ = null;
            entries_ = other.entries_;
            bitField0_ = (bitField0_ & ~0x00000001);
            entriesBuilder_ = 
              com.google.protobuf.GeneratedMessageV3.alwaysUseFieldBuilders ?
                 getEntriesFieldBuilder() : null;
          } else {
            entriesBuilder_.addAllMessages(other.entries_);
          }
        }
      }
      this.mergeUnknownFields(other.unknownFields);
      onChanged();
      return this;
    }

    @java.lang.Override
    public final boolean isInitialized() {
      return true;
    }

    @java.lang.Override
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      com.android.fastdeploy.APKMetaData parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.android.fastdeploy.APKMetaData) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }
    private int bitField0_;

    private java.lang.Object absolutePath_ = "";
    /**
     * <code>string absolute_path = 1;</code>
     * @return The absolutePath.
     */
    public java.lang.String getAbsolutePath() {
      java.lang.Object ref = absolutePath_;
      if (!(ref instanceof java.lang.String)) {
        com.google.protobuf.ByteString bs =
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        absolutePath_ = s;
        return s;
      } else {
        return (java.lang.String) ref;
      }
    }
    /**
     * <code>string absolute_path = 1;</code>
     * @return The bytes for absolutePath.
     */
    public com.google.protobuf.ByteString
        getAbsolutePathBytes() {
      java.lang.Object ref = absolutePath_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        absolutePath_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    /**
     * <code>string absolute_path = 1;</code>
     * @param value The absolutePath to set.
     * @return This builder for chaining.
     */
    public Builder setAbsolutePath(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      absolutePath_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>string absolute_path = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearAbsolutePath() {
      
      absolutePath_ = getDefaultInstance().getAbsolutePath();
      onChanged();
      return this;
    }
    /**
     * <code>string absolute_path = 1;</code>
     * @param value The bytes for absolutePath to set.
     * @return This builder for chaining.
     */
    public Builder setAbsolutePathBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
      
      absolutePath_ = value;
      onChanged();
      return this;
    }

    private java.util.List<com.android.fastdeploy.APKEntry> entries_ =
      java.util.Collections.emptyList();
    private void ensureEntriesIsMutable() {
      if (!((bitField0_ & 0x00000001) != 0)) {
        entries_ = new java.util.ArrayList<com.android.fastdeploy.APKEntry>(entries_);
        bitField0_ |= 0x00000001;
       }
    }

    private com.google.protobuf.RepeatedFieldBuilderV3<
        com.android.fastdeploy.APKEntry, com.android.fastdeploy.APKEntry.Builder, com.android.fastdeploy.APKEntryOrBuilder> entriesBuilder_;

    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public java.util.List<com.android.fastdeploy.APKEntry> getEntriesList() {
      if (entriesBuilder_ == null) {
        return java.util.Collections.unmodifiableList(entries_);
      } else {
        return entriesBuilder_.getMessageList();
      }
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public int getEntriesCount() {
      if (entriesBuilder_ == null) {
        return entries_.size();
      } else {
        return entriesBuilder_.getCount();
      }
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public com.android.fastdeploy.APKEntry getEntries(int index) {
      if (entriesBuilder_ == null) {
        return entries_.get(index);
      } else {
        return entriesBuilder_.getMessage(index);
      }
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder setEntries(
        int index, com.android.fastdeploy.APKEntry value) {
      if (entriesBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureEntriesIsMutable();
        entries_.set(index, value);
        onChanged();
      } else {
        entriesBuilder_.setMessage(index, value);
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder setEntries(
        int index, com.android.fastdeploy.APKEntry.Builder builderForValue) {
      if (entriesBuilder_ == null) {
        ensureEntriesIsMutable();
        entries_.set(index, builderForValue.build());
        onChanged();
      } else {
        entriesBuilder_.setMessage(index, builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder addEntries(com.android.fastdeploy.APKEntry value) {
      if (entriesBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureEntriesIsMutable();
        entries_.add(value);
        onChanged();
      } else {
        entriesBuilder_.addMessage(value);
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder addEntries(
        int index, com.android.fastdeploy.APKEntry value) {
      if (entriesBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        ensureEntriesIsMutable();
        entries_.add(index, value);
        onChanged();
      } else {
        entriesBuilder_.addMessage(index, value);
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder addEntries(
        com.android.fastdeploy.APKEntry.Builder builderForValue) {
      if (entriesBuilder_ == null) {
        ensureEntriesIsMutable();
        entries_.add(builderForValue.build());
        onChanged();
      } else {
        entriesBuilder_.addMessage(builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder addEntries(
        int index, com.android.fastdeploy.APKEntry.Builder builderForValue) {
      if (entriesBuilder_ == null) {
        ensureEntriesIsMutable();
        entries_.add(index, builderForValue.build());
        onChanged();
      } else {
        entriesBuilder_.addMessage(index, builderForValue.build());
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder addAllEntries(
        java.lang.Iterable<? extends com.android.fastdeploy.APKEntry> values) {
      if (entriesBuilder_ == null) {
        ensureEntriesIsMutable();
        com.google.protobuf.AbstractMessageLite.Builder.addAll(
            values, entries_);
        onChanged();
      } else {
        entriesBuilder_.addAllMessages(values);
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder clearEntries() {
      if (entriesBuilder_ == null) {
        entries_ = java.util.Collections.emptyList();
        bitField0_ = (bitField0_ & ~0x00000001);
        onChanged();
      } else {
        entriesBuilder_.clear();
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public Builder removeEntries(int index) {
      if (entriesBuilder_ == null) {
        ensureEntriesIsMutable();
        entries_.remove(index);
        onChanged();
      } else {
        entriesBuilder_.remove(index);
      }
      return this;
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public com.android.fastdeploy.APKEntry.Builder getEntriesBuilder(
        int index) {
      return getEntriesFieldBuilder().getBuilder(index);
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public com.android.fastdeploy.APKEntryOrBuilder getEntriesOrBuilder(
        int index) {
      if (entriesBuilder_ == null) {
        return entries_.get(index);  } else {
        return entriesBuilder_.getMessageOrBuilder(index);
      }
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public java.util.List<? extends com.android.fastdeploy.APKEntryOrBuilder> 
         getEntriesOrBuilderList() {
      if (entriesBuilder_ != null) {
        return entriesBuilder_.getMessageOrBuilderList();
      } else {
        return java.util.Collections.unmodifiableList(entries_);
      }
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public com.android.fastdeploy.APKEntry.Builder addEntriesBuilder() {
      return getEntriesFieldBuilder().addBuilder(
          com.android.fastdeploy.APKEntry.getDefaultInstance());
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public com.android.fastdeploy.APKEntry.Builder addEntriesBuilder(
        int index) {
      return getEntriesFieldBuilder().addBuilder(
          index, com.android.fastdeploy.APKEntry.getDefaultInstance());
    }
    /**
     * <code>repeated .com.android.fastdeploy.APKEntry entries = 2;</code>
     */
    public java.util.List<com.android.fastdeploy.APKEntry.Builder> 
         getEntriesBuilderList() {
      return getEntriesFieldBuilder().getBuilderList();
    }
    private com.google.protobuf.RepeatedFieldBuilderV3<
        com.android.fastdeploy.APKEntry, com.android.fastdeploy.APKEntry.Builder, com.android.fastdeploy.APKEntryOrBuilder> 
        getEntriesFieldBuilder() {
      if (entriesBuilder_ == null) {
        entriesBuilder_ = new com.google.protobuf.RepeatedFieldBuilderV3<
            com.android.fastdeploy.APKEntry, com.android.fastdeploy.APKEntry.Builder, com.android.fastdeploy.APKEntryOrBuilder>(
                entries_,
                ((bitField0_ & 0x00000001) != 0),
                getParentForChildren(),
                isClean());
        entries_ = null;
      }
      return entriesBuilder_;
    }
    @java.lang.Override
    public final Builder setUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.setUnknownFields(unknownFields);
    }

    @java.lang.Override
    public final Builder mergeUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.mergeUnknownFields(unknownFields);
    }


    // @@protoc_insertion_point(builder_scope:com.android.fastdeploy.APKMetaData)
  }

  // @@protoc_insertion_point(class_scope:com.android.fastdeploy.APKMetaData)
  private static final com.android.fastdeploy.APKMetaData DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.android.fastdeploy.APKMetaData();
  }

  public static com.android.fastdeploy.APKMetaData getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<APKMetaData>
      PARSER = new com.google.protobuf.AbstractParser<APKMetaData>() {
    @java.lang.Override
    public APKMetaData parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new APKMetaData(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<APKMetaData> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<APKMetaData> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.android.fastdeploy.APKMetaData getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

