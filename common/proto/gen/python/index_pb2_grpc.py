# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import index_pb2 as index__pb2


class VisualIndexingServiceStub(object):
    """Missing associated documentation comment in .proto file"""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.IndexImage = channel.unary_unary(
                '/VisualIndexingService/IndexImage',
                request_serializer=index__pb2.IndexImageRequest.SerializeToString,
                response_deserializer=index__pb2.IndexImageResponse.FromString,
                )
        self.GetBagOfWords = channel.unary_unary(
                '/VisualIndexingService/GetBagOfWords',
                request_serializer=index__pb2.GetBagOfWordsRequest.SerializeToString,
                response_deserializer=index__pb2.GetBagOfWordsResponse.FromString,
                )
        self.ClosestN = channel.unary_unary(
                '/VisualIndexingService/ClosestN',
                request_serializer=index__pb2.ClosestNRequest.SerializeToString,
                response_deserializer=index__pb2.ClosestNResponse.FromString,
                )
        self.ScoreImage = channel.unary_unary(
                '/VisualIndexingService/ScoreImage',
                request_serializer=index__pb2.ScoreImageRequest.SerializeToString,
                response_deserializer=index__pb2.ScoreImageResponse.FromString,
                )


class VisualIndexingServiceServicer(object):
    """Missing associated documentation comment in .proto file"""

    def IndexImage(self, request, context):
        """Missing associated documentation comment in .proto file"""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetBagOfWords(self, request, context):
        """Missing associated documentation comment in .proto file"""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ClosestN(self, request, context):
        """Missing associated documentation comment in .proto file"""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ScoreImage(self, request, context):
        """Missing associated documentation comment in .proto file"""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_VisualIndexingServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'IndexImage': grpc.unary_unary_rpc_method_handler(
                    servicer.IndexImage,
                    request_deserializer=index__pb2.IndexImageRequest.FromString,
                    response_serializer=index__pb2.IndexImageResponse.SerializeToString,
            ),
            'GetBagOfWords': grpc.unary_unary_rpc_method_handler(
                    servicer.GetBagOfWords,
                    request_deserializer=index__pb2.GetBagOfWordsRequest.FromString,
                    response_serializer=index__pb2.GetBagOfWordsResponse.SerializeToString,
            ),
            'ClosestN': grpc.unary_unary_rpc_method_handler(
                    servicer.ClosestN,
                    request_deserializer=index__pb2.ClosestNRequest.FromString,
                    response_serializer=index__pb2.ClosestNResponse.SerializeToString,
            ),
            'ScoreImage': grpc.unary_unary_rpc_method_handler(
                    servicer.ScoreImage,
                    request_deserializer=index__pb2.ScoreImageRequest.FromString,
                    response_serializer=index__pb2.ScoreImageResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'VisualIndexingService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class VisualIndexingService(object):
    """Missing associated documentation comment in .proto file"""

    @staticmethod
    def IndexImage(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/VisualIndexingService/IndexImage',
            index__pb2.IndexImageRequest.SerializeToString,
            index__pb2.IndexImageResponse.FromString,
            options, channel_credentials,
            call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetBagOfWords(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/VisualIndexingService/GetBagOfWords',
            index__pb2.GetBagOfWordsRequest.SerializeToString,
            index__pb2.GetBagOfWordsResponse.FromString,
            options, channel_credentials,
            call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ClosestN(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/VisualIndexingService/ClosestN',
            index__pb2.ClosestNRequest.SerializeToString,
            index__pb2.ClosestNResponse.FromString,
            options, channel_credentials,
            call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ScoreImage(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/VisualIndexingService/ScoreImage',
            index__pb2.ScoreImageRequest.SerializeToString,
            index__pb2.ScoreImageResponse.FromString,
            options, channel_credentials,
            call_credentials, compression, wait_for_ready, timeout, metadata)
