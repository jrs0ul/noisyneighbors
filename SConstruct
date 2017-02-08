import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = ["src/mainer.cpp",
        "src/Xml.cpp",
        "src/gui/Text.cpp",
        "src/TextureLoader.cpp",
        "src/Image.cpp",
        "src/Vectors.cpp",
        "src/gui/Button.cpp",
        "src/Utils.cpp",
        "src/Extensions.cpp",
        "src/Particles2D.cpp",
        "src/Matrix.cpp",
        "src/gui/YesNoDialog.cpp",
        "src/CppSingleton.cpp",
        "src/audio/OggStream.cpp",
        "src/audio/SoundSystem.cpp",
        "src/OldMan.cpp",
        "src/Driller.cpp",
        "src/Brat.cpp",
        "src/Door.cpp",
        "src/Dog.cpp",
        "src/HighScore.cpp",
        "src/Projectile.cpp",
        "src/ProjectileList.cpp",
        "src/externals.cpp",
        "src/SysConfig.cpp",
        "src/SDLVideo.cpp",
        "src/Threads.cpp"

        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDLmain', 'SDL', 'OpenGL32',
                       'openal32', 'vorbisfile', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg', 'libcurl']
        env.Append(LIBS = ['iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
    else: #Linux
        env.Append(LIBS = ['SDL','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread'])


env.Program(target = "NoisyNeighbors", 
            source = code
            )

