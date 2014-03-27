set(_MAQXT_PATHS PATHS
    ${MAQXT_ROOT_DIR}
)


find_path(MAQXT_INCLUDE_DIR core/maqxtglobal.h ${_MAQXT_PATHS} PATH_SUFFIXES include ${MAQXT_INCLUDE_PATH_SUFFIXES} NO_DEFAULT_PATH )

find_library(MAQXT_LIBRARY NAMES maqxt ${_MAQXT_PATHS} PATH_SUFFIXES lib ${MAQXT_LIBRARY_PATH_SUFFIXES} NO_DEFAULT_PATH )
get_filename_component(MAQXT_LIBRARY_DIR ${MAQXT_LIBRARY} PATH)
set(MAQXT_LIBRARY_DIR ${MAQXT_LIBRARY_DIR} CACHE PATH "")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libmaqxt
    REQUIRED_VARS MAQXT_LIBRARY MAQXT_INCLUDE_DIR
    )

mark_as_advanced(MAQXT_INCLUDE_DIR MAQXT_LIBRARY MAQXT_LIBRARY_DIR)

