# Maxmod

A library to play music on the GBA and Nintendo DS. It supports MOD, XM, S3M, IT
and WAV files.

It's also possible to build a headless version of the library that can work in
any platform.

- [Documentation](https://blocksds.skylyrac.net/maxmod/index.html)

Please, report issues [here](https://codeberg.org/blocksds/sdk/issues).

## Testing

To run the tests, do:

```sh
cmake -B build
cmake --build build -j$(nproc --all)
ctest --test-dir build
```
