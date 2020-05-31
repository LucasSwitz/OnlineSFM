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

const proto = require('./server_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.ReconstructionServiceClient =
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
proto.ReconstructionServicePromiseClient =
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
 *   !proto.HandhsakeRequest,
 *   !proto.HandshakeResponse>}
 */
const methodDescriptor_ReconstructionService_Handshake = new grpc.web.MethodDescriptor(
  '/ReconstructionService/Handshake',
  grpc.web.MethodType.UNARY,
  proto.HandhsakeRequest,
  proto.HandshakeResponse,
  /**
   * @param {!proto.HandhsakeRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.HandshakeResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.HandhsakeRequest,
 *   !proto.HandshakeResponse>}
 */
const methodInfo_ReconstructionService_Handshake = new grpc.web.AbstractClientBase.MethodInfo(
  proto.HandshakeResponse,
  /**
   * @param {!proto.HandhsakeRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.HandshakeResponse.deserializeBinary
);


/**
 * @param {!proto.HandhsakeRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.HandshakeResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.HandshakeResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.handshake =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/Handshake',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_Handshake,
      callback);
};


/**
 * @param {!proto.HandhsakeRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.HandshakeResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.handshake =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/Handshake',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_Handshake);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.ComputeMatchesRequest,
 *   !proto.ComputeMatchesResponse>}
 */
const methodDescriptor_ReconstructionService_ComputeMatches = new grpc.web.MethodDescriptor(
  '/ReconstructionService/ComputeMatches',
  grpc.web.MethodType.UNARY,
  proto.ComputeMatchesRequest,
  proto.ComputeMatchesResponse,
  /**
   * @param {!proto.ComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ComputeMatchesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.ComputeMatchesRequest,
 *   !proto.ComputeMatchesResponse>}
 */
const methodInfo_ReconstructionService_ComputeMatches = new grpc.web.AbstractClientBase.MethodInfo(
  proto.ComputeMatchesResponse,
  /**
   * @param {!proto.ComputeMatchesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ComputeMatchesResponse.deserializeBinary
);


/**
 * @param {!proto.ComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.ComputeMatchesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.ComputeMatchesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.computeMatches =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_ComputeMatches,
      callback);
};


/**
 * @param {!proto.ComputeMatchesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.ComputeMatchesResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.computeMatches =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/ComputeMatches',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_ComputeMatches);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.SparseReconstructRequest,
 *   !proto.SparseReconstructResponse>}
 */
const methodDescriptor_ReconstructionService_SparseReconstruct = new grpc.web.MethodDescriptor(
  '/ReconstructionService/SparseReconstruct',
  grpc.web.MethodType.UNARY,
  proto.SparseReconstructRequest,
  proto.SparseReconstructResponse,
  /**
   * @param {!proto.SparseReconstructRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SparseReconstructResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.SparseReconstructRequest,
 *   !proto.SparseReconstructResponse>}
 */
const methodInfo_ReconstructionService_SparseReconstruct = new grpc.web.AbstractClientBase.MethodInfo(
  proto.SparseReconstructResponse,
  /**
   * @param {!proto.SparseReconstructRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SparseReconstructResponse.deserializeBinary
);


/**
 * @param {!proto.SparseReconstructRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.SparseReconstructResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.SparseReconstructResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.sparseReconstruct =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/SparseReconstruct',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SparseReconstruct,
      callback);
};


/**
 * @param {!proto.SparseReconstructRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.SparseReconstructResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.sparseReconstruct =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/SparseReconstruct',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SparseReconstruct);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetOBJRequest,
 *   !proto.GetOBJResponse>}
 */
const methodDescriptor_ReconstructionService_GetOBJ = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetOBJ',
  grpc.web.MethodType.SERVER_STREAMING,
  proto.GetOBJRequest,
  proto.GetOBJResponse,
  /**
   * @param {!proto.GetOBJRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetOBJResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetOBJRequest,
 *   !proto.GetOBJResponse>}
 */
const methodInfo_ReconstructionService_GetOBJ = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetOBJResponse,
  /**
   * @param {!proto.GetOBJRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetOBJResponse.deserializeBinary
);


/**
 * @param {!proto.GetOBJRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetOBJResponse>}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getOBJ =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/ReconstructionService/GetOBJ',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetOBJ);
};


/**
 * @param {!proto.GetOBJRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetOBJResponse>}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServicePromiseClient.prototype.getOBJ =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/ReconstructionService/GetOBJ',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetOBJ);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetSparseRequest,
 *   !proto.GetSparseResponse>}
 */
const methodDescriptor_ReconstructionService_GetSparse = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetSparse',
  grpc.web.MethodType.SERVER_STREAMING,
  proto.GetSparseRequest,
  proto.GetSparseResponse,
  /**
   * @param {!proto.GetSparseRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetSparseResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetSparseRequest,
 *   !proto.GetSparseResponse>}
 */
const methodInfo_ReconstructionService_GetSparse = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetSparseResponse,
  /**
   * @param {!proto.GetSparseRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetSparseResponse.deserializeBinary
);


/**
 * @param {!proto.GetSparseRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetSparseResponse>}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getSparse =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/ReconstructionService/GetSparse',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetSparse);
};


/**
 * @param {!proto.GetSparseRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.GetSparseResponse>}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServicePromiseClient.prototype.getSparse =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/ReconstructionService/GetSparse',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetSparse);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.NewReconstructionRequest,
 *   !proto.NewReconstructionResponse>}
 */
const methodDescriptor_ReconstructionService_NewReconstruction = new grpc.web.MethodDescriptor(
  '/ReconstructionService/NewReconstruction',
  grpc.web.MethodType.UNARY,
  proto.NewReconstructionRequest,
  proto.NewReconstructionResponse,
  /**
   * @param {!proto.NewReconstructionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.NewReconstructionResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.NewReconstructionRequest,
 *   !proto.NewReconstructionResponse>}
 */
const methodInfo_ReconstructionService_NewReconstruction = new grpc.web.AbstractClientBase.MethodInfo(
  proto.NewReconstructionResponse,
  /**
   * @param {!proto.NewReconstructionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.NewReconstructionResponse.deserializeBinary
);


/**
 * @param {!proto.NewReconstructionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.NewReconstructionResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.NewReconstructionResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.newReconstruction =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/NewReconstruction',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_NewReconstruction,
      callback);
};


/**
 * @param {!proto.NewReconstructionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.NewReconstructionResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.newReconstruction =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/NewReconstruction',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_NewReconstruction);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.DeleteReconstructionRequest,
 *   !proto.DeleteReconstructionResponse>}
 */
const methodDescriptor_ReconstructionService_DeleteReconstruction = new grpc.web.MethodDescriptor(
  '/ReconstructionService/DeleteReconstruction',
  grpc.web.MethodType.UNARY,
  proto.DeleteReconstructionRequest,
  proto.DeleteReconstructionResponse,
  /**
   * @param {!proto.DeleteReconstructionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.DeleteReconstructionResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.DeleteReconstructionRequest,
 *   !proto.DeleteReconstructionResponse>}
 */
const methodInfo_ReconstructionService_DeleteReconstruction = new grpc.web.AbstractClientBase.MethodInfo(
  proto.DeleteReconstructionResponse,
  /**
   * @param {!proto.DeleteReconstructionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.DeleteReconstructionResponse.deserializeBinary
);


/**
 * @param {!proto.DeleteReconstructionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.DeleteReconstructionResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.DeleteReconstructionResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.deleteReconstruction =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/DeleteReconstruction',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_DeleteReconstruction,
      callback);
};


/**
 * @param {!proto.DeleteReconstructionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.DeleteReconstructionResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.deleteReconstruction =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/DeleteReconstruction',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_DeleteReconstruction);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.StartSessionRequest,
 *   !proto.StartSessionResponse>}
 */
const methodDescriptor_ReconstructionService_StartSession = new grpc.web.MethodDescriptor(
  '/ReconstructionService/StartSession',
  grpc.web.MethodType.UNARY,
  proto.StartSessionRequest,
  proto.StartSessionResponse,
  /**
   * @param {!proto.StartSessionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.StartSessionResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.StartSessionRequest,
 *   !proto.StartSessionResponse>}
 */
const methodInfo_ReconstructionService_StartSession = new grpc.web.AbstractClientBase.MethodInfo(
  proto.StartSessionResponse,
  /**
   * @param {!proto.StartSessionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.StartSessionResponse.deserializeBinary
);


/**
 * @param {!proto.StartSessionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.StartSessionResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.StartSessionResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.startSession =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/StartSession',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_StartSession,
      callback);
};


/**
 * @param {!proto.StartSessionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.StartSessionResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.startSession =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/StartSession',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_StartSession);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.StopSessionRequest,
 *   !proto.StopSessionResponse>}
 */
const methodDescriptor_ReconstructionService_StopSession = new grpc.web.MethodDescriptor(
  '/ReconstructionService/StopSession',
  grpc.web.MethodType.UNARY,
  proto.StopSessionRequest,
  proto.StopSessionResponse,
  /**
   * @param {!proto.StopSessionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.StopSessionResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.StopSessionRequest,
 *   !proto.StopSessionResponse>}
 */
const methodInfo_ReconstructionService_StopSession = new grpc.web.AbstractClientBase.MethodInfo(
  proto.StopSessionResponse,
  /**
   * @param {!proto.StopSessionRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.StopSessionResponse.deserializeBinary
);


/**
 * @param {!proto.StopSessionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.StopSessionResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.StopSessionResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.stopSession =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/StopSession',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_StopSession,
      callback);
};


/**
 * @param {!proto.StopSessionRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.StopSessionResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.stopSession =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/StopSession',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_StopSession);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetReconstructionConfigRequest,
 *   !proto.GetReconstructionConfigResponse>}
 */
const methodDescriptor_ReconstructionService_GetReconstructionConfig = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetReconstructionConfig',
  grpc.web.MethodType.UNARY,
  proto.GetReconstructionConfigRequest,
  proto.GetReconstructionConfigResponse,
  /**
   * @param {!proto.GetReconstructionConfigRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetReconstructionConfigResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetReconstructionConfigRequest,
 *   !proto.GetReconstructionConfigResponse>}
 */
const methodInfo_ReconstructionService_GetReconstructionConfig = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetReconstructionConfigResponse,
  /**
   * @param {!proto.GetReconstructionConfigRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetReconstructionConfigResponse.deserializeBinary
);


/**
 * @param {!proto.GetReconstructionConfigRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.GetReconstructionConfigResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetReconstructionConfigResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getReconstructionConfig =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/GetReconstructionConfig',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetReconstructionConfig,
      callback);
};


/**
 * @param {!proto.GetReconstructionConfigRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetReconstructionConfigResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.getReconstructionConfig =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/GetReconstructionConfig',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetReconstructionConfig);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetAgentConfigRequest,
 *   !proto.GetAgentCOnfigResponse>}
 */
const methodDescriptor_ReconstructionService_GetAgentConfig = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetAgentConfig',
  grpc.web.MethodType.UNARY,
  proto.GetAgentConfigRequest,
  proto.GetAgentCOnfigResponse,
  /**
   * @param {!proto.GetAgentConfigRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetAgentCOnfigResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetAgentConfigRequest,
 *   !proto.GetAgentCOnfigResponse>}
 */
const methodInfo_ReconstructionService_GetAgentConfig = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetAgentCOnfigResponse,
  /**
   * @param {!proto.GetAgentConfigRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetAgentCOnfigResponse.deserializeBinary
);


/**
 * @param {!proto.GetAgentConfigRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.GetAgentCOnfigResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetAgentCOnfigResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getAgentConfig =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/GetAgentConfig',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetAgentConfig,
      callback);
};


/**
 * @param {!proto.GetAgentConfigRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetAgentCOnfigResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.getAgentConfig =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/GetAgentConfig',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetAgentConfig);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.MVSRequest,
 *   !proto.MVSResponse>}
 */
const methodDescriptor_ReconstructionService_MVS = new grpc.web.MethodDescriptor(
  '/ReconstructionService/MVS',
  grpc.web.MethodType.UNARY,
  proto.MVSRequest,
  proto.MVSResponse,
  /**
   * @param {!proto.MVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.MVSResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.MVSRequest,
 *   !proto.MVSResponse>}
 */
const methodInfo_ReconstructionService_MVS = new grpc.web.AbstractClientBase.MethodInfo(
  proto.MVSResponse,
  /**
   * @param {!proto.MVSRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.MVSResponse.deserializeBinary
);


/**
 * @param {!proto.MVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.MVSResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.MVSResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.mVS =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/MVS',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_MVS,
      callback);
};


/**
 * @param {!proto.MVSRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.MVSResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.mVS =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/MVS',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_MVS);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.SetAgentConfigFieldsRequest,
 *   !proto.SetAgentConfigFieldsResponse>}
 */
const methodDescriptor_ReconstructionService_SetAgentConfigFields = new grpc.web.MethodDescriptor(
  '/ReconstructionService/SetAgentConfigFields',
  grpc.web.MethodType.UNARY,
  proto.SetAgentConfigFieldsRequest,
  proto.SetAgentConfigFieldsResponse,
  /**
   * @param {!proto.SetAgentConfigFieldsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SetAgentConfigFieldsResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.SetAgentConfigFieldsRequest,
 *   !proto.SetAgentConfigFieldsResponse>}
 */
const methodInfo_ReconstructionService_SetAgentConfigFields = new grpc.web.AbstractClientBase.MethodInfo(
  proto.SetAgentConfigFieldsResponse,
  /**
   * @param {!proto.SetAgentConfigFieldsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SetAgentConfigFieldsResponse.deserializeBinary
);


/**
 * @param {!proto.SetAgentConfigFieldsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.SetAgentConfigFieldsResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.SetAgentConfigFieldsResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.setAgentConfigFields =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/SetAgentConfigFields',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SetAgentConfigFields,
      callback);
};


/**
 * @param {!proto.SetAgentConfigFieldsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.SetAgentConfigFieldsResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.setAgentConfigFields =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/SetAgentConfigFields',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SetAgentConfigFields);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.SetReconstructionConfigFieldsRequest,
 *   !proto.SetReconstructionConfigFieldsResponse>}
 */
const methodDescriptor_ReconstructionService_SetReconstructionConfigFields = new grpc.web.MethodDescriptor(
  '/ReconstructionService/SetReconstructionConfigFields',
  grpc.web.MethodType.UNARY,
  proto.SetReconstructionConfigFieldsRequest,
  proto.SetReconstructionConfigFieldsResponse,
  /**
   * @param {!proto.SetReconstructionConfigFieldsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SetReconstructionConfigFieldsResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.SetReconstructionConfigFieldsRequest,
 *   !proto.SetReconstructionConfigFieldsResponse>}
 */
const methodInfo_ReconstructionService_SetReconstructionConfigFields = new grpc.web.AbstractClientBase.MethodInfo(
  proto.SetReconstructionConfigFieldsResponse,
  /**
   * @param {!proto.SetReconstructionConfigFieldsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.SetReconstructionConfigFieldsResponse.deserializeBinary
);


/**
 * @param {!proto.SetReconstructionConfigFieldsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.SetReconstructionConfigFieldsResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.SetReconstructionConfigFieldsResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.setReconstructionConfigFields =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/SetReconstructionConfigFields',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SetReconstructionConfigFields,
      callback);
};


/**
 * @param {!proto.SetReconstructionConfigFieldsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.SetReconstructionConfigFieldsResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.setReconstructionConfigFields =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/SetReconstructionConfigFields',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_SetReconstructionConfigFields);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetAllImagesRequest,
 *   !proto.GetAllImagesResponse>}
 */
const methodDescriptor_ReconstructionService_GetAllImages = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetAllImages',
  grpc.web.MethodType.UNARY,
  proto.GetAllImagesRequest,
  proto.GetAllImagesResponse,
  /**
   * @param {!proto.GetAllImagesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetAllImagesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetAllImagesRequest,
 *   !proto.GetAllImagesResponse>}
 */
const methodInfo_ReconstructionService_GetAllImages = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetAllImagesResponse,
  /**
   * @param {!proto.GetAllImagesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetAllImagesResponse.deserializeBinary
);


/**
 * @param {!proto.GetAllImagesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.GetAllImagesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetAllImagesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getAllImages =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/GetAllImages',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetAllImages,
      callback);
};


/**
 * @param {!proto.GetAllImagesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetAllImagesResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.getAllImages =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/GetAllImages',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetAllImages);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetImageRegionsRequest,
 *   !proto.GetImageRegionsResponse>}
 */
const methodDescriptor_ReconstructionService_GetImageRegions = new grpc.web.MethodDescriptor(
  '/ReconstructionService/GetImageRegions',
  grpc.web.MethodType.UNARY,
  proto.GetImageRegionsRequest,
  proto.GetImageRegionsResponse,
  /**
   * @param {!proto.GetImageRegionsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetImageRegionsResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.GetImageRegionsRequest,
 *   !proto.GetImageRegionsResponse>}
 */
const methodInfo_ReconstructionService_GetImageRegions = new grpc.web.AbstractClientBase.MethodInfo(
  proto.GetImageRegionsResponse,
  /**
   * @param {!proto.GetImageRegionsRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetImageRegionsResponse.deserializeBinary
);


/**
 * @param {!proto.GetImageRegionsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.GetImageRegionsResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetImageRegionsResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.getImageRegions =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/GetImageRegions',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetImageRegions,
      callback);
};


/**
 * @param {!proto.GetImageRegionsRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetImageRegionsResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.getImageRegions =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/GetImageRegions',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_GetImageRegions);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.ScoreImagesRequest,
 *   !proto.ScoreImagesResponse>}
 */
const methodDescriptor_ReconstructionService_ScoreImage = new grpc.web.MethodDescriptor(
  '/ReconstructionService/ScoreImage',
  grpc.web.MethodType.UNARY,
  proto.ScoreImagesRequest,
  proto.ScoreImagesResponse,
  /**
   * @param {!proto.ScoreImagesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ScoreImagesResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.ScoreImagesRequest,
 *   !proto.ScoreImagesResponse>}
 */
const methodInfo_ReconstructionService_ScoreImage = new grpc.web.AbstractClientBase.MethodInfo(
  proto.ScoreImagesResponse,
  /**
   * @param {!proto.ScoreImagesRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.ScoreImagesResponse.deserializeBinary
);


/**
 * @param {!proto.ScoreImagesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.ScoreImagesResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.ScoreImagesResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ReconstructionServiceClient.prototype.scoreImage =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/ReconstructionService/ScoreImage',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_ScoreImage,
      callback);
};


/**
 * @param {!proto.ScoreImagesRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.ScoreImagesResponse>}
 *     A native promise that resolves to the response
 */
proto.ReconstructionServicePromiseClient.prototype.scoreImage =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/ReconstructionService/ScoreImage',
      request,
      metadata || {},
      methodDescriptor_ReconstructionService_ScoreImage);
};


module.exports = proto;

