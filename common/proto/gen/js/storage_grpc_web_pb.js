/**
 * @fileoverview gRPC-Web generated client stub for 
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');

const proto = require('./storage_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.StorageClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.StoragePromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetItemRequest,
 *   !proto.GetItemResponse>}
 */
const methodDescriptor_Storage_Get = new grpc.web.MethodDescriptor(
  '/Storage/Get',
  grpc.web.MethodType.SERVER_STREAMING,
  proto.GetItemRequest,
  proto.GetItemResponse,
  /**
   * @param {!proto.GetItemRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetItemResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetItemRequest,
 *   !proto.GetItemResponse>}
 */
const methodInfo_Storage_Get = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetItemResponse,
  /**
   * @param {!proto.GetItemRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetItemResponse.deserializeBinary
);


/**
 * @param {!proto.GetItemRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetItemResponse>}
 *     The XHR Node Readable Stream
 */
proto.StorageClient.prototype.get =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/Storage/Get',
      request,
      metadata || {},
      methodDescriptor_Storage_Get);
};


/**
 * @param {!proto.GetItemRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetItemResponse>}
 *     The XHR Node Readable Stream
 */
proto.StoragePromiseClient.prototype.get =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/Storage/Get',
      request,
      metadata || {},
      methodDescriptor_Storage_Get);
};


module.exports = proto;

