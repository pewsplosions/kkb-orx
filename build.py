""" Simple way to build, compile, and deploy. """
import ntpath
import os
import sys
import shutil

### PROJECT CONFIGURATION

exts = [] # Can build other meson projects in the ext dir if listed here.
base_name = "Kamikaze Birds" # Project name.
build_type_flag = " -Dbuildtype=debug" # Can configure meson cli flags here.
base_dir = os.getcwd()

### END PROJECT CONFIGURATION

class colors:
  """ Colors for terminal output. """
  END = "\033[0m"
  GO = "\033[1;32m"
  TASK = "\033[1;94m"
  ALERT = "\033[1;31m"

def meson(what):
  """ Create meson build setup in current directory. """
  print("{}{}{}{}".format(colors.TASK, "Meson: ", what, colors.END))
  os.system("meson setup build" + build_type_flag)

def ninja(what):
  """ Compile the setup using ninja for the current directory. """
  print("{}{}{}{}".format(colors.TASK, "Ninja: ", what, colors.END))
  os.system("ninja -C build")

def clean(what):
  """ Remove the build setup for the current directory. """
  print("{}{}{}{}".format(colors.ALERT, "Clean: ", what, colors.END))
  shutil.rmtree("./build", ignore_errors=True)

def meson_ext():
  """ Set up the meson build for ext projects. """
  for item in exts:
    os.chdir(item)
    meson(item)
    os.chdir(base_dir)

def ninja_ext():
  """ Compile the meson builds for ext projects. """
  for item in exts:
    os.chdir(item)
    ninja(item)
    os.chdir(base_dir)

def clean_ext():
  """ Remove the build setup for ext projects. """
  for item in exts:
    os.chdir(item)
    clean(item)
    os.chdir(base_dir)

def meson_all():
  """ Meson ext projects then root project. """
  meson_ext()
  meson(base_name)

def ninja_all():
  """ Ninja ext projects then root project. """
  ninja_ext()
  ninja(base_name)

def clean_all():
  """ Clean ext projects then root project. """
  clean_ext()
  clean(base_name)

def update_submodules():
  """ Recursively pull then update submodules. """
  os.system("git submodule update --init --recursive")
  os.system("git submodule update --remote --merge")

def build_exts():
  """ For each ext project init python script or build with meson/ninja. """
  for item in exts:
    os.chdir(item)
    if ntpath.exists("build.py"):
      os.system("python build.py init")
    else:
      meson(item)
      ninja(item)
    os.chdir(base_dir)

# Start printing commands.
print(colors.GO + "Start" + colors.END)

for arg in sys.argv:
  if (arg == sys.argv[0]):
    continue  # Skip the python script's name.
  if (arg == "clean"):
    clean_all()
  if (arg == "ninja"):
    ninja_all()
  if (arg == "meson"):
    meson_all()
  if (arg == "update"):
    update_submodules()
  if (arg == "exts"):
    meson_ext()
    ninja_ext()
  if (arg == "init"):
    update_submodules()
    build_exts()
    meson(base_name)
    ninja(base_name)

if (len(sys.argv) == 1):
  ninja_all()

print(colors.GO + "Done" + colors.END)
