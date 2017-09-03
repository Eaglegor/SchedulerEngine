macro (UpdateDependencies)
  
  message(STATUS "Checking if dependencies have to be updated...")
  CheckDependencies()
  
  if(SCHEDULER_ENGINE_DEPENDENCIES_UPDATE_NEEDED)
    PrepareThirdPartyFolder()

	PreparePackages()
	
    if(WIN32)
		if(MSVC)
		  get_filename_component(DEVENV_PATH ${CMAKE_VS_DEVENV_COMMAND} DIRECTORY)
		  set(VSDEVCMD_PATH "${DEVENV_PATH}/../Tools/VsDevCmd.bat" CACHE FILEPATH "Path to the MS VsDevCmd.bat file")
		  
		  if(NOT VSDEVCMD_PATH) 
			message(FATAL_ERROR "Please set the path to the VsDevCmd.bat script")
		  endif()
		endif()
		
		Retrieve7Zip()
		Prepare7Zip()
    endif()
	
    RetrieveBoost()
    RetrieveCotire()
    RetrieveCatch()
    RetrieveSpdlog()
    RetrieveLibXML2()
	RetrieveCImg()
	
    PrepareBoost()
    PrepareCotire()
    PrepareCatch()
    PrepareSpdlog()
    PrepareLibXML2()
	PrepareCImg()
     
    CleanupTempDirectory()
    
    message(STATUS "Writing dependencies update script timestamp")
    file(MD5 ${CPP_SCHEDULER_SOURCE_DIR}/PrepareDeps.cmake DEPS_FILE_HASH)
    file(WRITE ${THIRD_PARTY_DIRECTORY}/deps_file_hash ${DEPS_FILE_HASH})
  endif()
  
  set(BOOST_ROOT ${THIRD_PARTY_DIRECTORY}/boost CACHE PATH "Root folder of boost" FORCE)
  set(COTIRE_ROOT ${THIRD_PARTY_DIRECTORY}/cotire CACHE PATH "Root folder of cotire" FORCE)
  set(CATCH_ROOT ${THIRD_PARTY_DIRECTORY}/catch CACHE PATH "Root folder of catch" FORCE)
  set(SPDLOG_ROOT ${THIRD_PARTY_DIRECTORY}/spdlog CACHE PATH "Root folder of spdlog" FORCE)
  set(LIBXML2_ROOT ${THIRD_PARTY_DIRECTORY}/libxml2 CACHE PATH "Root folder of libxml2" FORCE)
  set(CIMG_ROOT ${THIRD_PARTY_DIRECTORY}/cimg CACHE PATH "Root folder of cimg" FORCE)
  
  if(UNIX)
    set(ENV{PKG_CONFIG_PATH} "${THIRD_PARTY_DIRECTORY}/libxml2/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
  elseif(WIN32)
    set(ENV{CMAKE_PREFIX_PATH} "$ENV{CMAKE_PREFIX_PATH};${LIBXML2_ROOT}/include;${LIBXML2_ROOT}/lib;${LIBXML2_ROOT}/bin")
	add_definitions(-DBOOST_ALL_NO_LIB)
	add_definitions(-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
  endif()
 
endmacro()

macro (CheckDependencies)
  if(EXISTS ${THIRD_PARTY_DIRECTORY}/deps_file_hash)
    file(MD5 ${CPP_SCHEDULER_SOURCE_DIR}/PrepareDeps.cmake DEPS_FILE_HASH)
    file(READ ${THIRD_PARTY_DIRECTORY}/deps_file_hash STORED_DEPS_FILE_HASH)
    if(NOT DEPS_FILE_HASH STREQUAL STORED_DEPS_FILE_HASH)
      message(STATUS "Stored hash differs from the calculated one. Dependencies will be updated")
      set(SCHEDULER_ENGINE_DEPENDENCIES_UPDATE_NEEDED True)
    else()
      message(STATUS "Stored hash is actual. No dependencies update is needed")
      set(SCHEDULER_ENGINE_DEPENDENCIES_UPDATE_NEEDED False)
    endif()
  else()
    message(STATUS "No stored hash found. Dependencies will be updated")
    set(SCHEDULER_ENGINE_DEPENDENCIES_UPDATE_NEEDED True)
  endif()
endmacro()

macro (PrepareThirdPartyFolder)
  message(STATUS "Preparing third party folder")
  file(REMOVE_RECURSE ${THIRD_PARTY_DIRECTORY})
  file(MAKE_DIRECTORY ${THIRD_PARTY_DIRECTORY})
endmacro()

macro (PreparePackages)

	set(PACKAGE_7ZIP_SOURCE_URL http://www.7-zip.org/a/7za920.zip)
	set(PACKAGE_7ZIP_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/7za920.zip)
	set(PACKAGE_7ZIP_NAME 7za920.zip)
	set(PACKAGE_7ZIP_MD5 "2fac454a90ae96021f4ffc607d4c00f8")

	if(UNIX)
		set(PACKAGE_BOOST_SOURCE_URL http://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz)
		set(PACKAGE_BOOST_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/boost_1_64_0.tar.gz)
		set(PACKAGE_BOOST_NAME boost_1_64_0.tar.gz)
		set(PACKAGE_BOOST_MD5 "319c6ffbbeccc366f14bb68767a6db79")
	elseif(WIN32)
		set(PACKAGE_BOOST_SOURCE_URL http://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.zip)
		set(PACKAGE_BOOST_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/boost_1_64_0.zip)
		set(PACKAGE_BOOST_NAME boost_1_64_0.zip)
		set(PACKAGE_BOOST_INCLUDE_DIR_NAME boost-1_64)
		set(PACKAGE_BOOST_MD5 "36093e4018aecd5b0e31e80457ac5fc1")
	endif()
	set(PACKAGE_BOOST_FOLDER_NAME boost_1_64_0)
	set(PACKAGE_BOOST_PREFIX ${THIRD_PARTY_DIRECTORY}/boost)
	
	set(PACKAGE_COTIRE_SOURCE_URL https://github.com/sakra/cotire/releases/download/cotire-1.7.10/cotire.cmake)
	set(PACKAGE_COTIRE_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/cotire.cmake)
	set(PACKAGE_COTIRE_PREFIX ${THIRD_PARTY_DIRECTORY}/cotire)
	set(PACKAGE_COTIRE_MD5 "80d521367456046d4f3397a25395ea5b")
	
	set(PACKAGE_CATCH_SOURCE_URL https://github.com/philsquared/Catch/releases/download/v2.0.0-develop.3/catch.hpp)
	set(PACKAGE_CATCH_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/catch.hpp)
	set(PACKAGE_CATCH_PREFIX ${THIRD_PARTY_DIRECTORY}/catch)
	set(PACKAGE_CATCH_MD5 "26659f90195037a343f11d8671cfa31c")
	
	if(UNIX)
		set(PACKAGE_SPDLOG_SOURCE_URL https://github.com/gabime/spdlog/archive/v0.14.0.tar.gz)
		set(PACKAGE_SPDLOG_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/spdlog-v0.14.0.tar.gz)
		set(PACKAGE_SPDLOG_NAME spdlog-v0.14.0.tar.gz)
		set(PACKAGE_SPDLOG_MD5 "e8c83f739e0172b4687d625bb1da1aac")
	elseif(WIN32)
		set(PACKAGE_SPDLOG_SOURCE_URL https://github.com/gabime/spdlog/archive/v0.14.0.zip)
		set(PACKAGE_SPDLOG_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/spdlog-v0.14.0.zip)
		set(PACKAGE_SPDLOG_NAME spdlog-v0.14.0.zip)
		set(PACKAGE_SPDLOG_MD5 "f213d83c466aa7044a132e2488d71b11")
	endif()
	set(PACKAGE_SPDLOG_FOLDER_NAME spdlog-0.14.0)
	set(PACKAGE_SPDLOG_PREFIX ${THIRD_PARTY_DIRECTORY}/spdlog)
	
	set(PACKAGE_LIBXML2_SOURCE_URL http://xmlsoft.org/sources/libxml2-2.9.4.tar.gz)
	set(PACKAGE_LIBXML2_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/libxml2-2.9.4.tar.gz)
	set(PACKAGE_LIBXML2_PREFIX ${THIRD_PARTY_DIRECTORY}/libxml2)
	set(PACKAGE_LIBXML2_NAME libxml2-2.9.4.tar.gz)
	set(PACKAGE_LIBXML2_TAR_NAME libxml2-2.9.4.tar)
	set(PACKAGE_LIBXML2_FOLDER_NAME libxml2-2.9.4)
	set(PACKAGE_LIBXML2_MD5 "ae249165c173b1ff386ee8ad676815f5")
	
	if(UNIX)
		set(PACKAGE_CIMG_SOURCE_URL https://github.com/dtschump/CImg/archive/v.203.tar.gz)
		set(PACKAGE_CIMG_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/cimg-v.203.tar.gz)
		set(PACKAGE_CIMG_NAME cimg-v.203.tar.gz)
		set(PACKAGE_CIMG_MD5 "4300ed6af04c49f1d80036071abbc307")
	elseif(WIN32)
		set(PACKAGE_CIMG_SOURCE_URL https://github.com/dtschump/CImg/archive/v.203.zip)
		set(PACKAGE_CIMG_URI ${THIRD_PARTY_PACKAGES_DIRECTORY}/cimg-v.203.zip)
		set(PACKAGE_CIMG_NAME cimg-v.203.zip)
		set(PACKAGE_CIMG_MD5 "0e409171f66d77b86beccc4317ec4ea4")
	endif()
	set(PACKAGE_CIMG_FOLDER_NAME CImg-v.203)
	set(PACKAGE_CIMG_PREFIX ${THIRD_PARTY_DIRECTORY}/cimg)
	
endmacro()

macro (CheckIfDownloadNeeded uri md5 output)
	if(NOT EXISTS ${uri})
		message(STATUS "${uri} not found")
		set(${output} True)
	else()
		file(MD5 ${uri} PACKAGE_MD5)
		if(NOT ${PACKAGE_MD5} STREQUAL ${md5})
			message(STATUS "${uri} hash (${PACKAGE_MD5}) differs from the expected one (${md5})")
			set(${output} True)
		else()
			set(${output} False)
		endif()
	endif()
endmacro()

macro (Retrieve7Zip)
	CheckIfDownloadNeeded(${PACKAGE_7ZIP_URI} ${PACKAGE_7ZIP_MD5} 7ZIP_DOWNLOAD_NEEDED)
	if(7ZIP_DOWNLOAD_NEEDED)
		Download7Zip()
	endif()
endmacro()

macro (Download7Zip)
	message(STATUS "Downloading 7zip")
	file(DOWNLOAD ${PACKAGE_7ZIP_SOURCE_URL} ${PACKAGE_7ZIP_URI} SHOW_PROGRESS)
endmacro()

macro (Prepare7Zip)
	message(STATUS "Preparing 7zip")
	file(COPY ${PACKAGE_7ZIP_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/7zip)
	execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PACKAGE_7ZIP_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/7zip)
	set(7ZIP_EXECUTABLE ${THIRD_PARTY_DIRECTORY}/tmp/7zip/7za.exe)
endmacro()

macro (RetrieveBoost)
	CheckIfDownloadNeeded(${PACKAGE_BOOST_URI} ${PACKAGE_BOOST_MD5} BOOST_DOWNLOAD_NEEDED)
	if(BOOST_DOWNLOAD_NEEDED)
		DownloadBoost()
	endif()
endmacro()

macro (DownloadBoost)
	message(STATUS "Downloading boost")
	file(DOWNLOAD ${PACKAGE_BOOST_SOURCE_URL} ${PACKAGE_BOOST_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareBoost)
  message(STATUS "Preparing boost")
  
  if(UNIX)
	file(COPY ${PACKAGE_BOOST_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/boost)
  elseif(WIN32)
	file(COPY ${PACKAGE_BOOST_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/boost)
  endif()
  
  set(BOOST_BUILD_THREADS "1" CACHE STRING "Number of threads to use when building boost")
  
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(BOOST_ADDRESS_MODEL 64)
  else()
	set(BOOST_ADDRESS_MODEL 32)
  endif()
  
  set(BOOST_BUILD_OPTIONS link=shared address-model=${BOOST_ADDRESS_MODEL} --prefix=${PACKAGE_BOOST_PREFIX} -j ${BOOST_BUILD_THREADS} --with-program_options --with-system --with-date_time --with-filesystem --with-thread)
  if(UNIX)
	message(STATUS "Extracting boost archive")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PACKAGE_BOOST_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost)
	message(STATUS "Preparing Boost.build")
    execute_process(COMMAND ./bootstrap.sh WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
	message(STATUS "Building boost")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      execute_process(COMMAND ./b2 ${BOOST_BUILD_OPTIONS} toolset=clang install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      execute_process(COMMAND ./b2 ${BOOST_BUILD_OPTIONS} toolset=gcc install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
    endif()
	set(BOOST_INCLUDEDIR ${PACKAGE_BOOST_PREFIX}/include CACHE PATH "Path to the boost include dir" FORCE)
	set(BOOST_LIBRARYDIR ${PACKAGE_BOOST_PREFIX}/lib CACHE PATH "Path to the boost lib dir" FORCE)
  elseif(WIN32)
	message(STATUS "Extracting boost archive")
    execute_process(COMMAND ${7ZIP_EXECUTABLE} x -y ${PACKAGE_BOOST_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost)
    message(STATUS "Preparing Boost.build")
	execute_process(COMMAND bootstrap.bat WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
    message(STATUS "Building boost")
	if(MSVC)
      execute_process(COMMAND b2 ${BOOST_BUILD_OPTIONS} toolset=msvc install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
    else()
      execute_process(COMMAND b2 ${BOOST_BUILD_OPTIONS} toolset=gcc install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/boost/${PACKAGE_BOOST_FOLDER_NAME})
    endif()
	set(BOOST_INCLUDEDIR ${PACKAGE_BOOST_PREFIX}/include/${PACKAGE_BOOST_INCLUDE_DIR_NAME} CACHE PATH "Path to the boost include dir" FORCE)
	set(BOOST_LIBRARYDIR ${PACKAGE_BOOST_PREFIX}/lib CACHE PATH "Path to the boost lib dir" FORCE)
	add_definitions(-DBOOST_ALL_NO_LIB )
  endif()
endmacro()

macro (RetrieveCotire)
	CheckIfDownloadNeeded(${PACKAGE_COTIRE_URI} ${PACKAGE_COTIRE_MD5} COTIRE_DOWNLOAD_NEEDED)
	if(COTIRE_DOWNLOAD_NEEDED)
		DownloadCotire()
	endif()
endmacro()

macro (DownloadCotire)
	message(STATUS "Downloading cotire")
	file(DOWNLOAD ${PACKAGE_COTIRE_SOURCE_URL} ${PACKAGE_COTIRE_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareCotire)
	message(STATUS "Preparing cotire")
	file(COPY ${PACKAGE_COTIRE_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/cotire)
endmacro()

macro (RetrieveCatch)
	CheckIfDownloadNeeded(${PACKAGE_CATCH_URI} ${PACKAGE_CATCH_MD5} CATCH_DOWNLOAD_NEEDED)
	if(CATCH_DOWNLOAD_NEEDED)
		DownloadCatch()
	endif()
endmacro()

macro (DownloadCatch)
	message(STATUS "Downloading catch")
    file(DOWNLOAD ${PACKAGE_CATCH_SOURCE_URL} ${PACKAGE_CATCH_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareCatch)
  message(STATUS "Preparing catch")
  file(COPY ${PACKAGE_CATCH_URI} DESTINATION ${PACKAGE_CATCH_PREFIX})
endmacro()

macro (RetrieveSpdlog)
  CheckIfDownloadNeeded(${PACKAGE_SPDLOG_URI} ${PACKAGE_SPDLOG_MD5} SPDLOG_DOWNLOAD_NEEDED)
  if(SPDLOG_DOWNLOAD_NEEDED)
	DownloadSpdlog()
  endif()
endmacro()

macro (DownloadSpdlog)
	message(STATUS "Downloading spdlog")
	file(DOWNLOAD ${PACKAGE_SPDLOG_SOURCE_URL} ${PACKAGE_SPDLOG_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareSpdlog)
	message(STATUS "Preparing spdlog")
  
	file(COPY ${PACKAGE_SPDLOG_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/spdlog)
	if(UNIX)
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PACKAGE_SPDLOG_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/spdlog)
	elseif(WIN32)
		execute_process(COMMAND ${7ZIP_EXECUTABLE} x -y ${PACKAGE_SPDLOG_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/spdlog)
	endif()
	file(MAKE_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/spdlog/build)
	execute_process(COMMAND  ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=${PACKAGE_SPDLOG_PREFIX} ${THIRD_PARTY_DIRECTORY}/tmp/spdlog/${PACKAGE_SPDLOG_FOLDER_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/spdlog/build)
	execute_process(COMMAND ${CMAKE_COMMAND} --build .  --target install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/spdlog/build)
endmacro()

macro (RetrieveLibXML2)
	CheckIfDownloadNeeded(${PACKAGE_LIBXML2_URI} ${PACKAGE_LIBXML2_MD5} LIBXML2_DOWNLOAD_NEEDED)
	if(LIBXML2_DOWNLOAD_NEEDED)
		DownloadLibXML2()
	endif()
endmacro()

macro (DownloadLibXML2)
	message(STATUS "Downloading libxml2")
	file(DOWNLOAD ${PACKAGE_LIBXML2_SOURCE_URL} ${PACKAGE_LIBXML2_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareLibXML2)
  message(STATUS "Preparing libxml2")
  
  file(COPY ${PACKAGE_LIBXML2_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/libxml2)
  
  if(UNIX)
	execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PACKAGE_LIBXML2_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2)
    execute_process(COMMAND ./configure --without-python --without-iconv --prefix=${THIRD_PARTY_DIRECTORY}/libxml2 WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2/${PACKAGE_LIBXML2_FOLDER_NAME})
    execute_process(COMMAND make install WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2/${PACKAGE_LIBXML2_FOLDER_NAME})
  elseif(WIN32)
	execute_process(COMMAND ${7ZIP_EXECUTABLE} x -y ${PACKAGE_LIBXML2_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2)
	execute_process(COMMAND ${7ZIP_EXECUTABLE} x -y ${PACKAGE_LIBXML2_TAR_NAME}  WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2)
    if(MSVC)
	  execute_process(COMMAND cscript configure.js compiler=msvc iconv=no WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2/${PACKAGE_LIBXML2_FOLDER_NAME}/win32)
	  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	    execute_process(COMMAND ${VSDEVCMD_PATH} -arch=amd64 && nmake /F Makefile.msvc WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2/${PACKAGE_LIBXML2_FOLDER_NAME}/win32)
	  else()
	    execute_process(COMMAND ${VSDEVCMD_PATH} -arch=x86 && nmake /F Makefile.msvc WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/libxml2/${PACKAGE_LIBXML2_FOLDER_NAME}/win32)
	  endif()
	  file(GLOB LIBXML2_DLLS_LIST "${THIRD_PARTY_DIRECTORY}/tmp/libxml2/libxml2-2.9.4/win32/bin.msvc/*.dll")
	  foreach(fl ${LIBXML2_DLLS_LIST})
	    file(COPY ${fl} DESTINATION "${PACKAGE_LIBXML2_PREFIX}/bin")
	  endforeach()
	  file(GLOB LIBXML2_LIBS_LIST "${THIRD_PARTY_DIRECTORY}/tmp/libxml2/libxml2-2.9.4/win32/bin.msvc/*.lib")
	  foreach(fl ${LIBXML2_LIBS_LIST})
	    file(COPY ${fl} DESTINATION "${PACKAGE_LIBXML2_PREFIX}/lib")
	  endforeach()
	  file(COPY "${THIRD_PARTY_DIRECTORY}/tmp/libxml2/libxml2-2.9.4/include/libxml" DESTINATION "${PACKAGE_LIBXML2_PREFIX}/include")
	endif()
  endif()
endmacro()

macro (RetrieveCImg)
  CheckIfDownloadNeeded(${PACKAGE_CIMG_URI} ${PACKAGE_CIMG_MD5} CIMG_DOWNLOAD_NEEDED)
  if(CIMG_DOWNLOAD_NEEDED)
	DownloadCImg()
  endif()
endmacro()

macro (DownloadCImg)
	message(STATUS "Downloading cimg")
	file(DOWNLOAD ${PACKAGE_CIMG_SOURCE_URL} ${PACKAGE_CIMG_URI} SHOW_PROGRESS)
endmacro()

macro (PrepareCImg)
	message(STATUS "Preparing cimg")
  
	file(COPY ${PACKAGE_CIMG_URI} DESTINATION ${THIRD_PARTY_DIRECTORY}/tmp/cimg)
	if(UNIX)
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PACKAGE_CIMG_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/cimg)
	elseif(WIN32)
		execute_process(COMMAND ${7ZIP_EXECUTABLE} x -y ${PACKAGE_CIMG_NAME} WORKING_DIRECTORY ${THIRD_PARTY_DIRECTORY}/tmp/cimg)
	endif()
	file(COPY ${THIRD_PARTY_DIRECTORY}/tmp/cimg/${PACKAGE_CIMG_FOLDER_NAME}/CImg.h DESTINATION ${PACKAGE_CIMG_PREFIX})
endmacro()

macro (CleanupTempDirectory)
  message(STATUS "Cleaning up tmp directory")
  file(REMOVE_RECURSE ${THIRD_PARTY_DIRECTORY}/tmp)
endmacro()