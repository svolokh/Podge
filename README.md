# Podge

## Building for Android

Use Gradle build script in `podge-android` repository.

## Building for iOS

```
$ PODGE_CORE_DIR=<path to this repo>
$ cmake "$PODGE_CORE_DIR" -DCMAKE_TOOLCHAIN_FILE:PATH="$PODGE_CORE_DIR/ios.toolchain.cmake" -DIOS_PLATFORM=<platform> -DSDL_JOYSTICK_ENABLED_BY_DEFAULT=OFF -DSDL_HAPTIC_ENABLED_BY_DEFAULT=OFF
```
