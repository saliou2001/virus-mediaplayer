#!/bin/bash

# Update the system
sudo apt-get update

# Installation of GTK
sudo apt-get install libgtk-3-dev

# Installation of GST
sudo apt-get install gstreamer1.0-tools gstreamer1.0-plugins-base \
gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
gstreamer1.0-libav

# Installation of libgstreamer
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev

# Add the gstreamer1.0.pc file to the PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig