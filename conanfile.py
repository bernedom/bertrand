from conans import ConanFile, CMake


class bertrandConan(ConanFile):
    name = "bertrand"
    version = "0.0.11"
    license = "LGPLv3"
    author = "Dominik Berner <dominik.berner+bertrand-conan@gmail.com"
    url = "https://github.com/bernedom/bertrand"
    description = "A C++ header only library providing functionalitiy for design by contract"
    topics = ("design-by-contract",
              "cplusplus-library", "cplusplus-17")
    exports_sources = "include/*", "CMakeLists.txt", "test/*", "cmake/bertrandConfig.cmake.in", "LICENSE"
    no_copy_source = True
    generators = "cmake", "txt", "cmake_find_package"
    build_requires = "Catch2/2.11.1@catchorg/stable"

    def _configure_cmake(self):
        cmake = CMake(self)
        # Add additional settings with cmake.definitions["SOME_DEFINITION"] = True
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def test(self):
        cmake = self._configure_cmake()
        cmake.test()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_id(self):
        self.info.header_only()
