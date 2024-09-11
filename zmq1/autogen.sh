#!/bin/sh
# autogen.sh - Automatically generate configure script and build files

# Exit immediately if any command fails
set -e

# Check if required tools are installed
missing_programs=""

for cmd in autoreconf aclocal automake autoconf libtoolize; do
    if ! command -v $cmd > /dev/null 2>&1; then
        missing_programs="$missing_programs $cmd"
    fi
done

if [ ! -z "$missing_programs" ]; then
    echo "The following required tools are missing:$missing_programs"
    echo "Please install the necessary development tools and try again."
    exit 1
fi

# Run autoreconf to generate the configure script
echo "Running autoreconf to initialize the build system..."
autoreconf --install --force --verbose

echo "Initialization complete. You can now run './configure'."
