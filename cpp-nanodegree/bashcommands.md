| Command | Description | Notes / Example |
|---|---|---|
| sudo apt update | Refresh package lists from configured repositories | Run before apt install |
| sudo apt install <pkgs> | Install packages | e.g. sudo apt install build-essential cmake g++ libwxgtk3.2-dev |
| which <cmd> | Show full path of an executable on PATH | which wx-config |
| <cmd> --version | Print version for many tools | wx-config --version |
| mkdir -p <dir> | Create directory (no error if exists) | mkdir -p build |
| cmake -S . -B build -DVAR="value" | Configure CMake build (source ., build dir build) | cmake -S . -B build -DwxWidgets_CONFIG_EXECUTABLE="$(which wx-config)" |
| cmake --build build -- -j$(nproc) | Build project from build dir using all CPU cores | cmake --build build -- -j$(nproc) |
| cd <dir> && ./program | Change dir and run program | cd build && ./membot |
| rm -rf <path> | Remove files/directories recursively (dangerous) | rm -rf build/* (clean build) |
| wget <url> | Download a file from the web | wget https://.../wxWidgets-3.2.2.tar.bz2 |
| tar xjf <file.tar.bz2> | Extract bzip2-compressed tarball | tar xjf wxWidgets-3.2.2.tar.bz2 |
| ../configure [options] | Configure a source tree before make | ../configure --with-gtk=3 --enable-unicode --enable-stl |
| make -j$(nproc) | Build using all CPU cores | make -j$(nproc) |
| sudo make install | Install built software into system paths | (requires root) |
| sudo ldconfig | Refresh system linker cache after manual library installs | Run after sudo make install if libraries went to /usr/local/lib |