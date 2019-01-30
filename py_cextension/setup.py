from distutils.core import setup, Extension

setup(name="myModule", version="1.1",
      ext_modules=[Extension("myModule", ['test.c', ])])


# 使用下面的命令生成库
# python setup.py build
# 使用下面的命令安装库
# python setup.py install
