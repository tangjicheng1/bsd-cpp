from setuptools import setup, find_packages

setup(
    name='demo3',
    version='0.1',
    packages=find_packages(),
    package_data={
        'demo3_package': ['demo3.cpython-312-x86_64-linux-gnu.so'],
    },
    include_package_data=True,
    zip_safe=False,
)
