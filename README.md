# Computer Vision in C — Image Processing & Object Detection

University project implementing a Computer Vision library from scratch in C, with image processing, colour segmentation, blob detection and object tracking using video input.

## What it does

Builds a custom image processing pipeline without external CV libraries — implementing core computer vision algorithms directly in C, applied to real-time video analysis and object detection.

## Features

- **Image I/O** — read and write bitmap images
- **Colour space conversion** — RGB to HSV
- **HSV segmentation** — isolate objects by colour range (hmin/hmax, smin/smax, vmin/vmax)
- **Binary morphology** — erosion and dilation with configurable kernel
- **Blob labelling** — connected component analysis
- **Blob information** — bounding box, area, centroid, perimeter, label
- **Video processing** — frame-by-frame analysis of .avi video files
- **Object detection** — detect and track objects by colour in video streams

## Core Data Structures

```c
// Image structure
typedef struct {
    unsigned char *data;
    int width, height;
    int channels;      // 1=grayscale/binary, 3=RGB
    int levels;        // 1=binary, [1-255]=grayscale/RGB
    int bytesperline;  // width * channels
} IVC;

// Object/Blob structure
typedef struct {
    int x, y, width, height;  // Bounding box
    int area;
    int xc, yc;                // Centre of mass
    int perimeter;
    int label;
} OVC;
```

## Processing Pipeline

```text
Video frame
    → RGB to HSV conversion
    → HSV colour segmentation (binary mask)
    → Morphological operations (erode / dilate)
    → Blob labelling (connected components)
    → Blob info extraction (bounding box, centroid)
    → Object detection output
```

## Tech Stack

- **Language:** C (C99)
- **IDE:** Visual Studio (MSVC)
- **Video:** OpenCV (for video capture only)
- **Core algorithms:** Custom implementation (no OpenCV processing)

## Project Structure

```text
Visao por Computador/
├── Visao por Computador.cpp    # Main application and pipeline
├── Source.cpp                  # Additional processing
├── vc.c                        # Core CV library implementation
├── vc.h                        # IVC/OVC structures and prototypes
└── video.avi                   # Sample input video
```

## Concepts Demonstrated

- Low-level image processing without high-level libraries
- Colour space mathematics (RGB to HSV)
- Mathematical morphology (erosion, dilation)
- Connected component labelling algorithm
- Computer vision pipeline design
- Real-time video frame processing
