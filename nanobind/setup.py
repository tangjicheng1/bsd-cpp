from skbuild import setup
from setuptools import find_packages
import os

os.environ["CMAKE_GENERATOR"] = "Unix Makefiles"

setup(
    name="nanobind_demo_project",            # 包名
    version="0.1.0",                         # 版本号
    description="A demo project using nanobind",  # 描述
    author="Your Name",                      # 作者
    packages=find_packages(),                # 自动查找 Python 包
    cmake_install_dir=".",                   # 指定将构建的 .so 文件安装到哪个目录
    include_package_data=True,               # 包含 package 的数据文件
)
