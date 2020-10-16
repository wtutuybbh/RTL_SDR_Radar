#!/bin/bash
find ../ -type f -name "*.o" -exec rm -rf '{}' +
find ../ -type f -name "moc_*.*" -exec rm -rf '{}' +
find ../ -type f -name "ui_*.h" -exec rm -rf '{}' +
find ../ -type f -name "*.*~" -exec rm -rf '{}' +
find ../ -type f -name "*.pro.user.*" -exec rm -rf '{}' +
find ../ -type f -name "*.so*" -exec rm -rf '{}' +
find ../ -type f -name "*_d" -exec rm -rf '{}' +
find ../ -type f -name "Makefile" -exec rm -rf '{}' +
find ../ -type f -name "qrc_*.*" -exec rm -rf '{}' +
rm -rf ../bin/debug/
rm -rf ../bin/release/