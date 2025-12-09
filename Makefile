.DEFAULT_GOAL := build_all

PROTO_ROOT_DIR = src/sdk/protobuf
PROTO_FILES := $(wildcard $(PROTO_ROOT_DIR)/*.proto)

build_dir:
	mkdir -p ./build

update_dependencies:
	@git submodule update --init --recursive

compile_luajit:
	@cd ./external/luajit && make clean && make CC=clang STATIC_CC=clang BUILDMODE=static HOST_CC=clang CFLAGS="-O3 -march=native -DWIN32 -DLUAJIT_ENABLE_LUA52COMPAT" TARGET_SYS=Windows

build_sol3:
	@cd ./external/sol/single && python single.py

build_sdk_proto_files:
	@mkdir -p $(PROTO_ROOT_DIR)/gen
	@echo "Compiling all proto files at once..."
	protoc \
		--proto_path=$(PROTO_ROOT_DIR)/ \
		--cpp_out=$(PROTO_ROOT_DIR)/gen/ \
		$(PROTO_FILES)

build_dependencies: update_dependencies compile_luajit build_sol3 build_sdk_proto_files

build_all: build_dir build_dependencies