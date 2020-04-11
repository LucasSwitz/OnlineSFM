# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: image_storage.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import server_pb2 as server__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='image_storage.proto',
  package='',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=b'\n\x13image_storage.proto\x1a\x0cserver.proto\"[\n\x11StoreImageRequest\x12\x19\n\x11reconstruction_id\x18\x01 \x01(\t\x12\x10\n\x08image_id\x18\x02 \x01(\t\x12\x19\n\x05image\x18\x03 \x01(\x0b\x32\n.ImageData\"\"\n\x12StoreImageResponse\x12\x0c\n\x04path\x18\x01 \x01(\t\">\n\x0fGetImageRequest\x12\x19\n\x11reconstruction_id\x18\x01 \x01(\t\x12\x10\n\x08image_id\x18\x02 \x01(\t\"-\n\x10GetImageResponse\x12\x19\n\x05image\x18\x01 \x01(\x0b\x32\n.ImageData2p\n\x0cImageStorage\x12\x32\n\x05Store\x12\x12.StoreImageRequest\x1a\x13.StoreImageResponse(\x01\x12,\n\x03Get\x12\x10.GetImageRequest\x1a\x11.GetImageResponse0\x01\x62\x06proto3'
  ,
  dependencies=[server__pb2.DESCRIPTOR,])




_STOREIMAGEREQUEST = _descriptor.Descriptor(
  name='StoreImageRequest',
  full_name='StoreImageRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='reconstruction_id', full_name='StoreImageRequest.reconstruction_id', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='image_id', full_name='StoreImageRequest.image_id', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='image', full_name='StoreImageRequest.image', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=37,
  serialized_end=128,
)


_STOREIMAGERESPONSE = _descriptor.Descriptor(
  name='StoreImageResponse',
  full_name='StoreImageResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='path', full_name='StoreImageResponse.path', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=130,
  serialized_end=164,
)


_GETIMAGEREQUEST = _descriptor.Descriptor(
  name='GetImageRequest',
  full_name='GetImageRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='reconstruction_id', full_name='GetImageRequest.reconstruction_id', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='image_id', full_name='GetImageRequest.image_id', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=166,
  serialized_end=228,
)


_GETIMAGERESPONSE = _descriptor.Descriptor(
  name='GetImageResponse',
  full_name='GetImageResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='image', full_name='GetImageResponse.image', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=230,
  serialized_end=275,
)

_STOREIMAGEREQUEST.fields_by_name['image'].message_type = server__pb2._IMAGEDATA
_GETIMAGERESPONSE.fields_by_name['image'].message_type = server__pb2._IMAGEDATA
DESCRIPTOR.message_types_by_name['StoreImageRequest'] = _STOREIMAGEREQUEST
DESCRIPTOR.message_types_by_name['StoreImageResponse'] = _STOREIMAGERESPONSE
DESCRIPTOR.message_types_by_name['GetImageRequest'] = _GETIMAGEREQUEST
DESCRIPTOR.message_types_by_name['GetImageResponse'] = _GETIMAGERESPONSE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

StoreImageRequest = _reflection.GeneratedProtocolMessageType('StoreImageRequest', (_message.Message,), {
  'DESCRIPTOR' : _STOREIMAGEREQUEST,
  '__module__' : 'image_storage_pb2'
  # @@protoc_insertion_point(class_scope:StoreImageRequest)
  })
_sym_db.RegisterMessage(StoreImageRequest)

StoreImageResponse = _reflection.GeneratedProtocolMessageType('StoreImageResponse', (_message.Message,), {
  'DESCRIPTOR' : _STOREIMAGERESPONSE,
  '__module__' : 'image_storage_pb2'
  # @@protoc_insertion_point(class_scope:StoreImageResponse)
  })
_sym_db.RegisterMessage(StoreImageResponse)

GetImageRequest = _reflection.GeneratedProtocolMessageType('GetImageRequest', (_message.Message,), {
  'DESCRIPTOR' : _GETIMAGEREQUEST,
  '__module__' : 'image_storage_pb2'
  # @@protoc_insertion_point(class_scope:GetImageRequest)
  })
_sym_db.RegisterMessage(GetImageRequest)

GetImageResponse = _reflection.GeneratedProtocolMessageType('GetImageResponse', (_message.Message,), {
  'DESCRIPTOR' : _GETIMAGERESPONSE,
  '__module__' : 'image_storage_pb2'
  # @@protoc_insertion_point(class_scope:GetImageResponse)
  })
_sym_db.RegisterMessage(GetImageResponse)



_IMAGESTORAGE = _descriptor.ServiceDescriptor(
  name='ImageStorage',
  full_name='ImageStorage',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  serialized_start=277,
  serialized_end=389,
  methods=[
  _descriptor.MethodDescriptor(
    name='Store',
    full_name='ImageStorage.Store',
    index=0,
    containing_service=None,
    input_type=_STOREIMAGEREQUEST,
    output_type=_STOREIMAGERESPONSE,
    serialized_options=None,
  ),
  _descriptor.MethodDescriptor(
    name='Get',
    full_name='ImageStorage.Get',
    index=1,
    containing_service=None,
    input_type=_GETIMAGEREQUEST,
    output_type=_GETIMAGERESPONSE,
    serialized_options=None,
  ),
])
_sym_db.RegisterServiceDescriptor(_IMAGESTORAGE)

DESCRIPTOR.services_by_name['ImageStorage'] = _IMAGESTORAGE

# @@protoc_insertion_point(module_scope)
