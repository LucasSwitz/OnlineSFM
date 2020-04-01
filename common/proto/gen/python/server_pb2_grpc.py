# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import server_pb2 as server__pb2


class ReconstructionServiceStub(object):
  # missing associated documentation comment in .proto file
  pass

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.Handshake = channel.unary_unary(
        '/ReconstructionService/Handshake',
        request_serializer=server__pb2.HandhsakeRequest.SerializeToString,
        response_deserializer=server__pb2.HandshakeResponse.FromString,
        )
    self.StoreImage = channel.stream_unary(
        '/ReconstructionService/StoreImage',
        request_serializer=server__pb2.StoreImageRequest.SerializeToString,
        response_deserializer=server__pb2.StoreImageResponse.FromString,
        )
    self.SparseReconstruct = channel.unary_unary(
        '/ReconstructionService/SparseReconstruct',
        request_serializer=server__pb2.SparseReconstructRequest.SerializeToString,
        response_deserializer=server__pb2.SparseReconstructResponse.FromString,
        )
    self.GetOBJ = channel.unary_stream(
        '/ReconstructionService/GetOBJ',
        request_serializer=server__pb2.GetOBJRequest.SerializeToString,
        response_deserializer=server__pb2.GetOBJResponse.FromString,
        )
    self.GetSparse = channel.unary_stream(
        '/ReconstructionService/GetSparse',
        request_serializer=server__pb2.GetSparseRequest.SerializeToString,
        response_deserializer=server__pb2.GetSparseResponse.FromString,
        )
    self.NewReconstruction = channel.unary_unary(
        '/ReconstructionService/NewReconstruction',
        request_serializer=server__pb2.NewReconstructionRequest.SerializeToString,
        response_deserializer=server__pb2.NewReconstructionResponse.FromString,
        )
    self.DeleteReconstruction = channel.unary_unary(
        '/ReconstructionService/DeleteReconstruction',
        request_serializer=server__pb2.DeleteReconstructionRequest.SerializeToString,
        response_deserializer=server__pb2.DeleteReconstructionResponse.FromString,
        )
    self.StartSession = channel.unary_unary(
        '/ReconstructionService/StartSession',
        request_serializer=server__pb2.StartSessionRequest.SerializeToString,
        response_deserializer=server__pb2.StartSessionResponse.FromString,
        )
    self.StopSession = channel.unary_unary(
        '/ReconstructionService/StopSession',
        request_serializer=server__pb2.StopSessionRequest.SerializeToString,
        response_deserializer=server__pb2.StopSessionResponse.FromString,
        )
    self.SessionAddImage = channel.stream_unary(
        '/ReconstructionService/SessionAddImage',
        request_serializer=server__pb2.SessionAddImageRequest.SerializeToString,
        response_deserializer=server__pb2.SessionAddImageResponse.FromString,
        )
    self.GetReconstructionConfig = channel.unary_unary(
        '/ReconstructionService/GetReconstructionConfig',
        request_serializer=server__pb2.GetReconstructionConfigRequest.SerializeToString,
        response_deserializer=server__pb2.GetReconstructionConfigResponse.FromString,
        )
    self.GetAgentConfig = channel.unary_unary(
        '/ReconstructionService/GetAgentConfig',
        request_serializer=server__pb2.GetAgentConfigRequest.SerializeToString,
        response_deserializer=server__pb2.GetAgentCOnfigResponse.FromString,
        )
    self.ReconstructionUploadImageBatch = channel.stream_unary(
        '/ReconstructionService/ReconstructionUploadImageBatch',
        request_serializer=server__pb2.ReconstructionUploadImageBatchRequest.SerializeToString,
        response_deserializer=server__pb2.ReconstructionUploadImageBatchResponse.FromString,
        )
    self.MVS = channel.unary_unary(
        '/ReconstructionService/MVS',
        request_serializer=server__pb2.MVSRequest.SerializeToString,
        response_deserializer=server__pb2.MVSResponse.FromString,
        )
    self.SetAgentConfigFields = channel.unary_unary(
        '/ReconstructionService/SetAgentConfigFields',
        request_serializer=server__pb2.SetAgentConfigFieldsRequest.SerializeToString,
        response_deserializer=server__pb2.SetAgentConfigFieldsResponse.FromString,
        )
    self.SetReconstructionConfigFields = channel.unary_unary(
        '/ReconstructionService/SetReconstructionConfigFields',
        request_serializer=server__pb2.SetReconstructionConfigFieldsRequest.SerializeToString,
        response_deserializer=server__pb2.SetReconstructionConfigFieldsResponse.FromString,
        )


