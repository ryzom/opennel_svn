FILE(GLOB SRC *.cpp *.h)

DECORATE_NEL_LIB("nel3d")
SET(NL3D_LIB ${LIBNAME})
DECORATE_NEL_LIB("nelmisc")
SET(NLMISC_LIB ${LIBNAME})

SOURCE_GROUP(animation FILES
        anim_ctrl.cpp animatable.cpp animatable.h animated_lightmap.cpp animated_lightmap.h
        animated_material.cpp animated_material.h animated_morph.cpp animated_morph.h
        animated_value.cpp animated_value.h animation.cpp animation.h animation_optimizer.cpp
        animation_optimizer.h animation_playlist.cpp animation_playlist.h animation_set.cpp
        animation_set.h animation_time.cpp bone.cpp bone.h channel_mixer.cpp channel_mixer.h
        key.cpp key.h play_list_manager.cpp play_list_manager.h skeleton_spawn_script.cpp
        skeleton_spawn_script.h skeleton_weight.cpp skeleton_weight.h target_anim_ctrl.cpp
        track.cpp track.h track_bezier.h track_keyframer.cpp track_keyframer.h track_sampled_common.cpp
        track_sampled_common.h track_sampled_quat.cpp track_sampled_quat.h track_sampled_quat_small_header.cpp
        track_sampled_quat_small_header.h track_sampled_vector.cpp track_sampled_vector.h track_tcb.h
        transformable.cpp transformable.h)

SOURCE_GROUP(clip_accelerator FILES cluster.cpp cluster.h portal.cpp portal.h quad_grid_clip_cluster.cpp
        quad_grid_clip_cluster.h quad_grid_clip_manager.cpp quad_grid_clip_manager.h)

SOURCE_GROUP(cloud_scape FILES cloud.cpp cloud.h cloud_scape.cpp cloud_scape.h noise_3d.cpp noise_3d.h)

SOURCE_GROUP(collisions FILES camera_col.cpp camera_col.h landscape_collision_grid.cpp landscape_collision_grid.h
        mini_col.cpp mini_col.h packed_world.cpp packed_world.h packed_zone.cpp packed_zone.h ray_mesh.cpp
        ray_mesh.h shape_info.cpp shape_info.h visual_collision_entity.cpp visual_collision_entity.h
        visual_collision_manager.cpp visual_collision_manager.h visual_collision_mesh.cpp visual_collision_mesh.h)

SOURCE_GROUP(container FILES bsp_tree.h cube_grid.cpp cube_grid.h fast_ptr_list.cpp fast_ptr_list.h
        layed_ordering_table.h ordering_table.h ptr_set.cpp ptr_set.h quad_grid.cpp quad_grid.h
        static_quad_grid.cpp static_quad_grid.h)

SOURCE_GROUP(font FILES computed_string.cpp computed_string.h font_generator.cpp font_generator.h
        font_manager.h font_manager.cpp text_context.cpp text_context.h)

IF(NOT WIN32)
  ADD_LIBRARY(${NL3D_LIB} SHARED ${SRC})
ELSE(NOT WIN32)
  ADD_LIBRARY(${NL3D_LIB} STATIC ${SRC})
ENDIF(NOT WIN32)

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${NL3D_LIB} ${NLMISC_LIB} ${FREETYPE_LIBRARY} ${JPEG_LIBRARY})
SET_TARGET_PROPERTIES(${NL3D_LIB} PROPERTIES VERSION ${NL_VERSION})

IF(WIN32)
  SET_TARGET_PROPERTIES(${NL3D_LIB} PROPERTIES COMPILE_FLAGS "/Yustd3d.h")
  SET_SOURCE_FILES_PROPERTIES(std3d.cpp PROPERTIES COMPILE_FLAGS "/Ycstd3d.h")
  SET_TARGET_PROPERTIES(${NL3D_LIB} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")

  IF(JPEG_FOUND)
    ADD_DEFINITIONS(/DUSE_JPEG)
  ENDIF(JPEG_FOUND)
ELSE(WIN32)
  IF(JPEG_FOUND)
    ADD_DEFINITIONS(-DUSE_JPEG)
  ENDIF(JPEG_FOUND)
ENDIF(WIN32)

ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INSTALL(TARGETS ${NL3D_LIB} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

ADD_SUBDIRECTORY(driver)
