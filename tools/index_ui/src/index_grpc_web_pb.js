/* eslint-disable */
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

const proto = require('./index_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.VisualIndexingServiceClient =
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
proto.VisualIndexingServicePromiseClient =
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
 *   !proto.IndexImageRequest,
 *   !proto.IndexImageResponse>}
 */
const methodDescriptor_VisualIndexingService_IndexImage = new grpc.web.MethodDescriptor(
  '/VisualIndexingService/IndexImage',
  grpc.web.MethodType.UNARY,
  proto.IndexImageRequest,
  proto.IndexImageResponse,
  /**
   * @param {!proto.IndexImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.IndexImageResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.IndexImageRequest,
 *   !proto.IndexImageResponse>}
 */
const methodInfo_VisualIndexingService_IndexImage = new grpc.web.AbstractClientBase.MethodInfo(
  proto.IndexImageResponse,
  /**
   * @param {!proto.IndexImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.IndexImageResponse.deserializeBinary
);


/**
 * @param {!proto.IndexImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.IndexImageResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.IndexImageResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.VisualIndexingServiceClient.prototype.indexImage =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/VisualIndexingService/IndexImage',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_IndexImage,
      callback);
};


/**
 * @param {!proto.IndexImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.IndexImageResponse>}
 *     A native promise that resolves to the response
 */
proto.VisualIndexingServicePromiseClient.prototype.indexImage =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/VisualIndexingService/IndexImage',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_IndexImage);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetBagOfWordsRequest,
 *   !proto.GetBagOfWordsResponse>}
 */
const methodDescriptor_VisualIndexingService_GetBagOfWords = new grpc.web.MethodDescriptor(
  '/VisualIndexingService/GetBagOfWords',
  grpc.web.MethodType.UNARY,
  proto.GetBagOfWordsRequest,
  proto.GetBagOfWordsResponse,
  /**
   * @param {!proto.GetBagOfWordsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetBagOfWordsResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetBagOfWordsRequest,
 *   !proto.GetBagOfWordsResponse>}
 */
const methodInfo_VisualIndexingService_GetBagOfWords = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetBagOfWordsResponse,
  /**
   * @param {!proto.GetBagOfWordsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetBagOfWordsResponse.deserializeBinary
);


/**
 * @param {!proto.GetBagOfWordsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.GetBagOfWordsResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetBagOfWordsResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.VisualIndexingServiceClient.prototype.getBagOfWords =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/VisualIndexingService/GetBagOfWords',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_GetBagOfWords,
      callback);
};


/**
 * @param {!proto.GetBagOfWordsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetBagOfWordsResponse>}
 *     A native promise that resolves to the response
 */
proto.VisualIndexingServicePromiseClient.prototype.getBagOfWords =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/VisualIndexingService/GetBagOfWords',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_GetBagOfWords);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.ClosestNRequest,
 *   !proto.ClosestNResponse>}
 */
const methodDescriptor_VisualIndexingService_ClosestN = new grpc.web.MethodDescriptor(
  '/VisualIndexingService/ClosestN',
  grpc.web.MethodType.UNARY,
  proto.ClosestNRequest,
  proto.ClosestNResponse,
  /**
   * @param {!proto.ClosestNRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ClosestNResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.ClosestNRequest,
 *   !proto.ClosestNResponse>}
 */
const methodInfo_VisualIndexingService_ClosestN = new grpc.web.AbstractClientBase.MethodInfo(
  proto.ClosestNResponse,
  /**
   * @param {!proto.ClosestNRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ClosestNResponse.deserializeBinary
);


/**
 * @param {!proto.ClosestNRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.ClosestNResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.ClosestNResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.VisualIndexingServiceClient.prototype.closestN =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/VisualIndexingService/ClosestN',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_ClosestN,
      callback);
};


/**
 * @param {!proto.ClosestNRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.ClosestNResponse>}
 *     A native promise that resolves to the response
 */
proto.VisualIndexingServicePromiseClient.prototype.closestN =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/VisualIndexingService/ClosestN',
      request,
      metadata || {},
      methodDescriptor_VisualIndexingService_ClosestN);
};


module.exports = proto;

