abspath_wa = $(join $(filter %:,$(subst :,: ,$1)),$(abspath $(filter-out %:,$(subst :,: ,$1))))

LOCAL_PATH := $(call my-dir)

JNI_SRC_PATH := $(call abspath_wa, $(LOCAL_PATH)/../../Engine)
EXTERNS_PATH := $(call abspath_wa, $(LOCAL_PATH)/../../Externs)

include $(CLEAR_VARS)

LOCAL_MODULE    := waloengine

LOCAL_CFLAGS    := -std=c++11 -O0 -Wno-address-of-temporary

LOCAL_SRC_FILES := \
	$(EXTERNS_PATH)/box2d/box2d/Collision/Shapes/b2ChainShape.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/Shapes/b2CircleShape.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/Shapes/b2EdgeShape.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/Shapes/b2PolygonShape.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2BroadPhase.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2CollideCircle.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2CollideEdge.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2CollidePolygon.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2Collision.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2Distance.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2DynamicTree.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Collision/b2TimeOfImpact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2BlockAllocator.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2Draw.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2Math.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2Settings.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2StackAllocator.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Common/b2Timer.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2CircleContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2Contact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2ContactSolver.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Contacts/b2PolygonContact.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2DistanceJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2FrictionJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2GearJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2Joint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2MotorJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2MouseJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2PrismaticJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2PulleyJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2RevoluteJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2RopeJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2WeldJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/Joints/b2WheelJoint.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2Body.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2ContactManager.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2Fixture.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2Island.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2World.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Dynamics/b2WorldCallbacks.cpp \
	$(EXTERNS_PATH)/box2d/box2d/Rope/b2Rope.cpp \
	\
	$(EXTERNS_PATH)/pugixml/pugixml.cpp \
	\
	$(EXTERNS_PATH)/tinythread/tinythread.cpp \
	\
	$(EXTERNS_PATH)/stb/stb_image.c \
	$(EXTERNS_PATH)/stb/stb_image_write.c \
	$(EXTERNS_PATH)/stb/stb_vorbis.c \
	\
	$(EXTERNS_PATH)/openal/ALc/backends/android.c \
	$(EXTERNS_PATH)/openal/ALc/backends/loopback.c \
	$(EXTERNS_PATH)/openal/ALc/backends/null.c \
	$(EXTERNS_PATH)/openal/ALc/backends/opensl.c \
	$(EXTERNS_PATH)/openal/ALc/backends/opensl.c \
	$(EXTERNS_PATH)/openal/ALc/ALc.c \
	$(EXTERNS_PATH)/openal/ALc/alcConfig.c \
	$(EXTERNS_PATH)/openal/ALc/alcDedicated.c \
	$(EXTERNS_PATH)/openal/ALc/alcEcho.c \
	$(EXTERNS_PATH)/openal/ALc/alcModulator.c \
	$(EXTERNS_PATH)/openal/ALc/alcReverb.c \
	$(EXTERNS_PATH)/openal/ALc/alcRing.c \
	$(EXTERNS_PATH)/openal/ALc/alcThread.c \
	$(EXTERNS_PATH)/openal/ALc/ALu.c \
	$(EXTERNS_PATH)/openal/ALc/bs2b.c \
	$(EXTERNS_PATH)/openal/ALc/helpers.c \
	$(EXTERNS_PATH)/openal/ALc/hrtf.c \
	$(EXTERNS_PATH)/openal/ALc/mixer.c \
	$(EXTERNS_PATH)/openal/ALc/panning.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alAuxEffectSlot.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alBuffer.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alEffect.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alError.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alExtension.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alFilter.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alListener.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alSource.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alState.c \
	$(EXTERNS_PATH)/openal/OpenAL32/alThunk.c \
	\
	$(EXTERNS_PATH)/vorbis/lib/analysis.c \
	$(EXTERNS_PATH)/vorbis/lib/bitrate.c \
	$(EXTERNS_PATH)/vorbis/lib/block.c \
	$(EXTERNS_PATH)/vorbis/lib/codebook.c \
	$(EXTERNS_PATH)/vorbis/lib/envelope.c \
	$(EXTERNS_PATH)/vorbis/lib/floor0.c \
	$(EXTERNS_PATH)/vorbis/lib/floor1.c \
	$(EXTERNS_PATH)/vorbis/lib/info.c \
	$(EXTERNS_PATH)/vorbis/lib/lookup.c \
	$(EXTERNS_PATH)/vorbis/lib/lpc.c \
	$(EXTERNS_PATH)/vorbis/lib/lsp.c \
	$(EXTERNS_PATH)/vorbis/lib/mapping0.c \
	$(EXTERNS_PATH)/vorbis/lib/mdct.c \
	$(EXTERNS_PATH)/vorbis/lib/psy.c \
	$(EXTERNS_PATH)/vorbis/lib/registry.c \
	$(EXTERNS_PATH)/vorbis/lib/res0.c \
	$(EXTERNS_PATH)/vorbis/lib/sharedbook.c \
	$(EXTERNS_PATH)/vorbis/lib/smallft.c \
	$(EXTERNS_PATH)/vorbis/lib/synthesis.c \
	$(EXTERNS_PATH)/vorbis/lib/vorbisenc.c \
	$(EXTERNS_PATH)/vorbis/lib/vorbisfile.c \
	$(EXTERNS_PATH)/vorbis/lib/window.c \
	\
	$(EXTERNS_PATH)/libogg/src/bitwise.c \
	$(EXTERNS_PATH)/libogg/src/framing.c \
	\
	$(JNI_SRC_PATH)/Assets/Asset.cpp \
	$(JNI_SRC_PATH)/Assets/AssetsDB.cpp \
	$(JNI_SRC_PATH)/Core/Color.cpp \
	$(JNI_SRC_PATH)/Core/Log.cpp \
	$(JNI_SRC_PATH)/Core/Math.cpp \
	$(JNI_SRC_PATH)/Core/Matrix.cpp \
	$(JNI_SRC_PATH)/Core/Memory.cpp \
	$(JNI_SRC_PATH)/Core/String.cpp \
	$(JNI_SRC_PATH)/Device/Android/DeviceAndroid.cpp \
	$(JNI_SRC_PATH)/Device/Device.cpp \
	$(JNI_SRC_PATH)/Filesystem/Android/FileStreamAndroid.cpp \
	$(JNI_SRC_PATH)/Filesystem/FileStream.cpp \
	$(JNI_SRC_PATH)/Filesystem/FileSystem.cpp \
	$(JNI_SRC_PATH)/Filesystem/Stream.cpp \
	$(JNI_SRC_PATH)/Render/OpenGLES/IndexBufferGLES.cpp \
	$(JNI_SRC_PATH)/Render/OpenGLES/RenderGLES.cpp \
	$(JNI_SRC_PATH)/Render/OpenGLES/ShaderGLES.cpp \
	$(JNI_SRC_PATH)/Render/OpenGLES/TextureGLES.cpp \
	$(JNI_SRC_PATH)/Render/OpenGLES/VertexBufferGLES.cpp \
	$(JNI_SRC_PATH)/Render/Image.cpp \
	$(JNI_SRC_PATH)/Render/IndexBuffer.cpp \
	$(JNI_SRC_PATH)/Render/Material.cpp \
	$(JNI_SRC_PATH)/Render/Mesh.cpp \
	$(JNI_SRC_PATH)/Render/IndexBuffer.cpp \
	$(JNI_SRC_PATH)/Render/MeshBuffer.cpp \
	$(JNI_SRC_PATH)/Render/Render.cpp \
	$(JNI_SRC_PATH)/Render/Shader.cpp \
	$(JNI_SRC_PATH)/Render/Texture.cpp \
	$(JNI_SRC_PATH)/Render/VertexBuffer.cpp \
	$(JNI_SRC_PATH)/Scene/Input/Gesturer.cpp \
	$(JNI_SRC_PATH)/Scene/Input/InputSystem.cpp \
	$(JNI_SRC_PATH)/Scene/Physics2D/CollisionBox2D.cpp \
	$(JNI_SRC_PATH)/Scene/Physics2D/CollisionShape2D.cpp \
	$(JNI_SRC_PATH)/Scene/Physics2D/Physics2D.cpp \
	$(JNI_SRC_PATH)/Scene/Physics2D/RigidBody2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/Batch2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/Font2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/Object2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/Quadtree.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/Scene2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene2D/TileMap2D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene3D/Camera3D.cpp \
	$(JNI_SRC_PATH)/Scene/Scene3D/Octree.cpp \
	$(JNI_SRC_PATH)/Scene/Scene3D/Scene3D.cpp \
	$(JNI_SRC_PATH)/Scene/Sound/SoundSystem.cpp \
	$(JNI_SRC_PATH)/Scene/Update/UpdateSystem.cpp \
	$(JNI_SRC_PATH)/Scene/DebugRenderer.cpp \
	$(JNI_SRC_PATH)/Scene/Node.cpp \
	$(JNI_SRC_PATH)/Sound/SoundManager.cpp \
	$(JNI_SRC_PATH)/Sound/SoundStream.cpp \
	$(JNI_SRC_PATH)/Game.cpp 

LOCAL_C_INCLUDES :=	\
	$(JNI_SRC_PATH) \
	$(EXTERNS_PATH)/stb \
	$(EXTERNS_PATH)/box2d \
	$(EXTERNS_PATH)/libogg \
	$(EXTERNS_PATH)/openal \
	$(EXTERNS_PATH)/openal/OpenAL32/include \
	$(EXTERNS_PATH)/pugixml \
	$(EXTERNS_PATH)/tinythread \
	$(EXTERNS_PATH)/rapidjson/include \
	$(EXTERNS_PATH)/vorbis \
	$(EXTERNS_PATH)/vorbis/lib
				
LOCAL_CFLAGS += -D_ANDROID -D_DEBUG -fpermissive

include $(BUILD_STATIC_LIBRARY)