class ReconstructionServiceServicer(object):
  # missing associated documentation comment in .proto file
  pass

  def Handshake(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def StoreImage(self, request_iterator, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SparseReconstruct(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def GetOBJ(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def GetSparse(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def NewReconstruction(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def DeleteReconstruction(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def StartSession(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def StopSession(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SessionAddImage(self, request_iterator, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def GetReconstructionConfig(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def GetAgentConfig(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def ReconstructionUploadImageBatch(self, request_iterator, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def MVS(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SetAgentConfigFields(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def SetReconstructionConfigFields(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_ReconstructionServiceServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'Handshake': grpc.unary_unary_rpc_method_handler(
          servicer.Handshake,
          request_deserializer=server__pb2.HandhsakeRequest.FromString,
          response_serializer=server__pb2.HandshakeResponse.SerializeToString,
      ),
      'StoreImage': grpc.stream_unary_rpc_method_handler(
          servicer.StoreImage,
          request_deserializer=server__pb2.StoreImageRequest.FromString,
          response_serializer=server__pb2.StoreImageResponse.SerializeToString,
      ),
      'SparseReconstruct': grpc.unary_unary_rpc_method_handler(
          servicer.SparseReconstruct,
          request_deserializer=server__pb2.SparseReconstructRequest.FromString,
          response_serializer=server__pb2.SparseReconstructResponse.SerializeToString,
      ),
      'GetOBJ': grpc.unary_stream_rpc_method_handler(
          servicer.GetOBJ,
          request_deserializer=server__pb2.GetOBJRequest.FromString,
          response_serializer=server__pb2.GetOBJResponse.SerializeToString,
      ),
      'GetSparse': grpc.unary_stream_rpc_method_handler(
          servicer.GetSparse,
          request_deserializer=server__pb2.GetSparseRequest.FromString,
          response_serializer=server__pb2.GetSparseResponse.SerializeToString,
      ),
      'NewReconstruction': grpc.unary_unary_rpc_method_handler(
          servicer.NewReconstruction,
          request_deserializer=server__pb2.NewReconstructionRequest.FromString,
          response_serializer=server__pb2.NewReconstructionResponse.SerializeToString,
      ),
      'DeleteReconstruction': grpc.unary_unary_rpc_method_handler(
          servicer.DeleteReconstruction,
          request_deserializer=server__pb2.DeleteReconstructionRequest.FromString,
          response_serializer=server__pb2.DeleteReconstructionResponse.SerializeToString,
      ),
      'StartSession': grpc.unary_unary_rpc_method_handler(
          servicer.StartSession,
          request_deserializer=server__pb2.StartSessionRequest.FromString,
          response_serializer=server__pb2.StartSessionResponse.SerializeToString,
      ),
      'StopSession': grpc.unary_unary_rpc_method_handler(
          servicer.StopSession,
          request_deserializer=server__pb2.StopSessionRequest.FromString,
          response_serializer=server__pb2.StopSessionResponse.SerializeToString,
      ),
      'SessionAddImage': grpc.stream_unary_rpc_method_handler(
          servicer.SessionAddImage,
          request_deserializer=server__pb2.SessionAddImageRequest.FromString,
          response_serializer=server__pb2.SessionAddImageResponse.SerializeToString,
      ),
      'GetReconstructionConfig': grpc.unary_unary_rpc_method_handler(
          servicer.GetReconstructionConfig,
          request_deserializer=server__pb2.GetReconstructionConfigRequest.FromString,
          response_serializer=server__pb2.GetReconstructionConfigResponse.SerializeToString,
      ),
      'GetAgentConfig': grpc.unary_unary_rpc_method_handler(
          servicer.GetAgentConfig,
          request_deserializer=server__pb2.GetAgentConfigRequest.FromString,
          response_serializer=server__pb2.GetAgentCOnfigResponse.SerializeToString,
      ),
      'ReconstructionUploadImageBatch': grpc.stream_unary_rpc_method_handler(
          servicer.ReconstructionUploadImageBatch,
          request_deserializer=server__pb2.ReconstructionUploadImageBatchRequest.FromString,
          response_serializer=server__pb2.ReconstructionUploadImageBatchResponse.SerializeToString,
      ),
      'MVS': grpc.unary_unary_rpc_method_handler(
          servicer.MVS,
          request_deserializer=server__pb2.MVSRequest.FromString,
          response_serializer=server__pb2.MVSResponse.SerializeToString,
      ),
      'SetAgentConfigFields': grpc.unary_unary_rpc_method_handler(
          servicer.SetAgentConfigFields,
          request_deserializer=server__pb2.SetAgentConfigFieldsRequest.FromString,
          response_serializer=server__pb2.SetAgentConfigFieldsResponse.SerializeToString,
      ),
      'SetReconstructionConfigFields': grpc.unary_unary_rpc_method_handler(
          servicer.SetReconstructionConfigFields,
          request_deserializer=server__pb2.SetReconstructionConfigFieldsRequest.FromString,
          response_serializer=server__pb2.SetReconstructionConfigFieldsResponse.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'ReconstructionService', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
