#!/usr/bin/env python
import os
import sys
import shutil
import SCons

def GlobRecursive(pattern, node='.'):
    results = []
    for f in Glob(str(node) + '/*', source=True):
        if type(f) is SCons.Node.FS.Dir:
            results += GlobRecursive(pattern, f)
    results += Glob(str(node) + '/' + pattern, source=True)
    return results

def ensure_file(source, dest):
    if not os.path.isfile(os.path.abspath(dest)):
        shutil.copyfile(os.path.abspath(source), os.path.abspath(dest))
    return

env = SConscript("../godot-cpp/SConstruct")

env.Append(CPPPATH=["sources/"])
env.Append(CPPPATH=["../steam/include"])
env.Append(LIBPATH=["../steam/lib"])
if env["platform"] == "windows":
    env.Append(LIBS=["steam_api64"])
else:
    env.Append(LIBS=["steam_api"])


sources = GlobRecursive("*.cpp", "sources")

# Remove default O2 flag
if '-O2' in env['CCFLAGS']:
    env['CCFLAGS'].remove('-O2')

if env["target"] == "editor" or env["target"] == "template_debug":
    env.Append(CCFLAGS='-O0 -g')
else:
    env.Append(CCFLAGS='-O3')

library = env.SharedLibrary(
    "bin/spacerisk{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)

if env["platform"] == "windows":
    ensure_file("../windows/mingw/bin/libgcc_s_seh-1.dll", "bin/libgcc_s_seh-1.dll")
    ensure_file("../windows/mingw/bin/libstdc++-6.dll", "bin/libstdc++-6.dll")
    ensure_file("../windows/mingw/bin/libwinpthread-1.dll", "bin/libwinpthread-1.dll")
    ensure_file("../steam/bin/steam_api64.dll", "bin/steam_api64.dll")
    ensure_file("steam_appid.txt", "bin/steam_appid.txt")
elif env["platform"] == "linux":
    ensure_file("../steam/lib/libsteam_api.so", "bin/libsteam_api.so")
    ensure_file("steam_appid.txt", "bin/steam_appid.txt")
