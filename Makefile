BUILD_DIR = build
OUTPUT_DIR = output


.PHONY: clean build

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)
	mkdir -p $(OUTPUT_DIR)

build:
	@echo "Starting build process..."
	make clean
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)
	cp $(BUILD_DIR)/example/example $(OUTPUT_DIR)
	cp $(BUILD_DIR)/lib/liblog.a $(OUTPUT_DIR)

