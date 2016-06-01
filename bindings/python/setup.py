#
# Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
# Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
#
# This file is part of the LibIodef library.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.


import sys, os
from os.path import abspath, exists
import shutil

from distutils.sysconfig import get_python_lib
from distutils.core import setup, Extension

FILES_TO_BE_COPIED = ("_iodef.cxx", "iodef.py")


def split_args(s):
    import re
    return re.split("\s+", s.strip())


def get_root():
    try:
        return sys.argv[sys.argv.index("--root") + 1]
    except ValueError:
        return ""


def is_system_wide_install():
    return os.access(get_python_lib(), os.W_OK)


def builddir_is_srcdir():
    return abspath("../..") == abspath("../..")


def pre_install():
    if not is_system_wide_install():
        sys.argv.extend(["--prefix", "/usr/local"])


def pre_build():
    if not builddir_is_srcdir():
        for file in FILES_TO_BE_COPIED:
            src = "../../bindings/python/" + file
            dst = "../../bindings/python/" + file
            if not exists(dst):
                shutil.copy(src, dst)


def pre_clean():
    if not builddir_is_srcdir():
        for file in FILES_TO_BE_COPIED:
            exists(file) and os.remove(file)


def uninstall():
    if is_system_wide_install():
        prefix = None
    else:
        prefix = "/usr/local"
    
    for f in "iodef.py", "iodef.pyc", "_iodef.so":
        file = get_root() + "/" + get_python_lib(prefix=prefix) + "/" + f
        exists(file) and os.remove(file)

        file = get_root() + "/" + get_python_lib(plat_specific=True, prefix=prefix) + "/" + f
        exists(file) and os.remove(file)

    sys.exit(0)



commands = {
    "install": pre_install,
    "build": pre_build,
    "clean": pre_clean,
    "uninstall": uninstall,
    }

if len(sys.argv) > 1 and sys.argv[1] in commands:
    commands[sys.argv[1]]()

setup(name="iodef",
      version="3.0.0",
      description="Python bindings for the LibIodef Library",
      author="CS-SI",
      url="https://www.libiodef-siem.org",
      license="GPL V2.1",
      package_dir={'iodef': '../../bindings/python'},
      py_modules=["iodef"],
      ext_modules=[Extension("_iodef",
                             ["_iodef.cxx"],
                             extra_compile_args=split_args("-I../.. -I../../src/include -I../../src/include -I../../src/libiodef-error -I../../bindings/c++/include"),
                             library_dirs=[ "../../src/.libs/", "../../bindings/c++/.libs/" ],
                             extra_link_args=split_args("-liodefcpp -liodef  -lgcrypt -ldl -lgpg-error -ldl  "))])
