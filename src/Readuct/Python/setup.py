import setuptools

# Read README.md for the long description
with open("README.md", "r") as fh:
  long_description = fh.read()

# Define the setup
setuptools.setup(
  name="scine_readuct",
  version="2.0.0",
  author="ETH Zurich, Laboratory for Physical Chemistry, Reiher Group",
  author_email="scine@phys.chem.ethz.ch",
  description="Open source quantum chemistry library",
  long_description=long_description,
  long_description_content_type="text/markdown",
  url="https://www.scine.ethz.ch",
  packages=["scine_readuct"],
  package_data={"scine_readuct": ["scine_readuct.so"]},
  classifiers=[
    "Programming Language :: Python",
    "Programming Language :: C++",
    "Development Status :: 5 - Production/Stable",
    "Intended Audience :: Science/Research",
    "License :: OSI Approved :: BSD License",
    "Natural Language :: English",
    "Topic :: Scientific/Engineering :: Chemistry"
  ],
  zip_safe=False,
  test_suite='pytest',
  tests_require=['pytest']
)
