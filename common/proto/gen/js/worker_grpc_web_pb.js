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

const proto = require('./worker_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.WorkerPoolManagerClient =
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
proto.WorkerPoolManagerPromiseClient =
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
 *   !proto.RegisterWorkerRequest,
 *   !proto.RegisterWorkerResponse>}
 */
const methodDescriptor_WorkerPoolManager_Register = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/Register',
  grpc.web.MethodType.UNARY,
  proto.RegisterWorkerRequest,
  proto.RegisterWorkerResponse,
  /**
   * @param {!proto.RegisterWorkerRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.RegisterWorkerResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.RegisterWorkerRequest,
 *   !proto.RegisterWorkerResponse>}
 */
const methodInfo_WorkerPoolManager_Register = new grpc.web.AbstractClientBase.MethodInfo(
  proto.RegisterWorkerResponse,
  /**
   * @param {!proto.RegisterWorkerRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.RegisterWorkerResponse.deserializeBinary
);


/**
 * @param {!proto.RegisterWorkerRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.RegisterWorkerResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.RegisterWorkerResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.register =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/Register',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_Register,
      callback);
};


/**
 * @param {!proto.RegisterWorkerRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.RegisterWorkerResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.register =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/Register',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_Register);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerAddImageRequest,
 *   !proto.WorkerAddImageResponse>}
 */
const methodDescriptor_WorkerPoolManager_AddImage = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/AddImage',
  grpc.web.MethodType.UNARY,
  proto.WorkerAddImageRequest,
  proto.WorkerAddImageResponse,
  /**
   * @param {!proto.WorkerAddImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerAddImageResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerAddImageRequest,
 *   !proto.WorkerAddImageResponse>}
 */
const methodInfo_WorkerPoolManager_AddImage = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerAddImageResponse,
  /**
   * @param {!proto.WorkerAddImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerAddImageResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerAddImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerAddImageResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerAddImageResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.addImage =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/AddImage',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_AddImage,
      callback);
};


/**
 * @param {!proto.WorkerAddImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerAddImageResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.addImage =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/AddImage',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_AddImage);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeFeaturesRequest,
 *   !proto.WorkerComputeFeaturesResponse>}
 */
const methodDescriptor_WorkerPoolManager_ComputeFeatures = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/ComputeFeatures',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeFeaturesRequest,
  proto.WorkerComputeFeaturesResponse,
  /**
   * @param {!proto.WorkerComputeFeaturesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeFeaturesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeFeaturesRequest,
 *   !proto.WorkerComputeFeaturesResponse>}
 */
const methodInfo_WorkerPoolManager_ComputeFeatures = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeFeaturesResponse,
  /**
   * @param {!proto.WorkerComputeFeaturesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeFeaturesResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeFeaturesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeFeaturesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeFeaturesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.computeFeatures =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/ComputeFeatures',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeFeatures,
      callback);
};


/**
 * @param {!proto.WorkerComputeFeaturesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeFeaturesResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.computeFeatures =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/ComputeFeatures',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeFeatures);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeMatchesRequest,
 *   !proto.WorkerComputeMatchesResponse>}
 */
const methodDescriptor_WorkerPoolManager_ComputeMatches = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/ComputeMatches',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeMatchesRequest,
  proto.WorkerComputeMatchesResponse,
  /**
   * @param {!proto.WorkerComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeMatchesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeMatchesRequest,
 *   !proto.WorkerComputeMatchesResponse>}
 */
const methodInfo_WorkerPoolManager_ComputeMatches = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeMatchesResponse,
  /**
   * @param {!proto.WorkerComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeMatchesResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeMatchesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeMatchesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.computeMatches =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeMatches,
      callback);
};


/**
 * @param {!proto.WorkerComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeMatchesResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.computeMatches =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeMatches);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerIncrementalSFMRequest,
 *   !proto.WorkerIncrementalSFMResponse>}
 */
const methodDescriptor_WorkerPoolManager_IncrementalSFM = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/IncrementalSFM',
  grpc.web.MethodType.UNARY,
  proto.WorkerIncrementalSFMRequest,
  proto.WorkerIncrementalSFMResponse,
  /**
   * @param {!proto.WorkerIncrementalSFMRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerIncrementalSFMResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerIncrementalSFMRequest,
 *   !proto.WorkerIncrementalSFMResponse>}
 */
const methodInfo_WorkerPoolManager_IncrementalSFM = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerIncrementalSFMResponse,
  /**
   * @param {!proto.WorkerIncrementalSFMRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerIncrementalSFMResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerIncrementalSFMRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerIncrementalSFMResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerIncrementalSFMResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.incrementalSFM =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/IncrementalSFM',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_IncrementalSFM,
      callback);
};


/**
 * @param {!proto.WorkerIncrementalSFMRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerIncrementalSFMResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.incrementalSFM =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/IncrementalSFM',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_IncrementalSFM);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeStructureRequest,
 *   !proto.WorkerComputeStructureResponse>}
 */
const methodDescriptor_WorkerPoolManager_ComputeStructure = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/ComputeStructure',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeStructureRequest,
  proto.WorkerComputeStructureResponse,
  /**
   * @param {!proto.WorkerComputeStructureRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeStructureResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeStructureRequest,
 *   !proto.WorkerComputeStructureResponse>}
 */
const methodInfo_WorkerPoolManager_ComputeStructure = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeStructureResponse,
  /**
   * @param {!proto.WorkerComputeStructureRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeStructureResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeStructureRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeStructureResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeStructureResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.computeStructure =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/ComputeStructure',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeStructure,
      callback);
};


/**
 * @param {!proto.WorkerComputeStructureRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeStructureResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.computeStructure =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/ComputeStructure',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_ComputeStructure);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerMVSRequest,
 *   !proto.WorkerMVSResponse>}
 */
const methodDescriptor_WorkerPoolManager_MVS = new grpc.web.MethodDescriptor(
  '/WorkerPoolManager/MVS',
  grpc.web.MethodType.UNARY,
  proto.WorkerMVSRequest,
  proto.WorkerMVSResponse,
  /**
   * @param {!proto.WorkerMVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerMVSResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerMVSRequest,
 *   !proto.WorkerMVSResponse>}
 */
const methodInfo_WorkerPoolManager_MVS = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerMVSResponse,
  /**
   * @param {!proto.WorkerMVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerMVSResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerMVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerMVSResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerMVSResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerPoolManagerClient.prototype.mVS =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/WorkerPoolManager/MVS',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_MVS,
      callback);
};


/**
 * @param {!proto.WorkerMVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerMVSResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPoolManagerPromiseClient.prototype.mVS =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/WorkerPoolManager/MVS',
      request,
      metadata || {},
      methodDescriptor_WorkerPoolManager_MVS);
};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.WorkerClient =
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
proto.WorkerPromiseClient =
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
 *   !proto.WorkerAddImageRequest,
 *   !proto.WorkerAddImageResponse>}
 */
const methodDescriptor_Worker_AddImage = new grpc.web.MethodDescriptor(
  '/Worker/AddImage',
  grpc.web.MethodType.UNARY,
  proto.WorkerAddImageRequest,
  proto.WorkerAddImageResponse,
  /**
   * @param {!proto.WorkerAddImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerAddImageResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerAddImageRequest,
 *   !proto.WorkerAddImageResponse>}
 */
const methodInfo_Worker_AddImage = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerAddImageResponse,
  /**
   * @param {!proto.WorkerAddImageRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerAddImageResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerAddImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerAddImageResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerAddImageResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.addImage =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/AddImage',
      request,
      metadata || {},
      methodDescriptor_Worker_AddImage,
      callback);
};


/**
 * @param {!proto.WorkerAddImageRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerAddImageResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.addImage =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/AddImage',
      request,
      metadata || {},
      methodDescriptor_Worker_AddImage);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeFeaturesRequest,
 *   !proto.WorkerComputeFeaturesResponse>}
 */
const methodDescriptor_Worker_ComputeFeatures = new grpc.web.MethodDescriptor(
  '/Worker/ComputeFeatures',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeFeaturesRequest,
  proto.WorkerComputeFeaturesResponse,
  /**
   * @param {!proto.WorkerComputeFeaturesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeFeaturesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeFeaturesRequest,
 *   !proto.WorkerComputeFeaturesResponse>}
 */
const methodInfo_Worker_ComputeFeatures = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeFeaturesResponse,
  /**
   * @param {!proto.WorkerComputeFeaturesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeFeaturesResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeFeaturesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeFeaturesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeFeaturesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.computeFeatures =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/ComputeFeatures',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeFeatures,
      callback);
};


/**
 * @param {!proto.WorkerComputeFeaturesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeFeaturesResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.computeFeatures =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/ComputeFeatures',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeFeatures);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeMatchesRequest,
 *   !proto.WorkerComputeMatchesResponse>}
 */
const methodDescriptor_Worker_ComputeMatches = new grpc.web.MethodDescriptor(
  '/Worker/ComputeMatches',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeMatchesRequest,
  proto.WorkerComputeMatchesResponse,
  /**
   * @param {!proto.WorkerComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeMatchesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeMatchesRequest,
 *   !proto.WorkerComputeMatchesResponse>}
 */
const methodInfo_Worker_ComputeMatches = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeMatchesResponse,
  /**
   * @param {!proto.WorkerComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeMatchesResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeMatchesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeMatchesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.computeMatches =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeMatches,
      callback);
};


/**
 * @param {!proto.WorkerComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeMatchesResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.computeMatches =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeMatches);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerIncrementalSFMRequest,
 *   !proto.WorkerIncrementalSFMResponse>}
 */
const methodDescriptor_Worker_IncrementalSFM = new grpc.web.MethodDescriptor(
  '/Worker/IncrementalSFM',
  grpc.web.MethodType.UNARY,
  proto.WorkerIncrementalSFMRequest,
  proto.WorkerIncrementalSFMResponse,
  /**
   * @param {!proto.WorkerIncrementalSFMRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerIncrementalSFMResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerIncrementalSFMRequest,
 *   !proto.WorkerIncrementalSFMResponse>}
 */
const methodInfo_Worker_IncrementalSFM = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerIncrementalSFMResponse,
  /**
   * @param {!proto.WorkerIncrementalSFMRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerIncrementalSFMResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerIncrementalSFMRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerIncrementalSFMResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerIncrementalSFMResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.incrementalSFM =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/IncrementalSFM',
      request,
      metadata || {},
      methodDescriptor_Worker_IncrementalSFM,
      callback);
};


/**
 * @param {!proto.WorkerIncrementalSFMRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerIncrementalSFMResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.incrementalSFM =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/IncrementalSFM',
      request,
      metadata || {},
      methodDescriptor_Worker_IncrementalSFM);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerComputeStructureRequest,
 *   !proto.WorkerComputeStructureResponse>}
 */
const methodDescriptor_Worker_ComputeStructure = new grpc.web.MethodDescriptor(
  '/Worker/ComputeStructure',
  grpc.web.MethodType.UNARY,
  proto.WorkerComputeStructureRequest,
  proto.WorkerComputeStructureResponse,
  /**
   * @param {!proto.WorkerComputeStructureRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeStructureResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerComputeStructureRequest,
 *   !proto.WorkerComputeStructureResponse>}
 */
const methodInfo_Worker_ComputeStructure = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerComputeStructureResponse,
  /**
   * @param {!proto.WorkerComputeStructureRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerComputeStructureResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerComputeStructureRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerComputeStructureResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerComputeStructureResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.computeStructure =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/ComputeStructure',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeStructure,
      callback);
};


/**
 * @param {!proto.WorkerComputeStructureRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerComputeStructureResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.computeStructure =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/ComputeStructure',
      request,
      metadata || {},
      methodDescriptor_Worker_ComputeStructure);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.WorkerMVSRequest,
 *   !proto.WorkerMVSResponse>}
 */
const methodDescriptor_Worker_MVS = new grpc.web.MethodDescriptor(
  '/Worker/MVS',
  grpc.web.MethodType.UNARY,
  proto.WorkerMVSRequest,
  proto.WorkerMVSResponse,
  /**
   * @param {!proto.WorkerMVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerMVSResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.WorkerMVSRequest,
 *   !proto.WorkerMVSResponse>}
 */
const methodInfo_Worker_MVS = new grpc.web.AbstractClientBase.MethodInfo(
  proto.WorkerMVSResponse,
  /**
   * @param {!proto.WorkerMVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.WorkerMVSResponse.deserializeBinary
);


/**
 * @param {!proto.WorkerMVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.WorkerMVSResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.WorkerMVSResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.WorkerClient.prototype.mVS =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Worker/MVS',
      request,
      metadata || {},
      methodDescriptor_Worker_MVS,
      callback);
};


/**
 * @param {!proto.WorkerMVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.WorkerMVSResponse>}
 *     A native promise that resolves to the response
 */
proto.WorkerPromiseClient.prototype.mVS =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Worker/MVS',
      request,
      metadata || {},
      methodDescriptor_Worker_MVS);
};


module.exports = proto;

