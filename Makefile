.PHONY: proto build

proto: common/proto/src/*
	protoc -Icommon/proto/src --cpp_out=common/proto/gen/cpp --grpc_out=common/proto/gen/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --proto_path=common/proto/src common/proto/src/* --js_out=import_style=commonjs:common/proto/gen/js --grpc-web_out=import_style=commonjs,mode=grpcwebtext:common/proto/gen/js; \
	python3 -m grpc_tools.protoc -Icommon/proto/src --python_out=common/proto/gen/python --grpc_python_out=common/proto/gen/python  common/proto/src/*
build: proto src/*
	mkdir -p build; \
	cd build; \
	cmake ..; \
	make server
docker: src/*
	docker image build . --tag gateway

worker_docker: src/worker/* src/core/*
	docker build -t worker -f src/worker/Dockerfile .

core: src/core/*
	mkdir -p build; \
	cd build; \
	cmake ..; \
	make core

worker: src/worker/* core
	mkdir -p build; \
	cd build; \
	cmake ..; \
	make worker