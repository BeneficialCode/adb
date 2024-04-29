// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ApkEntry.proto

package com.android.fastdeploy;

/**
 * Protobuf type {@code com.android.fastdeploy.APKEntry}
 */
public final class APKEntry extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:com.android.fastdeploy.APKEntry)
    APKEntryOrBuilder {
private static final long serialVersionUID = 0L;
  // Use APKEntry.newBuilder() to construct.
  private APKEntry(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private APKEntry() {
    md5_ = com.google.protobuf.ByteString.EMPTY;
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new APKEntry();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private APKEntry(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
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

            md5_ = input.readBytes();
            break;
          }
          case 16: {

            dataOffset_ = input.readInt64();
            break;
          }
          case 24: {

            dataSize_ = input.readInt64();
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
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKEntry_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKEntry_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            com.android.fastdeploy.APKEntry.class, com.android.fastdeploy.APKEntry.Builder.class);
  }

  public static final int MD5_FIELD_NUMBER = 1;
  private com.google.protobuf.ByteString md5_;
  /**
   * <code>bytes md5 = 1;</code>
   * @return The md5.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString getMd5() {
    return md5_;
  }

  public static final int DATAOFFSET_FIELD_NUMBER = 2;
  private long dataOffset_;
  /**
   * <code>int64 dataOffset = 2;</code>
   * @return The dataOffset.
   */
  @java.lang.Override
  public long getDataOffset() {
    return dataOffset_;
  }

  public static final int DATASIZE_FIELD_NUMBER = 3;
  private long dataSize_;
  /**
   * <code>int64 dataSize = 3;</code>
   * @return The dataSize.
   */
  @java.lang.Override
  public long getDataSize() {
    return dataSize_;
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
    if (!md5_.isEmpty()) {
      output.writeBytes(1, md5_);
    }
    if (dataOffset_ != 0L) {
      output.writeInt64(2, dataOffset_);
    }
    if (dataSize_ != 0L) {
      output.writeInt64(3, dataSize_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (!md5_.isEmpty()) {
      size += com.google.protobuf.CodedOutputStream
        .computeBytesSize(1, md5_);
    }
    if (dataOffset_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(2, dataOffset_);
    }
    if (dataSize_ != 0L) {
      size += com.google.protobuf.CodedOutputStream
        .computeInt64Size(3, dataSize_);
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
    if (!(obj instanceof com.android.fastdeploy.APKEntry)) {
      return super.equals(obj);
    }
    com.android.fastdeploy.APKEntry other = (com.android.fastdeploy.APKEntry) obj;

    if (!getMd5()
        .equals(other.getMd5())) return false;
    if (getDataOffset()
        != other.getDataOffset()) return false;
    if (getDataSize()
        != other.getDataSize()) return false;
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
    hash = (37 * hash) + MD5_FIELD_NUMBER;
    hash = (53 * hash) + getMd5().hashCode();
    hash = (37 * hash) + DATAOFFSET_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getDataOffset());
    hash = (37 * hash) + DATASIZE_FIELD_NUMBER;
    hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
        getDataSize());
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static com.android.fastdeploy.APKEntry parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.android.fastdeploy.APKEntry parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKEntry parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static com.android.fastdeploy.APKEntry parseFrom(
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
  public static Builder newBuilder(com.android.fastdeploy.APKEntry prototype) {
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
   * Protobuf type {@code com.android.fastdeploy.APKEntry}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:com.android.fastdeploy.APKEntry)
      com.android.fastdeploy.APKEntryOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKEntry_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKEntry_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.android.fastdeploy.APKEntry.class, com.android.fastdeploy.APKEntry.Builder.class);
    }

    // Construct using com.android.fastdeploy.APKEntry.newBuilder()
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
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      md5_ = com.google.protobuf.ByteString.EMPTY;

      dataOffset_ = 0L;

      dataSize_ = 0L;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return com.android.fastdeploy.ApkEntryProto.internal_static_com_android_fastdeploy_APKEntry_descriptor;
    }

    @java.lang.Override
    public com.android.fastdeploy.APKEntry getDefaultInstanceForType() {
      return com.android.fastdeploy.APKEntry.getDefaultInstance();
    }

    @java.lang.Override
    public com.android.fastdeploy.APKEntry build() {
      com.android.fastdeploy.APKEntry result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public com.android.fastdeploy.APKEntry buildPartial() {
      com.android.fastdeploy.APKEntry result = new com.android.fastdeploy.APKEntry(this);
      result.md5_ = md5_;
      result.dataOffset_ = dataOffset_;
      result.dataSize_ = dataSize_;
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
      if (other instanceof com.android.fastdeploy.APKEntry) {
        return mergeFrom((com.android.fastdeploy.APKEntry)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(com.android.fastdeploy.APKEntry other) {
      if (other == com.android.fastdeploy.APKEntry.getDefaultInstance()) return this;
      if (other.getMd5() != com.google.protobuf.ByteString.EMPTY) {
        setMd5(other.getMd5());
      }
      if (other.getDataOffset() != 0L) {
        setDataOffset(other.getDataOffset());
      }
      if (other.getDataSize() != 0L) {
        setDataSize(other.getDataSize());
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
      com.android.fastdeploy.APKEntry parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (com.android.fastdeploy.APKEntry) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private com.google.protobuf.ByteString md5_ = com.google.protobuf.ByteString.EMPTY;
    /**
     * <code>bytes md5 = 1;</code>
     * @return The md5.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString getMd5() {
      return md5_;
    }
    /**
     * <code>bytes md5 = 1;</code>
     * @param value The md5 to set.
     * @return This builder for chaining.
     */
    public Builder setMd5(com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      md5_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>bytes md5 = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearMd5() {
      
      md5_ = getDefaultInstance().getMd5();
      onChanged();
      return this;
    }

    private long dataOffset_ ;
    /**
     * <code>int64 dataOffset = 2;</code>
     * @return The dataOffset.
     */
    @java.lang.Override
    public long getDataOffset() {
      return dataOffset_;
    }
    /**
     * <code>int64 dataOffset = 2;</code>
     * @param value The dataOffset to set.
     * @return This builder for chaining.
     */
    public Builder setDataOffset(long value) {
      
      dataOffset_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 dataOffset = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearDataOffset() {
      
      dataOffset_ = 0L;
      onChanged();
      return this;
    }

    private long dataSize_ ;
    /**
     * <code>int64 dataSize = 3;</code>
     * @return The dataSize.
     */
    @java.lang.Override
    public long getDataSize() {
      return dataSize_;
    }
    /**
     * <code>int64 dataSize = 3;</code>
     * @param value The dataSize to set.
     * @return This builder for chaining.
     */
    public Builder setDataSize(long value) {
      
      dataSize_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>int64 dataSize = 3;</code>
     * @return This builder for chaining.
     */
    public Builder clearDataSize() {
      
      dataSize_ = 0L;
      onChanged();
      return this;
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


    // @@protoc_insertion_point(builder_scope:com.android.fastdeploy.APKEntry)
  }

  // @@protoc_insertion_point(class_scope:com.android.fastdeploy.APKEntry)
  private static final com.android.fastdeploy.APKEntry DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new com.android.fastdeploy.APKEntry();
  }

  public static com.android.fastdeploy.APKEntry getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<APKEntry>
      PARSER = new com.google.protobuf.AbstractParser<APKEntry>() {
    @java.lang.Override
    public APKEntry parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new APKEntry(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<APKEntry> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<APKEntry> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public com.android.fastdeploy.APKEntry getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

