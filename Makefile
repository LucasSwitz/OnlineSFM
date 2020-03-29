.PHONY: proto build

proto: common/proto/src/*
	protoc -Icommon/proto/src --cpp_out=common/proto/gen/cpp --grpc_out=common/proto/gen/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --proto_path=common/proto/src common/proto/src/*; \
	python3 -m grpc_tools.protoc -Icommon/proto/src --python_out=common/proto/gen/python --grpc_python_out=common/proto/gen/python  common/proto/src/*
build: src/*
	mkdir -p build; \
	cd build; \
	cmake ..; \
	make server
docker: src/*
	docker image build . --tag rserver